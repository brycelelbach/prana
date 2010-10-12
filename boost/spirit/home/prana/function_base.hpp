
  struct function_base
{
  virtual ~function_base() {};
  virtual utree operator()(scope const& env) const = 0;
  virtual function_base* clone() const = 0;
};
