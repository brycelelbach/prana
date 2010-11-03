/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2010      Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_SPIRIT_PRANA_FUNCTIONAL_PUSHERS_HPP)
#define BOOST_SPIRIT_PRANA_FUNCTIONAL_PUSHERS_HPP

namespace boost {
namespace spirit {
namespace prana {

template<typename Container>
struct container_pusher { 
  struct front;
  struct back;
};

template<typename Container>
struct container_pusher<Container>::front {
  template<typename, typename>
  struct result { typedef void type; };

  template<typename Tree, typename Value>
  void operator() (Tree&, Value) const;
};

template<typename Container>
template<typename Tree, typename Value>
inline void container_pusher<Container>::front::operator() (
  Tree& tree, Value value
) const {
  // DISCUSS (wash): If the source is a reference, copy from the referenced
  // tree. Note that a bad reference will seg fault here. By design, the user is
  // responsible for validating shallow copies. Should this behavior be changed?
  if (from_kind & reference_kind) {
    (*this)(*tree._pointer, value);
    return;
  }

  switch ((kind_type) from_kind) {
    case reference_kind:
    case numeric_kind:
    case container_kind:
      // DISCUSS (wash): The above three are impossible; they fall through to
      // nil_kind for now. Perhaps they should throw an error if hit? 
    case nil_kind:
      break; // TODO (wash) 
    case symbol_kind:
      break; // TODO (wash) 
    case record_kind:
      break; // TODO (wash): Requires the implementation of record. 
    case bool_kind:
      (*this)(from_tree._bool, to_tree._bool);
      break; 
    case integer_kind:
      break; // TODO (wash) 
    case floating_kind:
      break; // TODO (wash) 
    case sequence_kind:
      tree._sequence.push_back(Tree(value));
      break; 
    case array_kind:
      break; // TODO (wash): Requires the implementation of array.
    case unique_kind:
      break; // TODO (wash): Requires the implementation of unique.
  }
}

template<typename Container>
struct container_pusher<Container>::back {
  template<typename, typename>
  struct result { typedef void type; };

  template<typename Tree, typename Value>
  void operator() (Tree&, Value) const;
};

template<typename Container>
template<typename Tree, typename Value>
inline void container_pusher<Container>::back::operator() (
  Tree& tree, Value value
) const {
  // TODO (wash)
}


} // prana
} // spirit
} // boost

#endif // BOOST_SPIRIT_PRANA_FUNCTIONAL_PUSHERS_HPP
