/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_UTREE_FWD_HPP)
#define BOOST_SPIRIT_PRANA_UTREE_FWD_HPP

#include <string>

#include <boost/call_traits.hpp>
#include <boost/cstdint.hpp>

#include <boost/spirit/home/prana/adt/sequence_fwd.hpp>
#include <boost/spirit/home/prana/adt/symbol.hpp>
#include <boost/spirit/home/prana/kind.hpp>

namespace boost {
namespace spirit {
namespace prana {

struct shallow_copy;
struct deep_copy;

struct sequence;

template<class Char> struct symbol;

class utree {
 public:
  typedef utree          value_type;
  typedef utree&         reference;
  typedef utree const&   const_reference;
  typedef utree*         pointer;
  typedef utree const*   const_pointer;
  typedef std::ptrdiff_t difference_type;
  typedef std::size_t    size_type;

  union discriminated_union {
    pointer           _pointer; 
    sequence          _sequence;
    symbol<char>      _symbol;
    bool              _bool;
    boost::intmax_t   _integer;
    double            _floating;
  };

  utree (void);
  template<class T>
    utree (typename call_traits<T>::param_type);
  template<class T0, class T1>
    utree (typename call_traits<T0>::param_type,
           typename call_traits<T1>::param_type);
  
  template<class T>
    reference operator= (typename call_traits<T>::param_type);
  
  template<class T>
    void assign (typename call_traits<T>::param_type);
  template<class T0, class T1>
    void assign (typename call_traits<T0>::param_type,
                 typename call_traits<T1>::param_type);

  void swap (reference);

  boost::uint8_t kind (void) const;

  void clear (void);
  
  template<kind_type Kind>
    void become (void);

  template<class Value>
    void push_front (typename call_traits<Value>::param_type);
  template<class Value>
    void push_back (typename call_traits<Value>::param_type);
  
  void pop_front (void);
  void pop_back (void);

  reference operator* (void);
  const_reference operator* (void) const;

  pointer operator-> (void);
  const_pointer operator-> (void) const;
 
  bool operator== (const_reference) const;
  bool operator!= (const_reference) const;

 private:
  friend struct shallow_copy;
  friend struct deep_copy;

  template<typename Copy>
    void copy (const_reference, Copy);
  void copy (const_reference);
  void copy (bool);
  void copy (char);
  void copy (int);
  void copy (double);
  void copy (char const*);
  void copy (char const*, char const*);
  void copy (char const*, size_type);
  void copy (std::string const&);
 
  boost::uint8_t       _kind;
  discriminated_union  _du;
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_UTREE_FWD_HPP
