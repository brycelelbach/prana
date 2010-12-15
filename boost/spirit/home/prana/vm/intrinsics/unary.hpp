#define BSP_UNARY_INTRINSIC(name, expression)                     \
  struct name##_function: unary_function<name##_function> {       \
    name##_function (function const& a):                          \
      base_type(a) { }                                            \
                                                                  \
    typedef utree result_type;                                    \
                                                                  \
    utree eval (utree const& element) const {                     \
      return expression;                                          \
    }                                                             \
  };                                                              \
                                                                  \
  struct name##_composite: unary_composite<name##_function> { };  \
                                                                  \
  name##_composite const name = name##_composite();               \
  /***/

BSP_UNARY_INTRINSIC(display, (std::cout << element, utree()))
BSP_UNARY_INTRINSIC(front,   element.front())
BSP_UNARY_INTRINSIC(back,    element.back())
BSP_UNARY_INTRINSIC(rest,    rest(element))

