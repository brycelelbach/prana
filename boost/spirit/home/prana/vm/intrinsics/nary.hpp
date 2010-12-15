#define BSP_NARY_INTRINSIC(name, expression)                      \
  struct name##_function: nary_function<name##_function> {        \
    name##_function (actor_list const& elements) {                \
      base_type(elements) { }                                     \
                                                                  \
    typedef bool result_type;                                     \
                                                                  \
    bool eval (utree& result, utree const& element) const {       \
      expression;                                                 \
      return true;                                                \
    }                                                             \
  };                                                              \
                                                                  \
  struct name##_composite : nary_composite<name##_function> { };  \
                                                                  \
  name##_composite const name = name##_composite();               \
  /***/

BSP_NARY_INTRINSIC(plus,    result = result + element)
BSP_NARY_INTRINSIC(minus,   result = result - element)
BSP_NARY_INTRINSIC(times,   result = result * element)
BSP_NARY_INTRINSIC(divide,  result = result / element)
BSP_NARY_INTRINSIC(modulo,  result = result % element)
BSP_NARY_INTRINSIC(xor_,    result = result ^ element)
BSP_NARY_INTRINSIC(bitor_,  result = result | element)
BSP_NARY_INTRINSIC(bitand_, result = result & element)
BSP_NARY_INTRINSIC(or_,     result = result || element)
BSP_NARY_INTRINSIC(and_,    result = result && element)
BSP_NARY_INTRINSIC(shiftl,  result = result << element)
BSP_NARY_INTRINSIC(shiftr,  result = result >> element)

