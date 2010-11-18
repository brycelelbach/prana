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

template<class Char>
class intern_pool: private noncopyable {
 public:
  typedef Char char_type;
  typedef Char const* id_type;
  
  typedef iterator_range<Char const*> value_type;
  typedef iterator_range<Char const*>* pointer;
  typedef iterator_range<Char const*> const* const_pointer;

  typedef intern_node<Char> node;

  intern_pool (void);

  ~intern_pool (void);

  template<class Iterator>
    pointer find (Iterator&, Iterator);
  template<class Iterator>
    const_pointer find (Iterator&, Iterator) const;

  template<class Iterator>
    pointer insert (Iterator, Iterator);

  template<class Iterator>
    void erase (Iterator, Iterator);

  void clear (void);

 protected:
  friend struct intern_node<Char>;

  node* new_node (id_type);

  template<class Iterator>
    pointer new_data (Iterator, Iterator);

  void delete_node (node*);

  void delete_data (pointer);

 private:
  node* root;
};

template<class Char>
intern_pool<Char>::intern_pool (void): root(0) { }

template<class Char>
intern_pool<Char>::~intern_pool (void) {
  clear();
}

template<class Char>
template<class Iterator>
typename intern_pool<Char>::pointer
intern_pool<Char>::find (Iterator& first, Iterator last) {
  return (root ? typename node::find()(root, first, last) : 0);
}

template<class Char>
template<class Iterator>
typename intern_pool<Char>::const_pointer
intern_pool<Char>::find (Iterator& first, Iterator last) const {
  return (root ? typename node::find()(root, first, last) : 0);
}

template<class Char>
template<class Iterator>
typename intern_pool<Char>::pointer
intern_pool<Char>::insert (Iterator first, Iterator last) {
  return typename node::insert()(root, first, last, this);
}

template<class Char>
template<class Iterator>
void intern_pool<Char>::erase (Iterator first, Iterator last) {
  return typename node::erase()(root, first, last, this);
}

template<class Char>
void intern_pool<Char>::clear (void) {
  if (root) {
    typename node::destruct()(root, this);
    root = 0;
  }
}
template<class Char>
typename intern_pool<Char>::node* intern_pool<Char>::new_node (id_type id) {
  return new node(id);
}

template<class Char>
template<class Iterator>
typename intern_pool<Char>::pointer
intern_pool<Char>::new_data (Iterator first, Iterator last) {
  return new value_type(first, last);
}

template<class Char>
void intern_pool<Char>::delete_node (node* p) {
  if (p)
    delete p;
}

template<class Char>
void intern_pool<Char>::delete_data (pointer p) {
  if (p)
    delete p;
}

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_INTERN_POOL_HPP

