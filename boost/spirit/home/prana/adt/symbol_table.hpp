/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_ADT_SYMBOL_TABLE_HPP)
#define BOOST_SPIRIT_PRANA_ADT_SYMBOL_TABLE_HPP

#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/include/vector_fwd.hpp>
#include <boost/fusion/include/at_c.hpp>

#include <boost/noncopyable.hpp>
#include <boost/detail/iterator.hpp>

#include <boost/spirit/home/prana/adt/function_node.hpp>

namespace boost {
namespace spirit {
namespace prana {

struct schematic;

//[symbol_node
template<class Char, class Data>
struct symbol_node: private noncopyable {
  typedef Char id_type;

  typedef std::basic_string<Char>* key_type;
  typedef Data* mapped_type;
  typedef fusion::vector2<key_type, mapped_type> value_type;
  typedef value_type* pointer;
  typedef value_type const* const_pointer;
  
  symbol_node (id_type);

  struct destruct { /*< Destruction functor. >*/
    typedef void result_type;

    template<class Alloc>
    void operator() (symbol_node*, Alloc*) const;
  };

  struct find { /*< Lookup functor. >*/
    typedef pointer result_type;

    template<class Iterator>
    pointer operator() (symbol_node*, Iterator&, Iterator) const;
    
    template<class Iterator>
    const_pointer operator() (symbol_node const*, Iterator&,
                              Iterator) const;
    
    template<class Pointer, class Node, class Iterator>
    Pointer operator() (Node, Iterator&, Iterator) const;
  };

  struct insert { /*< Insertion functor. >*/
    typedef pointer result_type;

    template<class Iterator, class Value, class Alloc>
    pointer operator() (symbol_node*&, Iterator, Iterator,
                        Value const&, Alloc*) const;
  };

  struct erase { /*< Removal functor. >*/
    typedef pointer result_type;

    template<class Iterator, class Alloc>
    void operator() (symbol_node*&, Iterator, Iterator, Alloc*) const;
  };

  id_type id;        /*< The node's identity character. >*/
  pointer data;      /*< Optional data. >*/
  symbol_node* lt;   /*< Left pointer. >*/
  symbol_node* eq;   /*< Middle pointer. >*/
  symbol_node* gt;   /*< Right pointer. >*/
};
//]

template<class Char, class Data>
symbol_node<Char, Data>::symbol_node (id_type id_):
  id(id_), data(0), lt(0), eq(0), gt(0) { }

//[symbol_node_destruct_algorithm
template<class Char, class Data>
template<class Alloc>
inline void symbol_node<Char, Data>::destruct::operator() (
  symbol_node* p, Alloc* alloc
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
//]

template<class Char, class Data>
template<class Iterator>
inline typename symbol_node<Char, Data>::pointer
symbol_node<Char, Data>::find::operator() (
  symbol_node* root, Iterator& it, Iterator end
) const {
  return (*this).operator()<pointer, symbol_node*>(root, it, end);
}

template<class Char, class Data>
template<class Iterator>
inline typename symbol_node<Char, Data>::const_pointer
symbol_node<Char, Data>::find::operator() (
  symbol_node const* root, Iterator& it, Iterator end
) const {
  return (*this).operator()<const_pointer, symbol_node const*>(root, it, end);
}

//[symbol_node_lookup_algorithm
template<class Char, class Data>
template<class Pointer, class Node, class Iterator>
inline Pointer symbol_node<Char, Data>::find::operator() (
  Node root, Iterator& it, Iterator end
) const {
  if (it == end)
    return 0;

  Iterator i = it;
  Iterator latest = it;
  Node p = root;
  Pointer found = 0;

  while (p && i != end) {
    typename boost::detail::iterator_traits<Iterator>::value_type c = *i; 

    if (c == p->id) {
      if (p->data) {
        found = p->data;
        latest = i;
      }

      p = p->eq;
      i++;
    }

    else if (c < p->id) 
      p = p->lt;

    else
      p = p->gt;
  }

  if (found)
    it = ++latest; /*< one past the end matching char >*/

  return found;
}
//]

//[symbol_node_insertion_algorithm
template<class Char, class Data>
template<class Iterator, class Value, class Alloc>
inline typename symbol_node<Char, Data>::pointer
symbol_node<Char, Data>::insert::operator() (
  symbol_node*& root, Iterator first, Iterator last, Value const& val,
  Alloc* alloc
) const {
  if (first == last)
    return 0;

  Iterator it = first;

  symbol_node** pp = &root;

  for (;;) {
    typename boost::detail::iterator_traits<Iterator>::value_type c = *it;

    if (*pp == 0)
      *pp = alloc->new_node(*it);

    symbol_node* p = *pp;

    if (c == p->id) {
      if (++it == last) {
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
//]

//[symbol_node_erasure_algorithm
template<class Char, class Data>
template<class Iterator, class Alloc>
inline void symbol_node<Char, Data>::erase::operator() (
  symbol_node*& p, Iterator first, Iterator last, Alloc* alloc
) const {
  if (p == 0 || first == last)
    return;

  typename boost::detail::iterator_traits<Iterator>::value_type c = *first;
  
  if (c == p->id) {
    if (++first == last) {
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
//]

//[symbol_table
template<class Char, class Data = function_node<schematic> > 
class symbol_table: private noncopyable {
 public:
  typedef Char id_type;
  
  typedef std::basic_string<char>* key_type;
  typedef Data* mapped_type;
  typedef fusion::vector2<key_type, mapped_type> value_type;
  typedef value_type* pointer;
  typedef value_type const* const_pointer;

  typedef symbol_node<Char, Data> node;

  symbol_table (void);

  ~symbol_table (void);

  template<class Iterator>
    pointer find (Iterator&, Iterator);
  template<class Iterator>
    const_pointer find (Iterator&, Iterator) const;

  template<class Iterator, class Value>
    pointer insert (Iterator, Iterator, Value const&);

  template<class Iterator>
    void erase (Iterator, Iterator);

  void clear (void);

 protected:
  friend struct symbol_node<Char, Data>;

  node* new_node (id_type);

  template<class Iterator, class Value>
    pointer new_data (Iterator, Iterator, Value const&);

  void delete_node (node*);

  void delete_data (pointer);

 private:
  node* root;
};
//]

template<class Char, class Data>
symbol_table<Char, Data>::symbol_table (void): root(0) { }

template<class Char, class Data>
symbol_table<Char, Data>::~symbol_table (void) {
  clear();
}

template<class Char, class Data>
template<class Iterator>
typename symbol_table<Char, Data>::pointer
symbol_table<Char, Data>::find (Iterator& first, Iterator last) {
  return (root ? typename node::find()(root, first, last) : 0);
}

template<class Char, class Data>
template<class Iterator>
typename symbol_table<Char, Data>::const_pointer
symbol_table<Char, Data>::find (Iterator& first,
                                    Iterator last) const {
  return (root ? typename node::find()(root, first, last) : 0);
}

template<class Char, class Data>
template<class Iterator, class Value>
typename symbol_table<Char, Data>::pointer
symbol_table<Char, Data>::insert (Iterator first, Iterator last,
                                  Value const& val) {
  return typename node::insert()(root, first, last, val, this);
}

template<class Char, class Data>
template<class Iterator>
void symbol_table<Char, Data>::erase (Iterator first, Iterator last) {
  return typename node::erase()(root, first, last, this);
}

template<class Char, class Data>
void symbol_table<Char, Data>::clear (void) {
  if (root) {
    typename node::destruct()(root, this);
    root = 0;
  }
}

template<class Char, class Data>
typename symbol_table<Char, Data>::node*
symbol_table<Char, Data>::new_node (id_type id) {
  return new node(id);
}

template<class Char, class Data>
template<class Iterator, class Value>
typename symbol_table<Char, Data>::pointer
symbol_table<Char, Data>::new_data (Iterator first, Iterator last,
                                    Value const& val) {
  return new value_type(
    new std::basic_string<Char>(first, last), new Value(val)
  );
}

template<class Char, class Data>
void symbol_table<Char, Data>::delete_node (node* p) {
  if (p) 
    delete p;
}

template<class Char, class Data>
void symbol_table<Char, Data>::delete_data (pointer p) {
  if (p) {
    if (fusion::at_c<0>(*p))
      delete fusion::at_c<0>(*p);

    if (fusion::at_c<1>(*p))
      delete fusion::at_c<1>(*p);

    delete p;
  }
}

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_ADT_SYMBOL_TABLE_HPP 

