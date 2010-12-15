struct if_function: actor<if_function> {
  function cond;
  function then;
  function else_;

  if_function (function const& cond,
               function const& then,
               function const& else_): cond(cond), then(then), else_(else_) {
    BOOST_ASSERT(!cond.empty());
    BOOST_ASSERT(!then.empty());
    BOOST_ASSERT(!else_.empty());
  }

  utree eval (scope const& env) const {
    return cond(env).get<bool>() ? then(env) : else_(env);
  }
};

struct if_composite: composite<if_composite> {
  function compose (actor_list const& elements) const {
    actor_list::const_iterator i = elements.begin();
    function if_ = *i++;
    function then = *i++;
    function else_ = *i;
    return function(if_function(if_, then, else_));
  }
};

if_composite const if_ = if_composite();

