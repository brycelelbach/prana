struct list_function: actor<list_function> {
  actor_list elements;

  list_function (actor_list const& elements): elements(elements) {
    BOOST_FOREACH(function const & element, elements) {
      BOOST_ASSERT(!element.empty());
    }
  }

  utree eval (scope const& env) const {
    utree result;
    BOOST_FOREACH(function const & element, elements) {
      result.push_back(element(env));
    }
    return result;
  }
};

struct list_composite: composite<list_composite> {
  function compose (actor_list const& elements) const {
    return function(list_function(elements));
  }
};

list_composite const list = list_composite();

