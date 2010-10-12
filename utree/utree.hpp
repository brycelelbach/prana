/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(BOOST_SPIRIT_UTREE)
#define BOOST_SPIRIT_UTREE

#include <cstddef>
#include <algorithm>
#include <string>
#include <ostream>
#include <typeinfo>

#include <boost/assert.hpp>
#include <boost/noncopyable.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <boost/type_traits/is_polymorphic.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/ref.hpp>

#include <utree/detail/utree_detail1.hpp>

#if defined(BOOST_MSVC)
# pragma warning(push)
# pragma warning(disable: 4804)
# pragma warning(disable: 4805)
# pragma warning(disable: 4244)
#endif

namespace scheme
{
    ///////////////////////////////////////////////////////////////////////////
    // Our utree can store these types. This enum tells us what type
    // of data is stored in utree's discriminated union.
    ///////////////////////////////////////////////////////////////////////////
    struct utree_type
    {
        enum info
        {
            nil_type,
            bool_type,
            int_type,
            double_type,
            string_type,
            string_range_type,
            symbol_type,
            binary_type,
            list_type,
            range_type,
            reference_type,
            any_type,
            function_type
        };
    };

    ///////////////////////////////////////////////////////////////////////////
    // The nil type
    ///////////////////////////////////////////////////////////////////////////
    struct nil {};

    ///////////////////////////////////////////////////////////////////////////
    // A typed string with parametric Base storage. The storage can be any
    // range or (stl container) of chars.
    ///////////////////////////////////////////////////////////////////////////
    template <typename Base, utree_type::info type_>
    struct basic_string : Base
    {
        static utree_type::info const type = type_;

        basic_string()
          : Base() {}

        basic_string(Base const& base)
          : Base(base) {}

        template <typename Iterator>
        basic_string(Iterator bits, std::size_t len)
          : Base(bits, bits + len) {}

        template <typename Iterator>
        basic_string(Iterator first, Iterator last)
          : Base(first, last) {}

        basic_string& operator=(basic_string const& other)
        {
            Base::operator=(other);
            return *this;
        }

        basic_string& operator=(Base const& other)
        {
            Base::operator=(other);
            return *this;
        }
    };

    ///////////////////////////////////////////////////////////////////////////
    // Binary string
    ///////////////////////////////////////////////////////////////////////////
    typedef basic_string<
        boost::iterator_range<char const*>,
        utree_type::binary_type>
    binary_range;

    typedef basic_string<
        std::string,
        utree_type::binary_type>
    binary_string;

    ///////////////////////////////////////////////////////////////////////////
    // Our UTF-8 string
    ///////////////////////////////////////////////////////////////////////////
    typedef basic_string<
        boost::iterator_range<char const*>,
        utree_type::string_type>
    utf8_string_range;

    typedef basic_string<
        std::string,
        utree_type::string_type>
    utf8_string;

    ///////////////////////////////////////////////////////////////////////////
    // Our UTF-8 symbol (for identifiers)
    ///////////////////////////////////////////////////////////////////////////
    typedef basic_string<
        boost::iterator_range<char const*>,
        utree_type::symbol_type>
    utf8_symbol_range;

    typedef basic_string<
        std::string,
        utree_type::symbol_type>
    utf8_symbol;

    ///////////////////////////////////////////////////////////////////////////
    // Our function type
    ///////////////////////////////////////////////////////////////////////////
    class utree;
    class scope;

    struct function_base
    {
        virtual ~function_base() {};
        virtual utree operator()(scope const& env) const = 0;
        virtual function_base* clone() const = 0;
    };

    template <typename F>
    struct stored_function : function_base
    {
        F f;
        stored_function(F f = F());
        virtual ~stored_function();
        virtual utree operator()(scope const& env) const;
        virtual function_base* clone() const;
    };

    ///////////////////////////////////////////////////////////////////////////
    // Shallow tag. Instructs utree to hold an iterator_range
    // as-is without deep copying the range.
    ///////////////////////////////////////////////////////////////////////////
    struct shallow_tag {};
    shallow_tag const shallow = {};

    ///////////////////////////////////////////////////////////////////////////
    // A void* plus type_info
    ///////////////////////////////////////////////////////////////////////////
    class any_ptr
    {
    public:

        template <typename Ptr>
        typename boost::disable_if<
            boost::is_polymorphic<
                typename boost::remove_pointer<Ptr>::type>,
            Ptr>::type
        get() const
        {
            if (*i == typeid(Ptr))
            {
                return static_cast<Ptr>(p);
            }
            throw std::bad_cast();
        }

        template <typename T>
        any_ptr(T* p)
          : p(p), i(&typeid(T*))
        {}

        friend bool operator==(any_ptr const& a, any_ptr const& b)
        {
            return (a.p == b.p) && (*a.i == *b.i);
        }

    private:

        // constructor is private
        any_ptr(void* p, std::type_info const* i)
          : p(p), i(i) {}

        template <typename UTreeX, typename UTreeY>
        friend struct detail::visit_impl;

        friend class utree;

        void* p;
        std::type_info const* i;
    };

    ///////////////////////////////////////////////////////////////////////////
    // The main utree (Universal Tree) class
    // The utree is a hierarchical, dynamic type that can store:
    //  - a nil
    //  - a bool
    //  - an integer
    //  - a double
    //  - a string
    //  - a string range
    //  - a symbol (identifier)
    //  - binary data
    //  - a (doubly linked) list of utree
    //  - an iterator_range of list::iterator
    //  - a reference to a utree
    //  - a pointer or reference to any type
    //  - a function
    //
    // The utree has minimal memory footprint. The data structure size is
    // 16 bytes on a 32-bit platform. Being a container of itself, it can
    // represent tree structures.
    ///////////////////////////////////////////////////////////////////////////
    class utree
    {
    public:

        typedef utree value_type;
        typedef detail::list::node_iterator<utree> iterator;
        typedef detail::list::node_iterator<utree const> const_iterator;
        typedef detail::list::node_iterator<boost::reference_wrapper<utree> >
            ref_iterator;
        typedef utree& reference;
        typedef utree const& const_reference;
        typedef std::ptrdiff_t difference_type;
        typedef std::size_t size_type;

        typedef boost::iterator_range<iterator> range;
        typedef boost::iterator_range<const_iterator> const_range;

        utree();
        utree(bool b);
        utree(char c);
        utree(unsigned int i);
        utree(int i);
        utree(double d);
        utree(char const* str);
        utree(char const* str, std::size_t len);
        utree(std::string const& str);
        utree(boost::reference_wrapper<utree> ref);
        utree(any_ptr const& p);

        template <typename Iter>
        utree(boost::iterator_range<Iter> r);
        utree(range r, shallow_tag);
        utree(const_range r, shallow_tag);
        utree(utf8_string_range const& str, shallow_tag);

        template <typename F>
        utree(stored_function<F> const& pf);

        template <typename Base, utree_type::info type_>
        utree(basic_string<Base, type_> const& str);

        utree(utree const& other);
        ~utree();

        utree& operator=(utree const& other);
        utree& operator=(bool b);
        utree& operator=(unsigned int i);
        utree& operator=(int i);
        utree& operator=(double d);
        utree& operator=(char const* s);
        utree& operator=(std::string const& s);
        utree& operator=(boost::reference_wrapper<utree> ref);

        template <typename F>
        utree& operator=(stored_function<F> const& pf);

        template <typename Iter>
        utree& operator=(boost::iterator_range<Iter> r);

        template <typename Base, utree_type::info type_>
        utree& operator=(basic_string<Base, type_> const& bin);

        template <typename F>
        typename F::result_type
        static visit(utree const& x, F f);

        template <typename F>
        typename F::result_type
        static visit(utree& x, F f);

        template <typename F>
        typename F::result_type
        static visit(utree const& x, utree const& y, F f);

        template <typename F>
        typename F::result_type
        static visit(utree& x, utree const& y, F f);

        template <typename F>
        typename F::result_type
        static visit(utree const& x, utree& y, F f);

        template <typename F>
        typename F::result_type
        static visit(utree& x, utree& y, F f);

        template <typename T>
        void push_back(T const& val);

        template <typename T>
        void push_front(T const& val);

        template <typename T>
        iterator insert(iterator pos, T const& x);

        template <typename T>
        void insert(iterator pos, std::size_t, T const& x);

        template <typename Iter>
        void insert(iterator pos, Iter first, Iter last);

        template <typename Iter>
        void assign(Iter first, Iter last);

        void clear();
        void pop_front();
        void pop_back();
        iterator erase(iterator pos);
        iterator erase(iterator first, iterator last);

        utree& front();
        utree& back();
        utree const& front() const;
        utree const& back() const;

        utree& operator[](std::size_t i);
        utree const& operator[](std::size_t i) const;

        void swap(utree& other);

        iterator begin();
        iterator end();
        const_iterator begin() const;
        const_iterator end() const;

        ref_iterator ref_begin();
        ref_iterator ref_end();

        bool empty() const;
        std::size_t size() const;

        utree_type::info which() const;

        template <typename T>
        T get() const;

        utree& deref();
        utree const& deref() const;

        short tag() const;
        void tag(short tag);

        utree eval(scope const& env) const;

    private:

        typedef utree_type type;

        template <typename UTreeX, typename UTreeY>
        friend struct detail::visit_impl;
        friend struct detail::index_impl;

        template <typename T>
        friend struct detail::get_impl;

        type::info get_type() const;
        void set_type(type::info t);
        void ensure_list_type();
        void free();
        void copy(utree const& other);

        union
        {
            detail::fast_string s;
            detail::list l;
            detail::range r;
            detail::string_range sr;
            detail::void_ptr v;
            bool b;
            int i;
            double d;
            utree* p;
            function_base* pf;
        };
    };

    inline utree::utree()
    {
        set_type(type::nil_type);
    }

    inline utree::utree(bool b) : b(b)
    {
        set_type(type::bool_type);
    }

    inline utree::utree(char c)
    {
        // char constructs a single element string
        s.construct(&c, &c+1);
        set_type(type::string_type);
    }

    inline utree::utree(unsigned int i) : i(i)
    {
        set_type(type::int_type);
    }

    inline utree::utree(int i) : i(i)
    {
        set_type(type::int_type);
    }

    inline utree::utree(double d) : d(d)
    {
        set_type(type::double_type);
    }

    inline utree::utree(char const* str)
    {
        s.construct(str, str + strlen(str));
        set_type(type::string_type);
    }

    inline utree::utree(char const* str, std::size_t len)
    {
        s.construct(str, str + len);
        set_type(type::string_type);
    }

    inline utree::utree(std::string const& str)
    {
        s.construct(str.begin(), str.end());
        set_type(type::string_type);
    }

    template <typename Base, utree_type::info type_>
    inline utree::utree(basic_string<Base, type_> const& bin)
    {
        s.construct(bin.begin(), bin.end());
        set_type(type_);
    }

    inline utree::utree(boost::reference_wrapper<utree> ref)
      : p(ref.get_pointer())
    {
        set_type(type::reference_type);
    }

    inline utree::utree(any_ptr const& p)
    {
        v.p = p.p;
        v.i = p.i;
        set_type(type::any_type);
    }

    template <typename F>
    inline utree::utree(stored_function<F> const& pf)
      : pf(new stored_function<F>(pf))
    {
        set_type(type::function_type);
    }

    template <typename Iter>
    inline utree::utree(boost::iterator_range<Iter> r)
    {
        set_type(type::nil_type);
        assign(r.begin(), r.end());
    }

    inline utree::utree(range r, shallow_tag)
    {
        this->r.first = r.begin().node;
        this->r.last = r.end().prev;
        set_type(type::range_type);
    }

    inline utree::utree(const_range r, shallow_tag)
    {
        this->r.first = r.begin().node;
        this->r.last = r.end().prev;
        set_type(type::range_type);
    }

    inline utree::utree(utf8_string_range const& str, shallow_tag)
    {
        this->sr.first = str.begin();
        this->sr.last = str.end();
        set_type(type::string_range_type);
    }

    inline utree::utree(utree const& other)
    {
        copy(other);
    }

    inline utree::~utree()
    {
        free();
    }

    inline utree& utree::operator=(utree const& other)
    {
        if (this != &other)
        {
            free();
            copy(other);
        }
        return *this;
    }

    inline utree& utree::operator=(bool b_)
    {
        free();
        b = b_;
        set_type(type::bool_type);
        return *this;
    }

    inline utree& utree::operator=(unsigned int i_)
    {
        free();
        i = i_;
        set_type(type::int_type);
        return *this;
    }

    inline utree& utree::operator=(int i_)
    {
        free();
        i = i_;
        set_type(type::int_type);
        return *this;
    }

    inline utree& utree::operator=(double d_)
    {
        free();
        d = d_;
        set_type(type::double_type);
        return *this;
    }

    inline utree& utree::operator=(char const* s_)
    {
        free();
        s.construct(s_, s_ + strlen(s_));
        set_type(type::string_type);
        return *this;
    }

    inline utree& utree::operator=(std::string const& s_)
    {
        free();
        s.construct(s_.begin(), s_.end());
        set_type(type::string_type);
        return *this;
    }

    template <typename Base, utree_type::info type_>
    inline utree& utree::operator=(basic_string<Base, type_> const& bin)
    {
        free();
        s.construct(bin.begin(), bin.end());
        set_type(type_);
        return *this;
    }

    inline utree& utree::operator=(boost::reference_wrapper<utree> ref)
    {
        free();
        p = ref.get_pointer();
        set_type(type::reference_type);
        return *this;
    }

    template <typename F>
    utree& utree::operator=(stored_function<F> const& pf)
    {
        free();
        pf = new stored_function<F>(pf);
        set_type(type::function_type);
        return *this;
    }

    template <typename Iter>
    inline utree& utree::operator=(boost::iterator_range<Iter> r)
    {
        free();
        assign(r.begin(), r.end());
        return *this;
    }

    template <typename F>
    typename F::result_type
    inline utree::visit(utree const& x, F f)
    {
        return detail::visit_impl<utree const>::apply(x, f);
    }

    template <typename F>
    typename F::result_type
    inline utree::visit(utree& x, F f)
    {
        return detail::visit_impl<utree>::apply(x, f);
    }

    template <typename F>
    typename F::result_type
    inline utree::visit(utree const& x, utree const& y, F f)
    {
        return detail::visit_impl<utree const, utree const>::apply(x, y, f);
    }

    template <typename F>
    typename F::result_type
    inline utree::visit(utree const& x, utree& y, F f)
    {
        return detail::visit_impl<utree const, utree>::apply(x, y, f);
    }

    template <typename F>
    typename F::result_type
    inline utree::visit(utree& x, utree const& y, F f)
    {
        return detail::visit_impl<utree, utree const>::apply(x, y, f);
    }

    template <typename F>
    typename F::result_type
    inline utree::visit(utree& x, utree& y, F f)
    {
        return detail::visit_impl<utree, utree>::apply(x, y, f);
    }

    inline utree& utree::operator[](std::size_t i)
    {
        if (get_type() == type::reference_type)
            return (*p)[i];
        else if (get_type() == type::range_type)
            return detail::index_impl::apply(r.first, i);

        // otherwise...
        BOOST_ASSERT(get_type() == type::list_type && size() > i);
        return detail::index_impl::apply(l.first, i);
    }

    inline utree const& utree::operator[](std::size_t i) const
    {
        if (get_type() == type::reference_type)
            return (*(utree const*)p)[i];
        else if (get_type() == type::range_type)
            return detail::index_impl::apply(r.first, i);

        // otherwise...
        BOOST_ASSERT(get_type() == type::list_type && size() > i);
        return detail::index_impl::apply(l.first, i);
    }

    template <typename T>
    inline void utree::push_front(T const& val)
    {
        if (get_type() == type::reference_type)
            return p->push_front(val);
        ensure_list_type();
        l.push_front(val);
    }

    template <typename T>
    inline void utree::push_back(T const& val)
    {
        if (get_type() == type::reference_type)
            return p->push_back(val);
        ensure_list_type();
        l.push_back(val);
    }

    template <typename T>
    inline utree::iterator utree::insert(iterator pos, T const& val)
    {
        if (get_type() == type::reference_type)
            return p->insert(pos, val);
        ensure_list_type();
        if (pos == end())
        {
            push_back(val);
            return begin();
        }
        else
        {
            l.insert_before(val, pos.node);
            return utree::iterator(pos.node->prev, pos.node->prev->prev);
        }
    }

    template <typename T>
    inline void utree::insert(iterator pos, std::size_t n, T const& val)
    {
        if (get_type() == type::reference_type)
            return p->insert(pos, n, val);
        for (std::size_t i = 0; i != n; ++i)
            insert(pos, val);
    }

    template <typename Iter>
    inline void utree::insert(iterator pos, Iter first, Iter last)
    {
        if (get_type() == type::reference_type)
            return p->insert(pos, first, last);
        ensure_list_type();
        while (first != last)
            insert(pos, *first++);
    }

    template <typename Iter>
    inline void utree::assign(Iter first, Iter last)
    {
        if (get_type() == type::reference_type)
            return p->assign(first, last);
        ensure_list_type();
        clear();
        while (first != last)
        {
            push_back(*first);
            ++first;
        }
    }

    inline void utree::clear()
    {
        if (get_type() == type::reference_type)
            return p->clear();
        // clear will always make this a nil type
        free();
        set_type(type::nil_type);
    }

    inline void utree::pop_front()
    {
        if (get_type() == type::reference_type)
            return p->pop_front();
        BOOST_ASSERT(get_type() == type::list_type);
        l.pop_front();
    }

    inline void utree::pop_back()
    {
        if (get_type() == type::reference_type)
            return p->pop_back();
        BOOST_ASSERT(get_type() == type::list_type);
        l.pop_back();
    }

    inline utree::iterator utree::erase(iterator pos)
    {
        if (get_type() == type::reference_type)
            return p->erase(pos);
        BOOST_ASSERT(get_type() == type::list_type);
        detail::list::node* np = l.erase(pos.node);
        return iterator(np, np?np->prev:l.last);
    }

    inline utree::iterator utree::erase(iterator first, iterator last)
    {
        if (get_type() == type::reference_type)
            return p->erase(first, last);
        while (first != last)
            erase(first++);
        return last;
    }

    inline utree::iterator utree::begin()
    {
        if (get_type() == type::reference_type)
            return p->begin();
        else if (get_type() == type::range_type)
            return iterator(r.first, 0);

        // otherwise...
        ensure_list_type();
        return iterator(l.first, 0);
    }

    inline utree::iterator utree::end()
    {
        if (get_type() == type::reference_type)
            return p->end();
        else if (get_type() == type::range_type)
            return iterator(0, r.first);

        // otherwise...
        ensure_list_type();
        return iterator(0, l.last);
    }

    inline utree::ref_iterator utree::ref_begin()
    {
        if (get_type() == type::reference_type)
            return p->ref_begin();
        else if (get_type() == type::range_type)
            return ref_iterator(r.first, 0);

        // otherwise...
        ensure_list_type();
        return ref_iterator(l.first, 0);
    }

    inline utree::ref_iterator utree::ref_end()
    {
        if (get_type() == type::reference_type)
            return p->ref_end();
        else if (get_type() == type::range_type)
            return ref_iterator(0, r.first);

        // otherwise...
        ensure_list_type();
        return ref_iterator(0, l.last);
    }

    inline utree::const_iterator utree::begin() const
    {
        if (get_type() == type::reference_type)
            return ((utree const*)p)->begin();
        else if (get_type() == type::range_type)
            return const_iterator(r.first, 0);

        // otherwise...
        BOOST_ASSERT(get_type() == type::list_type);
        return const_iterator(l.first, 0);
    }

    inline utree::const_iterator utree::end() const
    {
        if (get_type() == type::reference_type)
            return ((utree const*)p)->end();
        else if (get_type() == type::range_type)
            return const_iterator(0, r.first);

        // otherwise...
        BOOST_ASSERT(get_type() == type::list_type);
        return const_iterator(0, l.last);
    }

    inline bool utree::empty() const
    {
        if (get_type() == type::reference_type)
            return ((utree const*)p)->empty();
        else if (get_type() == type::range_type)
            return r.first == 0;
        else if (get_type() == type::list_type)
            return l.size == 0;
        return get_type() == type::nil_type;
    }

    inline std::size_t utree::size() const
    {
        if (get_type() == type::reference_type)
        {
            return ((utree const*)p)->size();
        }
        else if (get_type() == type::range_type)
        {
            std::size_t size = 0;
            detail::list::node* n = r.first;
            while (n)
            {
                n = n->next;
                ++size;
            }
            return size;
        }
        else if (get_type() == type::list_type)
        {
            return l.size;
        }
        BOOST_ASSERT(get_type() == type::nil_type);
        return 0;
    }

    inline utree_type::info utree::which() const
    {
        return get_type();
    }

    inline utree& utree::front()
    {
        if (get_type() == type::reference_type)
        {
            return p->front();
        }
        else if (get_type() == type::range_type)
        {
            BOOST_ASSERT(r.first != 0);
            return r.first->val;
        }

        // otherwise...
        BOOST_ASSERT(get_type() == type::list_type && l.first != 0);
        return l.first->val;
    }

    inline utree& utree::back()
    {
        if (get_type() == type::reference_type)
        {
            return p->back();
        }
        else if (get_type() == type::range_type)
        {
            BOOST_ASSERT(r.last != 0);
            return r.last->val;
        }

        // otherwise...
        BOOST_ASSERT(get_type() == type::list_type && l.last != 0);
        return l.last->val;
    }

    inline utree const& utree::front() const
    {
        if (get_type() == type::reference_type)
        {
            return ((utree const*)p)->front();
        }
        else if (get_type() == type::range_type)
        {
            BOOST_ASSERT(r.first != 0);
            return r.first->val;
        }

        // otherwise...
        BOOST_ASSERT(get_type() == type::list_type && l.first != 0);
        return l.first->val;
    }

    inline utree const& utree::back() const
    {
        if (get_type() == type::reference_type)
        {
            return ((utree const*)p)->back();
        }
        else if (get_type() == type::range_type)
        {
            BOOST_ASSERT(r.last != 0);
            return r.last->val;
        }

        // otherwise...
        BOOST_ASSERT(get_type() == type::list_type && l.last != 0);
        return l.last->val;
    }

    inline void utree::swap(utree& other)
    {
        s.swap(other.s);
    }

    inline utree::type::info utree::get_type() const
    {
        // the fast string holds the type info
        return static_cast<utree::type::info>(s.get_type());
    }

    inline void utree::set_type(type::info t)
    {
        // the fast string holds the type info
        s.set_type(t);
    }

    inline void utree::ensure_list_type()
    {
        if (get_type() == type::nil_type)
        {
            set_type(type::list_type);
            l.default_construct();
        }
        else
        {
            BOOST_ASSERT(get_type() == type::list_type);
        }
    }

    inline void utree::free()
    {
        switch (get_type())
        {
            case type::binary_type:
            case type::symbol_type:
            case type::string_type:
                s.free();
                break;
            case type::list_type:
                l.free();
                break;
            case type::function_type:
                delete pf;
                break;
            default:
                break;
        };
    }

    inline void utree::copy(utree const& other)
    {
        set_type(other.get_type());
        switch (other.get_type())
        {
            case type::nil_type:
                break;
            case type::bool_type:
                b = other.b;
                break;
            case type::int_type:
                i = other.i;
                break;
            case type::double_type:
                d = other.d;
                break;
            case type::reference_type:
                p = other.p;
                break;
            case type::any_type:
                v = other.v;
                break;
            case type::range_type:
                r = other.r;
                break;
            case type::string_range_type:
                sr = other.sr;
                break;
            case type::function_type:
                pf = other.pf->clone();
                break;
            case type::string_type:
            case type::symbol_type:
            case type::binary_type:
                s.copy(other.s);
                break;
            case type::list_type:
                l.copy(other.l);
                s.tag(other.s.tag());
                break;
        }
    }
}

#if defined(BOOST_MSVC)
# pragma warning(pop)
#endif

#include <utree/detail/utree_detail2.hpp>

#endif
