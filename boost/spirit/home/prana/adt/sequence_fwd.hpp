/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_ADT_SEQUENCE_FWD_HPP)
#define BOOST_SPIRIT_PRANA_ADT_SEQUENCE_FWD_HPP

#include <boost/config.hpp>

namespace boost {
namespace spirit {
namespace prana {

struct utree;

struct sequence_iterator;
struct sequence_node;

template<typename Iterator> struct range;

struct sequence {
 public:
  typedef utree             value_type;
  typedef utree&            reference;
  typedef utree const&      const_reference;
  typedef utree*            pointer;
  typedef utree const*      const_pointer;
  typedef std::size_t       size_type;
  typedef sequence_iterator iterator;
  typedef sequence_iterator const_iterator;

  typedef range<iterator> range_type;
  
  void default_construct (void);
  
  void shallow_copy (sequence const&);
  
  void deep_copy (sequence const&);
  
  void clear (void);
  
  template<typename Container>
    Container get (void) const;

  // TODO (wash): Insertation needs to support shallow copying!

  template<typename T, typename Iterator>
    void insert (T const&, Iterator);

  template<typename T>
    void push_front (T const&);
  template<typename T>
    void push_back (T const&);

  void pop_front (void);
  void pop_back (void);

  iterator begin (void);
  const_iterator begin (void) const;

  iterator end (void);
  const_iterator end (void) const;

  template<typename Iterator>
    iterator erase (Iterator);

  template<typename Container>
    bool operator== (Container const&) const;
  template<typename Container>
    bool operator!= (Container const&) const;

  sequence_node* first;
  sequence_node* last;
};

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_ADT_SEQUENCE_FWD_HPP
