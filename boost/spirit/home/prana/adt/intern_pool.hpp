/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_INTERN_POOL_HPP)
#define BOOST_SPIRIT_PRANA_INTERN_POOL_HPP

#include <boost/spirit/home/prana/adt/intern_node.hpp>

namespace boost {
namespace spirit {
namespace prana {

template<class Iterator>
class intern_pool: private noncopyable {
 public:
  typedef Iterator id_type;
  
  typedef iterator_range<Iterator> value_type;
  typedef iterator_range<Iterator>* pointer;
  typedef iterator_range<Iterator> const* const_pointer;

  typedef intern_node<Iterator> node;

  intern_pool (void);

  ~intern_pool (void);

  template<class InputIterator>
    pointer find (InputIterator&, InputIterator);
  template<class InputIterator>
    const_pointer find (InputIterator&, InputIterator) const;

  template<class InputIterator>
    pointer insert (InputIterator, InputIterator);

  template<class InputIterator>
    void erase (InputIterator, InputIterator);

  void clear (void);

 protected:
  friend struct intern_node<Iterator>;

  node* new_node (id_type);

  template<class InputIterator>
    pointer new_data (InputIterator, InputIterator);

  void delete_node (node*);

  void delete_data (pointer);

 private:
  node* root;
};

template<class Iterator>
intern_pool<Iterator>::intern_pool (void): root(0) { }

template<class Iterator>
intern_pool<Iterator>::~intern_pool (void) {
  clear();
}

template<class Iterator>
template<class InputIterator>
typename intern_pool<Iterator>::pointer
intern_pool<Iterator>::find (InputIterator& first, InputIterator last) {
  return (root ? typename node::find()(root, first, last) : 0);
}

template<class Iterator>
template<class InputIterator>
typename intern_pool<Iterator>::const_pointer
intern_pool<Iterator>::find (InputIterator& first, InputIterator last) const {
  return (root ? typename node::find()(root, first, last) : 0);
}

template<class Iterator>
template<class InputIterator>
typename intern_pool<Iterator>::pointer
intern_pool<Iterator>::insert (InputIterator first, InputIterator last) {
  return typename node::insert()(root, first, last, this);
}

template<class Iterator>
template<class InputIterator>
void intern_pool<Iterator>::erase (InputIterator first, InputIterator last) {
  return typename node::erase()(root, first, last, this);
}

template<class Iterator>
void intern_pool<Iterator>::clear (void) {
  if (root) {
    typename node::destruct()(root, this);
    root = 0;
  }
}
template<class Iterator>
typename intern_pool<Iterator>::node*
intern_pool<Iterator>::new_node (id_type id) {
  return new node(id);
}

template<class Iterator>
template<class InputIterator>
typename intern_pool<Iterator>::pointer
intern_pool<Iterator>::new_data (InputIterator first, InputIterator last) {
  return new value_type(first, last);
}

template<class Iterator>
void intern_pool<Iterator>::delete_node (node* p) {
  if (p)
    delete p;
}

template<class Iterator>
void intern_pool<Iterator>::delete_data (pointer p) {
  if (p)
    delete p;
}

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_INTERN_POOL_HPP

