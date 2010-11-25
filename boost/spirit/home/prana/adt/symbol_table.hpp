/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_SYMBOL_TABLE_HPP)
#define BOOST_SPIRIT_PRANA_SYMBOL_TABLE_HPP

#include <boost/spirit/home/prana/adt/symbol_node.hpp>

namespace boost {
namespace spirit {
namespace prana {

template<class Iterator, class Data>
class symbol_table: private noncopyable {
 public:
  typedef Iterator id_type;
  
  typedef iterator_range<Iterator> key_type;
  typedef Data mapped_type;
  typedef fusion::vector2<key_type, mapped_type> value_type;
  typedef value_type* pointer;
  typedef value_type const* const_pointer;

  typedef symbol_node<Iterator, Data> node;

  symbol_table (void);

  ~symbol_table (void);

  template<class InputIterator>
    pointer find (InputIterator&, InputIterator);
  template<class InputIterator>
    const_pointer find (InputIterator&, InputIterator) const;

  template<class InputIterator, class Value>
    pointer insert (InputIterator, InputIterator, Value const&);

  template<class InputIterator>
    void erase (InputIterator, InputIterator);

  void clear (void);

 protected:
  friend struct symbol_node<Iterator, Data>;

  node* new_node (id_type);

  template<class InputIterator, class Value>
    pointer new_data (InputIterator, InputIterator, Value const&);

  void delete_node (node*);

  void delete_data (pointer);

 private:
  node* root;
};

template<class Iterator, class Data>
symbol_table<Iterator, Data>::symbol_table (void): root(0) { }

template<class Iterator, class Data>
symbol_table<Iterator, Data>::~symbol_table (void) {
  clear();
}

template<class Iterator, class Data>
template<class InputIterator>
typename symbol_table<Iterator, Data>::pointer
symbol_table<Iterator, Data>::find (InputIterator& first, InputIterator last) {
  return (root ? typename node::find()(root, first, last) : 0);
}

template<class Iterator, class Data>
template<class InputIterator>
typename symbol_table<Iterator, Data>::const_pointer
symbol_table<Iterator, Data>::find (InputIterator& first, InputIterator last) const {
  return (root ? typename node::find()(root, first, last) : 0);
}

template<class Iterator, class Data>
template<class InputIterator, class Value>
typename symbol_table<Iterator, Data>::pointer
symbol_table<Iterator, Data>::insert (InputIterator first, InputIterator last,
                                      Value const& val) {
  return typename node::insert()(root, first, last, val, this);
}

template<class Iterator, class Data>
template<class InputIterator>
void symbol_table<Iterator, Data>::erase (InputIterator first, InputIterator last) {
  return typename node::erase()(root, first, last, this);
}

template<class Iterator, class Data>
void symbol_table<Iterator, Data>::clear (void) {
  if (root) {
    typename node::destruct()(root, this);
    root = 0;
  }
}
template<class Iterator, class Data>
typename symbol_table<Iterator, Data>::node*
symbol_table<Iterator, Data>::new_node (id_type id) {
  return new node(id);
}

template<class Iterator, class Data>
template<class InputIterator, class Value>
typename symbol_table<Iterator, Data>::pointer
symbol_table<Iterator, Data>::new_data (InputIterator first, InputIterator last,
                                        Value const& val) {
  return new value_type(key_type(first, last), val);
}

template<class Iterator, class Data>
void symbol_table<Iterator, Data>::delete_node (node* p) {
  if (p)
    delete p;
}

template<class Iterator, class Data>
void symbol_table<Iterator, Data>::delete_data (pointer p) {
  if (p)
    delete p;
}

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_SYMBOL_TABLE_HPP

