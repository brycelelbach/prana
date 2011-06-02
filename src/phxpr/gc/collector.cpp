////////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2011       Bryce Lelbach
//  Copyright (c) 2002, 2003 Peter Dimov
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////////

#if defined(PHXPR_SP_ENABLE_DEBUG_HOOKS)

#include <phxpr/config.hpp>

#include <cstdlib>
#include <map>
#include <deque>
#include <iostream>

#include <boost/assert.hpp>
#include <boost/smart_ptr/detail/lightweight_mutex.hpp>

#include <phxpr/gc/shared_ptr.hpp>
#include <phxpr/gc/collector.hpp>

namespace phxpr {

struct sweep_tag;

struct count_layout
{
    phxpr::detail::sp_counted_base * pi;
    std::size_t id;
};

struct shared_ptr_layout
{
    sweep_tag * px;
    count_layout pn;
};

typedef std::deque< phxpr::shared_ptr<sweep_tag> > free_list_type;
typedef std::map< void const *, long > reachable_object_type;
typedef std::deque< void const * > open_type;
typedef std::map< void const *, std::pair<void *, std::size_t> > arena_type;
typedef boost::detail::lightweight_mutex mutex_type;

arena_type & get_map()
{
    static arena_type m;
    return m;
}

mutex_type & get_mutex()
{
    static mutex_type m;
    return m;
}

static void * init_mutex_before_main = &get_mutex();

std::size_t const pointer_align = sizeof(void*);

void scan_and_count(void const * area, std::size_t size, arena_type const & m,
                    reachable_object_type & m2)
{
    unsigned char const * p = static_cast<unsigned char const *>(area);

    for(std::size_t n = 0; n + sizeof(shared_ptr_layout) <= size;
        p += pointer_align, n += pointer_align)
    {
        shared_ptr_layout const * q = reinterpret_cast<shared_ptr_layout const *>(p);

        if(q && q->pn.id == phxpr::detail::shared_count_id && q->pn.pi != 0 &&
           m.count(q->pn.pi) != 0)
        {
            ++m2[q->pn.pi];
        }
    }
}

void scan_and_mark(void const * area, std::size_t size,
                   reachable_object_type & m2, open_type & open)
{
    unsigned char const * p = static_cast<unsigned char const *>(area);

    for(size_t n = 0; n + sizeof(shared_ptr_layout) <= size;
        p += pointer_align, n += pointer_align)
    {
        shared_ptr_layout const * q = reinterpret_cast<shared_ptr_layout const *>(p);

        if(q && q->pn.id == phxpr::detail::shared_count_id && q->pn.pi != 0 &&
           m2.count(q->pn.pi) != 0)
        {
            open.push_back(q->pn.pi);
            m2.erase(q->pn.pi);
        }
    }
}

void find_unreachable_objects_impl(arena_type const & m,
                                   reachable_object_type & m2, bool report)
{
    // scan objects for shared_ptr members, compute internal counts

    {
        if (report)
            std::cout << "... " << m.size() << " objects in the world\n";

        for(arena_type::const_iterator i = m.begin(); i != m.end(); ++i)
        {
            phxpr::detail::sp_counted_base const * p = 0;

            p = static_cast<phxpr::detail::sp_counted_base const *>(i->first);

            // there should be no inactive counts in the map
            BOOST_ASSERT(p->use_count() != 0); 

            m2[ i->first ];

            if (i->second.first)
                scan_and_count(i->second.first, i->second.second, m, m2);
        }
    }

    // mark reachable objects

    {
        open_type open;

        for(reachable_object_type::iterator i = m2.begin(); i != m2.end(); ++i)
        {
            phxpr::detail::sp_counted_base const * p
                = static_cast<phxpr::detail::sp_counted_base const *>(i->first);
            if(p->use_count() != i->second) open.push_back(p);
        }

        if (report)
            std::cout << "... " << open.size() << " objects are directly reachable\n";

        for(open_type::iterator j = open.begin(); j != open.end(); ++j)
        {
            m2.erase(*j);
        }

        while(!open.empty())
        {
            void const * p = open.front();
            open.pop_front();

            arena_type::const_iterator i = m.find(p);
            BOOST_ASSERT(i != m.end());

            if (i->second.first)
                scan_and_mark(i->second.first, i->second.second, m2, open);
        }
    }

    // m2 now contains the unreachable objects

    if (report)
        std::cout << "... " << m2.size() << " objects are unreachable\n";
}

std::size_t find_unreachable_objects(bool report)
{
    reachable_object_type m2;

    #ifdef BOOST_HAS_THREADS
        // This will work without the #ifdef, but some compilers warn
        // that lock is not referenced
        mutex_type::scoped_lock lock(get_mutex());
    #endif

    arena_type const & m = get_map();

    find_unreachable_objects_impl(m, m2, report);

    if(report)
    {
        for(reachable_object_type::iterator j = m2.begin(); j != m2.end(); ++j)
        {
            arena_type::const_iterator i = m.find(j->first);
            BOOST_ASSERT(i != m.end());
            std::cout << "Unreachable object at " << i->second.first << ", "
                      << i->second.second << " bytes long.\n";
        }
    }

    return m2.size();
}

void scan_and_free(void * area, std::size_t size,
                   reachable_object_type const & m2, free_list_type & free)
{
    unsigned char * p = static_cast<unsigned char *>(area);

    for(std::size_t n = 0; n + sizeof(shared_ptr_layout) <= size;
        p += pointer_align, n += pointer_align)
    {
        shared_ptr_layout * q = reinterpret_cast<shared_ptr_layout *>(p);

        if(q->pn.id == phxpr::detail::shared_count_id && q->pn.pi != 0 &&
           m2.count(q->pn.pi) != 0 && q->px != 0)
        {
            phxpr::shared_ptr<sweep_tag> * ppx
                = reinterpret_cast< phxpr::shared_ptr<sweep_tag> * >(p);
            free.push_back(*ppx);
            ppx->reset();
        }
    }
}

void free_unreachable_objects(bool report)
{
    free_list_type free;

    {
        reachable_object_type m2;

        #ifdef BOOST_HAS_THREADS
            mutex_type::scoped_lock lock(get_mutex());
        #endif

        arena_type const & m = get_map();

        find_unreachable_objects_impl(m, m2, report);

        for(reachable_object_type::iterator j = m2.begin(); j != m2.end(); ++j)
        {
            arena_type::const_iterator i = m.find(j->first);
            BOOST_ASSERT(i != m.end());
            if (i->second.first)
                scan_and_free(i->second.first, i->second.second, m2, free);
        }
    }

    if (report)
        std::cout << "... about to free " << free.size() << " objects.\n";
}

///////////////////////////////////////////////////////////////////////////////
// debug hooks

void sp_scalar_constructor_hook(void * px, std::size_t size, void * pn)
{
    #ifdef BOOST_HAS_THREADS
        mutex_type::scoped_lock lock(get_mutex());
    #endif

    get_map()[pn] = std::make_pair(px, size);
}

void sp_scalar_destructor_hook(void *, std::size_t, void * pn)
{
    #ifdef BOOST_HAS_THREADS
        mutex_type::scoped_lock lock(get_mutex());
    #endif
    
    get_map().erase(pn);
}

void sp_array_constructor_hook(void *) { }

void sp_array_destructor_hook(void *) { }

} // phxpr

#endif 

