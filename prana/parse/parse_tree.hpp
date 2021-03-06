/*==============================================================================
    Copyright (c) 2010-2011 Bryce Lelbach

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(PRANA_2B628554_191E_4CC1_9CFA_0B45B4FB2C3F)
#define PRANA_2B628554_191E_4CC1_9CFA_0B45B4FB2C3F

#include <prana/config.hpp>

#include <ios>
#include <istream>

#include <boost/spirit/home/support/assert_msg.hpp>
#include <boost/spirit/include/support_istream_iterator.hpp>

#include <prana/magic.hpp>
#include <prana/support/ignore_utf_bom.hpp>
#include <prana/diagnostics/position_iterator.hpp>

namespace prana {

template<class Tag>
class parse_tree {
 public:
  typedef typename magic::source_type<Tag>::type source_type;
  typedef typename magic::annotations_type<Tag>::type annotations_type;

 private:
  utree _ast;
  annotations_type _annotations;
  source_type _source;
  
  template<class Char>
  bool set (std::basic_istream<Char>& in) {
    typedef spirit::basic_istream_iterator<Char>
      stream_iterator_type;

    // REVIEW: why aren't we using position_iterator here?
    typedef position_iterator<stream_iterator_type>
      iterator_type;

    typedef typename magic::parser_type<Tag, iterator_type>::type
      parser_type;

    // no white space skipping in the stream
    in.unsetf(std::ios::skipws);

    // ignore the UTF bom created by windows (no-op if Tag doesn't support UTF)
    ignore_utf_bom<magic::utf_version<Tag>::value>(
      in, magic::stringify_source<Tag>(_source)
    );

    parser_type p(_source, _annotations);

    stream_iterator_type sfirst(in);
    stream_iterator_type slast;
    iterator_type first(sfirst);
    iterator_type last(slast);

    bool r = magic::invoke<Tag>(first, last, p, _ast);

    // put the last character back.
    in.putback(*first);

    return r;
  } 
 
  template<class Range> 
  typename boost::disable_if<
    boost::is_base_of<std::ios_base, Range>, bool
  >::type set (Range const& in) {
    // REVIEW: why aren't we using position_iterator here?
    typedef position_iterator<typename Range::const_iterator>
      iterator_type;

    typedef typename magic::parser_type<Tag, iterator_type>::type
      parser_type;

    parser_type p(_source, _annotations);

    iterator_type first(in.begin());
    iterator_type last(in.end());

    return magic::invoke<Tag>(first, last, p, _ast);
  }

  bool set (utree const& in) {
    return set(in.get<spirit::utf8_string_range_type>());
  }

  void copy (parse_tree const& other) {
    _ast = other._ast;
    _annotations = other._annotations;
    _source = other._source;
  }

  template<class OtherTag> 
  void copy (parse_tree<OtherTag> const& other) {
    BOOST_SPIRIT_ASSERT_MSG(
      (mpl::eval_if<
        magic::has_source_locations<Tag>,
        magic::has_source_locations<OtherTag>,
        mpl::true_
      >::type),
      incompatible_parse_tree_tags, (Tag, OtherTag));

    _ast = other._ast;
    _source = magic::stringify_source<Tag>(other._source);

    magic::build_annotations(*this, other);
  }

  bool equal (parse_tree const& other) const {
    return (_ast == other._ast)
        && (_annotations == other._annotations)
        && (_source == other._source);
  }

 public:
  parse_tree (void):
    _ast(), _annotations(), _source(magic::default_source<Tag>::value) { }

  template<class In>
  parse_tree (In& in, source_type const& s =
                        magic::default_source<Tag>::value):
    _ast(), _annotations(), _source(s)
  { set(in); }  

  parse_tree (char const* in, source_type const& s =
                                magic::default_source<Tag>::value):
    _ast(), _annotations(), _source(s)
    // TODO: not optimal
  { set(std::string(in)); }  

  parse_tree (parse_tree const& other):
    _ast(), _annotations(), _source()
  {
    copy(other);
  } 

  template<class OtherTag>  
  parse_tree (parse_tree<OtherTag> const& other):
    _ast(), _annotations(), _source()
  {
    copy(other);
  } 
  
  parse_tree& operator= (parse_tree const& other) {
    if (!equal(other))
      copy(other);
    return *this;
  } 

  template<class OtherTag>  
  parse_tree& operator= (parse_tree<OtherTag> const& other) {
    copy(other);
    return *this;
  } 
  
  template<class In>
  parse_tree& operator= (In& in) {
    _ast.clear();
    _annotations.clear();
    _source = magic::default_source<Tag>::value;
    set(in);
  }
  
  parse_tree& operator= (char const* in) {
    _ast.clear();
    _annotations.clear();
    _source = magic::default_source<Tag>::value;
    // TODO: not optimal
    set(std::string(in));
  }
  
  void assign (parse_tree const& other) {
    if (!equal(other))
      copy(other);
    return *this;
  } 

  template<class OtherTag>  
  void assign (parse_tree<OtherTag> const& other) {
    copy(other);
    return *this;
  } 

  template<class In>
  void assign (In& in, source_type const& s =
                               magic::default_source<Tag>::value)
  {
    _ast.clear();
    _annotations.clear();
    _source = s; 
    set(in);
  }
  
  void assign (char const* in, source_type const& s =
                                 magic::default_source<Tag>::value)
  {
    _ast.clear();
    _annotations.clear();
    _source = s; 
    set(std::string(in));
  }

  utree& ast (void) {
    return _ast;
  }
  
  utree const& ast (void) const {
    return _ast;
  }

  annotations_type& annotations (void) {
    return _annotations;
  }
  
  annotations_type const& annotations (void) const {
    return _annotations;
  }

  source_type const& source (void) const {
    return _source;
  } 

  bool operator== (parse_tree const& other) const {
    return equal(other);
  }
  
  template<class OtherTag>  
  bool operator== (parse_tree<OtherTag> const&) const {
    return false;
  }
  
  bool operator!= (parse_tree const& other) const {
    return !equal(other);
  }
  
  template<class OtherTag>  
  bool operator!= (parse_tree<OtherTag> const&) const {
    return true;
  }
};

} // prana

#endif // PRANA_2B628554_191E_4CC1_9CFA_0B45B4FB2C3F

