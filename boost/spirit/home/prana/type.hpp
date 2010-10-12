///////////////////////////////////////////////////////////////////////////
// Our utree can store these types. This enum tells us what type
// of data is stored in utree's discriminated union.
///////////////////////////////////////////////////////////////////////////
struct type {
  enum info
  {
    nil_type,
    bool_type,
    int_type,
    double_type,
    string_type,
    string_range_type,
    symbol_type,
    binary_type,
    list_type,
    range_type,
    reference_type,
    any_type,
    function_type
  };
};
