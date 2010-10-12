
template <typename F>
struct stored_function : function_base
{
  F f;
  stored_function(F f = F());
  virtual ~stored_function();
  virtual utree operator()(scope const& env) const;
  virtual function_base* clone() const;
};
