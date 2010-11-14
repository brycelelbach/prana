/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_ADT_SEQUENCE_HPP)
#define BOOST_SPIRIT_PRANA_ADT_SEQUENCE_HPP

#include <cstring>
#include <climits>

#include <algorithm>

#include <boost/integer.hpp>
#include <boost/cstdint.hpp>

#include <boost/noncopyable.hpp>

#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/add_pointer.hpp>

#include <boost/iterator/iterator_facade.hpp>

#include <boost/spirit/home/prana/kind.hpp>
#include <boost/spirit/home/prana/tag.hpp>
#include <boost/spirit/home/prana/functional/copy.hpp>

namespace boost {
namespace spirit {
namespace prana {

template<typename Data>
struct sequence_node: private boost::noncopyable {
  typedef Data        value_type;
  typedef Data&       reference;
  typedef Data const& const_reference;
  typedef Data*       pointer;
  typedef Data const* const_pointer;
  typedef std::size_t size_type;

  sequence_node (sequence_node*, sequence_node*, sequence_node*&);

  template<typename Value>
    sequence_node (Value const&, sequence_node*, sequence_node*);

  ~sequence_node (void);

  void unlink (void);

  Data* _val;
  sequence_node* _next;
  sequence_node* _prev;
};

template<typename Data>
sequence_node<Data>::sequence_node (
  sequence_node* other_, sequence_node* prev_, sequence_node*& last_ 
):
  _val((other_->_val
       ? new Data(*other_->_val)
       : 0)),
  _next((other_->_next
        ? new sequence_node(other_->_next, this, last_)
        : 0)),
  _prev(prev_)
{
  if (!other_->_next)
    last_ = this; 
}

template<typename Data>
template<typename Value>
sequence_node<Data>::sequence_node (
  Value const& val_, sequence_node* next_, sequence_node* prev_ 
):
  _val(new Data(val_)),
  _next(next_),
  _prev(prev_) { }

template<typename Data>
sequence_node<Data>::~sequence_node (void) {
  if (_val) {
    delete _val;
    _val = 0;
  }
}

template<typename Data>
inline void sequence_node<Data>::unlink (void) {
  _prev->_next = _next;
  _next->_prev = _prev;
}

template<typename Data>
struct sequence_iterator: public iterator_facade<
  sequence_iterator<Data>, typename remove_const<Data>::type,
  boost::bidirectional_traversal_tag
> {
 public:
  typedef typename add_pointer<
    sequence_node<typename remove_const<Data>::type>
  >::type node_type;

  sequence_iterator (void);
  sequence_iterator (node_type, node_type);
  template<typename Iterator>
    sequence_iterator (Iterator const&);

  template<typename Iterator>
    sequence_iterator& operator= (Iterator const&);

  void increment (void);
  void decrement (void);

  template<typename Iterator>
    bool equal (Iterator const&) const;
  template<typename Iterator>
    bool operator== (Iterator const&) const;
  template<typename Iterator>
    bool operator!= (Iterator const&) const;

  typename sequence_iterator::reference dereference (void) const;

  node_type _curr;
  node_type _prev;
};

template<typename Data>
sequence_iterator<Data>::sequence_iterator (void):
  _curr(0), _prev(0) { }

template<typename Data>
sequence_iterator<Data>::sequence_iterator (node_type curr_, node_type prev_):
  _curr(curr_), _prev(prev_) { }

template<typename Data>
template<typename Iterator>
sequence_iterator<Data>::sequence_iterator (Iterator const& it):
  _curr(it._curr), _prev(it._prev) { }

template<typename Data>
template<typename Iterator>
sequence_iterator<Data>&
sequence_iterator<Data>::operator= (Iterator const& it) {
  if (*this != it) {
    _curr = it._curr;
    _prev = it._prev;
  }
  return *this;
}

template<typename Data>
void sequence_iterator<Data>::increment (void) {
  if (_curr != 0) { 
    _prev = _curr;
    _curr = _curr->_next;
  }
}

template<typename Data>
void sequence_iterator<Data>::decrement (void) {
  if (_prev != 0) {
    _curr = _prev;
    _prev = _prev->_prev;
  }
}

template<typename Data>
template<typename Iterator>
bool sequence_iterator<Data>::equal (Iterator const& it) const {
  return _curr == it._curr;
}

template<typename Data>
template<typename Iterator>
bool sequence_iterator<Data>::operator== (Iterator const& it) const {
  return _curr == it._curr;
}

template<typename Data>
template<typename Iterator>
bool sequence_iterator<Data>::operator!= (Iterator const& it) const {
  return _curr != it._curr;
}

template<typename Data>
typename sequence_iterator<Data>::reference
sequence_iterator<Data>::dereference (void) const {
  return *_curr->_val;
}

template<typename Data>
struct sequence {
 public:
  typedef Data                          value_type;
  typedef Data&                         reference;
  typedef Data const&                   const_reference;
  typedef Data*                         pointer;
  typedef Data const*                   const_pointer;
  typedef std::size_t                   size_type;
  typedef sequence_iterator<Data>       iterator;
  typedef sequence_iterator<Data const> const_iterator;
  typedef boost::uint8_t                metadata;
  typedef sequence_node<Data>           node_type; 

  struct storage {
    metadata _control [(sizeof(void*[1]) / 2)];
    boost::int_t<(sizeof(void*[1]) / 2) * CHAR_BIT>::exact _size;
    node_type* _first;
    node_type* _last;
  };

  void default_construct (void);
 
  void assign (sequence const&);
  
  void clear (void);
  
  template<typename Container>
    Container get (void) const;

  template<typename Value, typename Iterator>
    void insert (Iterator, Value const&);

  template<typename Value>
    void push_front (Value const&);
  
  template<typename Value>
    void push_back (Value const&);

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

  size_type size (void) const;
  
  metadata kind (void) const;
  void kind (metadata);

  storage _data;
};

template<typename Data>
inline void sequence<Data>::default_construct (void) {
  // TODO: Move default_construct calls into a functor.
  std::memset(&_data, 0, sizeof(storage));
  _data._control[0] = sequence_kind;
}

template<typename Data>
inline void sequence<Data>::assign (sequence const& other_) {
  // DISCUSS (wash): Do we have to clear here? Would it be more efficient to
  // reuse the memory allocated for nodes and/or data, or would the complexity
  // of those intrinsics make such semantics more trouble than they're worth?
  clear();
  _data._first = new node_type(other_._data._first, 0, _data._last);
  _data._control[0] = sequence_kind;
  _data._size = other_._data._size;
}

template<typename Data>
inline void sequence<Data>::clear (void) {
  node_type* p = _data._first;
  
  while (p) {
    node_type* _next = p->_next;
    delete p;
    p = _next;
  } 

  std::memset(&_data, 0, sizeof(storage)); 
}

template<typename Data>
template<typename Container>
inline Container sequence<Data>::get (void) const {
  return Container(begin(), end());
}

template<typename Data>
template<typename Value, typename Iterator>
inline void sequence<Data>::insert (Iterator pos_, Value const& val_) {
  if (!pos_._curr)
    return push_back(val_);

  node_type* new_node =
    new node_type(val_, pos_._curr, pos_._curr->_prev);

  if (pos_._curr->_prev)
    pos_._curr->_prev->_next = new_node;
  else
    _data._first = new_node;

  pos_._curr->_prev = new_node;
  ++_data._size;
}

template<typename Data>
template<typename Value>
inline void sequence<Data>::push_front (Value const& val_) {
  if (_data._first == 0) {
    node_type* new_node = new node_type(val_, 0, 0);
    _data._first = _data._last = new_node;
    ++_data._size;
  }

  else {
    node_type* new_node =
      new node_type(val_, _data._first, _data._first->_prev);
    _data._first->_prev = new_node;
    _data._first = new_node;
    ++_data._size;
  }
}
template<typename Data>
template<typename Value>
inline void sequence<Data>::push_back (Value const& val_) {
  if (_data._last == 0) {
    node_type* new_node = new node_type(val_, 0, 0);
    _data._first = _data._last = new_node;
    ++_data._size;
  }

  else {
    node_type* new_node =
      new node_type(val_, _data._last->_next, _data._last);
    _data._last->_next = new_node;
    _data._last = new_node;
    ++_data._size;
  }
}

template<typename Data>
inline void sequence<Data>::pop_front (void) {
  if (!_data._first)
    return;

  else if (_data._first == _data._last) {
    delete _data._first;
    _data._first = _data._last = 0;
    --_data._size;
  }

  else {
    node_type* np = _data._first;
    _data._first = _data._first->_next;
    _data._first->_prev = 0;
    delete np;
    --_data._size;
  }
}

template<typename Data>
inline void sequence<Data>::pop_back (void) {
  if (!_data._first)
    return;

  else if (_data._first == _data._last) { 
    delete _data._first;
    _data._first = _data._last = 0;
    --_data._size;
  }

  else {
    node_type* np = _data._last;
    _data._last = _data._last->_prev;
    _data._last->_next = 0;
    delete np;
    --_data._size;
  }
}

template<typename Data>
inline typename sequence<Data>::iterator
sequence<Data>::begin (void) {
  return iterator(_data._first, 0);
}

template<typename Data>
inline typename sequence<Data>::const_iterator
sequence<Data>::begin (void) const {
  return const_iterator(_data._first, 0);
}

template<typename Data>
inline typename sequence<Data>::iterator
sequence<Data>::end (void) {
  return iterator(0, _data._last);
}

template<typename Data>
inline typename sequence<Data>::const_iterator
sequence<Data>::end (void) const {
  return const_iterator(0, _data._last);
}

template<typename Data>
template<typename Iterator>
inline typename sequence<Data>::iterator sequence<Data>::erase (Iterator pos_) {
  if (pos_._curr == 0)
    return Iterator(0, _data._last);

  else if (pos_._curr == _data._first) {
    pop_front();
    return Iterator(_data._first, 0);
  }

  else if (pos_._curr == _data._last) {
    pop_back();
    return Iterator(0, _data._last);
  }

  // DISCUSS (wash): Size will get decremented here, even if the node given
  // isn't in this sequence. Should we fix this? It would be expensive.
  node_type* next(pos_._curr->_next);
  pos_._curr->unlink();
  delete pos_._curr;
  --_data._size;
  return Iterator(next, next->_prev);
}

template<typename Data>
template<typename Container>
inline bool sequence<Data>::operator== (Container const& c_) const {
  return std::equal(begin(), end(), c_.begin());
}

template<typename Data>
template<typename Container>
inline bool sequence<Data>::operator!= (Container const& c_) const {
  return !std::equal(begin(), end(), c_.begin());
}

template<typename Data>
inline typename sequence<Data>::size_type sequence<Data>::size (void) const {
  return _data._size;
}

template<typename Data>
inline typename sequence<Data>::metadata
sequence<Data>::kind (void) const {
  return _data._control[0];
}

template<typename Data>
inline void sequence<Data>::kind (metadata kind_) {
  _data._control[0] = kind_;
}

} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_ADT_SEQUENCE_HPP
