/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(BOOST_SPIRIT_UTREE_DETAIL2)
#define BOOST_SPIRIT_UTREE_DETAIL2

#if defined(BOOST_MSVC)
# pragma warning(push)
# pragma warning(disable: 4800)
#endif

#include <boost/type_traits/remove_pointer.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/utility/enable_if.hpp>

namespace scheme { namespace detail


}}

namespace scheme
{


    template <typename T>
    struct is_iterator_range
      : boost::mpl::false_
    {};

    template <typename Iterator>
    struct is_iterator_range<boost::iterator_range<Iterator> >
      : boost::mpl::true_
    {};

    template <typename To>
    struct utree_cast
    {
        typedef To result_type;

        template <typename From>
        To dispatch(From const& val, boost::mpl::true_) const
        {
            return To(val); // From is convertible to To
        }

        template <typename From>
        To dispatch(From const& val, boost::mpl::false_) const
        {
            // From is NOT convertible to To !!!
            throw std::bad_cast();
            return To();
        }

        template <typename From>
        To operator()(From const& val) const
        {
            // boost::iterator_range has a templated constructor, accepting
            // any argument and hence any type is 'convertible' to it.
            typedef typename boost::mpl::eval_if<
                is_iterator_range<To>
              , boost::is_same<From, To>, boost::is_convertible<From, To>
            >::type is_convertible;
            return dispatch(val, is_convertible());
        }
    };

    template <typename T>
    struct utree_cast<T*>
    {
        typedef T* result_type;

        template <typename From>
        T* operator()(From const& val) const
        {
            // From is NOT convertible to T !!!
            throw std::bad_cast();
            return 0;
        }

        T* operator()(any_ptr const& p) const
        {
            return p.get<T*>();
        }
    };

    template <typename T>
    inline T utree::get() const
    {
        return utree::visit(*this, utree_cast<T>());
    }

    inline utree& utree::deref()
    {
        return (get_type() == type::reference_type) ? *p : *this;
    }

    inline utree const& utree::deref() const
    {
        return (get_type() == type::reference_type) ? *p : *this;
    }

    inline short utree::tag() const
    {
        BOOST_ASSERT(get_type() == type::list_type);
        return s.tag();
    }

    inline void utree::tag(short tag)
    {
        ensure_list_type();
        s.tag(tag);
    }

    inline utree utree::eval(scope const& env) const
    {
        BOOST_ASSERT(get_type() == type::function_type);
        return (*pf)(env);
    }
}

#if defined(BOOST_MSVC)
# pragma warning(pop)
#endif
#endif
