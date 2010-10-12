  ///////////////////////////////////////////////////////////////////////////
  // Our UTF-8 symbol (for identifiers)
  ///////////////////////////////////////////////////////////////////////////
  typedef basic_string<
  boost::iterator_range<char const*>,
  utree_type::symbol_type>
  utf8_symbol_range;

  typedef basic_string<
  std::string,
  utree_type::symbol_type>
  utf8_symbol;
