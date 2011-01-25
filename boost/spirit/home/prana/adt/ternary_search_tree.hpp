/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BSP_ADT_TERNARY_SEARCH_TREE_HPP)
#define BSP_ADT_TERNARY_SEARCH_TREE_HPP

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/detail/iterator.hpp>

#include <boost/spirit/home/support/string_traits.hpp>

namespace boost {
namespace spirit {
namespace prana {

template<class Char, class Data>
struct tst_node {
  typedef Char id_type;

  typedef Data value_type;
  typedef Data* pointer;
  typedef Data const* const_pointer;
  
  tst_node (id_type);

  struct destruct { // destruction functor 
    typedef void result_type;

    template<class Alloc>
    result_type operator() (tst_node*, Alloc*) const;
  };

  struct find { // lookup functor
    template<class> struct result;

    template<class This, class Iterator>
    struct result<This(tst_node*, Iterator&, Iterator)> {
      typedef pointer type;
    };

    template<class This, class Iterator>
    struct result<This(tst_node const*, Iterator&, Iterator)> {
      typedef const_pointer type;
    };

    template<class Iterator>
    pointer operator() (tst_node*, Iterator&,
                        Iterator) const;
    
    template<class Iterator>
    const_pointer operator() (tst_node const*, Iterator&,
                              Iterator) const;
    
    template<class Pointer, class Node, class Iterator>
    static Pointer call (Node, Iterator&, Iterator);
  };

  struct insert { // insertion functor
    typedef pointer result_type;

    template<class Iterator, class Value, class Alloc>
    result_type operator() (tst_node*&, Iterator, Iterator,
                            Value const&, Alloc*) const;
  };

  struct erase { // removal functor
    typedef void result_type;

    template<class Iterator, class Alloc>
    result_type operator() (tst_node*&, Iterator, Iterator,
                            Alloc*) const;
  };

  struct clone { // clone functor
    typedef tst_node* result_type;

    template<class Alloc>
    result_type operator() (tst_node const*, Alloc*) const;
  }; 

  struct for_each { // for_each functor
    typedef void result_type;

    template<class Node, class F>
    result_type operator() (Node*, std::basic_string<Char>, F) const;
  }; 
  
  id_type id;   // the node's identity character
  pointer data; // optional data
  tst_node* lt; // left pointer
  tst_node* eq; // middle pointer
  tst_node* gt; // right pointer
};

template<class Char, class Data>
tst_node<Char, Data>::tst_node (id_type id_):
  id(id_), data(0), lt(0), eq(0), gt(0) { }

template<class Char, class Data>
template<class Alloc>
inline void tst_node<Char, Data>::destruct::operator() (
  tst_node* p, Alloc* alloc
) const {
  if (p) {
    if (p->data)
      alloc->delete_data(p->data);

    (*this)(p->lt, alloc);
    (*this)(p->eq, alloc);
    (*this)(p->gt, alloc);
    alloc->delete_node(p);
  }
}

template<class Char, class Data>
template<class Iterator>
inline typename tst_node<Char, Data>::pointer
tst_node<Char, Data>::find::operator() (
  tst_node* root, Iterator& it, Iterator end
) const {
  return call<pointer>(root, it, end);
}

template<class Char, class Data>
template<class Iterator>
inline typename tst_node<Char, Data>::const_pointer
tst_node<Char, Data>::find::operator() (
  tst_node const* root, Iterator& it, Iterator end
) const {
  return call<const_pointer>(root, it, end);
}

template<class Char, class Data>
template<class Pointer, class Node, class Iterator>
inline Pointer tst_node<Char, Data>::find::call (
  Node root, Iterator& it, Iterator end
) {
  if (it == end)
    return 0;

  Iterator i = it;
  Iterator latest = it;
  Node p = root;
  Pointer found = 0;

  while (p && i != end) {
    typename boost::detail::iterator_traits<Iterator>::value_type
      c = traits::deref(i); 

    if (c == p->id) {
      if (p->data) {
        found = p->data;
        latest = i;
      }

      p = p->eq;
      traits::next(i);
    }

    else if (c < p->id) 
      p = p->lt;

    else
      p = p->gt;
  }

  if (found) {
    traits::next(latest); // one past the end matching char
    it = latest;
  }

  return found;
}

template<class Char, class Data>
template<class Iterator, class Value, class Alloc>
inline typename tst_node<Char, Data>::pointer
tst_node<Char, Data>::insert::operator() (
  tst_node*& root, Iterator first, Iterator last,
  Value const& val, Alloc* alloc
) const {
  if (first == last)
    return 0;

  Iterator it = first;

  tst_node** pp = &root;

  for (;;) {
    typename boost::detail::iterator_traits<Iterator>::value_type
      c = traits::deref(it); 

    if (*pp == 0)
      *pp = alloc->new_node(traits::deref(it));

    tst_node* p = *pp;

    if (c == p->id) {
      traits::next(it);
      if (it == last) {
        if (p->data == 0)
          p->data = alloc->new_data(first, last, val);

        return p->data;
      }

      pp = &p->eq;
    }

    else if (c < p->id)
      pp = &p->lt;

    else
      pp = &p->gt;
  }
}

template<class Char, class Data>
template<class Iterator, class Alloc>
inline void tst_node<Char, Data>::erase::operator() (
  tst_node*& p, Iterator first, Iterator last, Alloc* alloc
) const {
  if (p == 0 || first == last)
    return;

  typename boost::detail::iterator_traits<Iterator>::value_type
    c = traits::deref(first); 
  
  if (c == p->id) {
    traits::next(first);
    if (first == last) {
      if (p->data) {
        alloc->delete_data(p->data);
        p->data = 0;
      }
    }

    (*this)(p->eq, first, last, alloc);
  }

  else if (c < p->id)
    (*this)(p->lt, first, last, alloc);
  
  else
    (*this)(p->gt, first, last, alloc);

  if (p->data == 0 && p->lt == 0 && p->eq == 0 && p->gt == 0) {
    alloc->delete_node(p);
    p = 0;
  }
}

template<class Char, class Data>
template<class Alloc>
inline tst_node<Char, Data>*
tst_node<Char, Data>::clone::operator() (
  tst_node const* p, Alloc* alloc
) const {
  if (p) {
    tst_node* c = alloc->new_node(p->id);

    if (p->data)
      c->data = alloc->new_data(*p->data);

    c->lt = (*this)(p->lt, alloc);
    c->eq = (*this)(p->eq, alloc);
    c->gt = (*this)(p->gt, alloc);
    return c;
  }

  return 0;
}
        
template<class Char, class Data>
template<class Node, class F>
inline void tst_node<Char, Data>::for_each::operator() (
  Node* p, std::basic_string<Char> prefix, F f
) const {
  if (p) {
    (*this)(p->lt, prefix, f);

    std::basic_string<Char> s = prefix + p->id;

    (*this)(p->eq, s, f);

    if (p->data)
      f(s, *p->data);

    (*this)(p->gt, prefix, f);
  }
}

template<class Char, class Data> 
class ternary_search_tree {
 public:
  typedef Char id_type;
  
  typedef Data value_type;
  typedef Data* pointer;
  typedef Data const* const_pointer;
  typedef Data& reference;
  typedef Data const& const_reference;

  typedef tst_node<Char, Data> node;

  ternary_search_tree (void);

  ~ternary_search_tree (void);

  template<class Container>
    reference operator[] (Container const& c);
  template<class Container>
    const_reference operator[] (Container const& c) const;

  template<class Container>
    pointer find (Container const& c);
  template<class Iterator>
    pointer find (Iterator& first, Iterator last);

  template<class Container>
    const_pointer find (Container const& c) const;
  template<class Iterator>
    const_pointer find (Iterator& first, Iterator last) const;

  template<class Container, class Value>
    pointer insert (Container const&, Value const&);
  template<class Iterator, class Value>
    pointer insert (Iterator, Iterator, Value const&);

  template<class Container>
    void erase (Container const&);
  template<class Iterator>
    void erase (Iterator, Iterator);

  void clear (void);
  
  template<class F>
    void for_each (F);

 protected:
  friend struct tst_node<Char, Data>;

  node* new_node (id_type);

  template<class Iterator, class Value>
    typename disable_if<is_base_of<Data, Value>, pointer>::type
    new_data (Iterator, Iterator, Value const&);

  template<class Iterator, class Value>
    typename enable_if<is_base_of<Data, Value>, pointer>::type
    new_data (Iterator, Iterator, Value const&);

  void delete_node (node*);

  void delete_data (pointer);

 private:
  node* root;
};

template<class Char, class Data>
ternary_search_tree<Char, Data>::ternary_search_tree (void): root(0) { }

template<class Char, class Data>
ternary_search_tree<Char, Data>::~ternary_search_tree (void) {
  clear();
}

template<class Char, class Data>
template<class Container>
typename ternary_search_tree<Char, Data>::reference
ternary_search_tree<Char, Data>::operator[] (Container const& c) {
  typedef typename result_of::get_begin<Char, Container const>::type iterator;
  iterator first = traits::get_begin<Char>(c), last = traits::get_end<Char>(c);
  return *(typename node::insert()(root, first, last, Data(), this));
}

template<class Char, class Data>
template<class Container>
typename ternary_search_tree<Char, Data>::const_reference
ternary_search_tree<Char, Data>::operator[] (Container const& c) const {
  typedef typename result_of::get_begin<Char, Container const>::type iterator;
  iterator first = traits::get_begin<Char>(c), last = traits::get_end<Char>(c);
  return *(typename node::insert()(root, first, last, Data(), this));
}

template<class Char, class Data>
template<class Container>
typename ternary_search_tree<Char, Data>::pointer
ternary_search_tree<Char, Data>::find (Container const& c) {
  typedef typename result_of::get_begin<Char, Container const>::type iterator;
  iterator first = traits::get_begin<Char>(c), last = traits::get_end<Char>(c);
  return (root ? typename node::find()(root, first, last) : 0);
}

template<class Char, class Data>
template<class Iterator>
typename ternary_search_tree<Char, Data>::pointer
ternary_search_tree<Char, Data>::find (
  Iterator& first, Iterator last
) {
  return (root ? typename node::find()(root, first, last) : 0);
}

template<class Char, class Data>
template<class Container>
typename ternary_search_tree<Char, Data>::const_pointer
ternary_search_tree<Char, Data>::find (Container const& c) const {
  typedef typename result_of::get_begin<Char, Container const>::type iterator;
  iterator first = traits::get_begin<Char>(c), last = traits::get_end<Char>(c);
  return (root ? typename node::find()(root, first, last) : 0);
}

template<class Char, class Data>
template<class Iterator>
typename ternary_search_tree<Char, Data>::const_pointer
ternary_search_tree<Char, Data>::find (Iterator& first,
                                       Iterator last) const
{
  return (root ? typename node::find()(root, first, last) : 0);
}

template<class Char, class Data>
template<class Container, class Value>
typename ternary_search_tree<Char, Data>::pointer
ternary_search_tree<Char, Data>::insert (Container const& c,
                                         Value const& val) 
{
  typedef typename result_of::get_begin<Char, Container const>::type iterator;
  iterator first = traits::get_begin<Char>(c), last = traits::get_end<Char>(c);
  return typename node::insert()(root, first, last, val, this);
}

template<class Char, class Data>
template<class Iterator, class Value>
typename ternary_search_tree<Char, Data>::pointer
ternary_search_tree<Char, Data>::insert (Iterator first, Iterator last,
                                         Value const& val)
{
  return typename node::insert()(root, first, last, val, this);
}

template<class Char, class Data>
template<class Container>
void ternary_search_tree<Char, Data>::erase (Container const& c) {
  typedef typename result_of::get_begin<Char, Container const>::type iterator;
  iterator first = traits::get_begin<Char>(c), last = traits::get_end<Char>(c);
  return typename node::erase()(root, first, last, this);
}

template<class Char, class Data>
template<class Iterator>
void ternary_search_tree<Char, Data>::erase (Iterator first, Iterator last) {
  return typename node::erase()(root, first, last, this);
}

template<class Char, class Data>
void ternary_search_tree<Char, Data>::clear (void) {
  if (root) {
    typename node::destruct()(root, this);
    root = 0;
  }
}

template<class Char, class Data>
template<class F>
void ternary_search_tree<Char, Data>::for_each (F f) {
  return typename node::for_each()(root, std::basic_string<Char>(), f);
}

template<class Char, class Data>
typename ternary_search_tree<Char, Data>::node*
ternary_search_tree<Char, Data>::new_node (id_type id) {
  return new node(id);
}

template<class Char, class Data>
template<class Iterator, class Value>
typename disable_if<is_base_of<Data, Value>, Data*>::type
ternary_search_tree<Char, Data>::new_data (Iterator first, Iterator last,
                                           Value const& val)
{
  return new Data(val);
}

template<class Char, class Data>
template<class Iterator, class Value>
typename enable_if<is_base_of<Data, Value>, Data*>::type
ternary_search_tree<Char, Data>::new_data (Iterator first, Iterator last,
                                           Value const& val)
{
  return new Value(val);
}

template<class Char, class Data>
void ternary_search_tree<Char, Data>::delete_node (node* p) {
  if (p) 
    delete p;
}

template<class Char, class Data>
void ternary_search_tree<Char, Data>::delete_data (pointer p) {
  if (p) 
    delete p;
}

} // prana
} // spirit
} // boost

#endif // BSP_ADT_TERNARY_SEARCH_TREE_HPP 

