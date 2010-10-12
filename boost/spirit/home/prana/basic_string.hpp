
///////////////////////////////////////////////////////////////////////////
// A typed string with parametric Base storage. The storage can be any
// range or (stl container) of chars.
///////////////////////////////////////////////////////////////////////////
template <typename Base, utree_type::info type_>
struct basic_string : Base
{
  static utree_type::info const type = type_;

  basic_string()
    : Base() {}

  basic_string(Base const& base)
    : Base(base) {}

  template <typename Iterator>
    basic_string(Iterator bits, std::size_t len)
    : Base(bits, bits + len) {}

  template <typename Iterator>
    basic_string(Iterator first, Iterator last)
    : Base(first, last) {}

  basic_string& operator=(basic_string const& other)
  {
    Base::operator=(other);
    return *this;
  }

  basic_string& operator=(Base const& other)
  {
    Base::operator=(other);
    return *this;
  }
};
