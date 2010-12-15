struct block_function: actor<block_function> {
  actor_list elements;

  block_function (actor_list const& elements): elements(elements) {
    BOOST_FOREACH(function const & element, elements) {
      BOOST_ASSERT(!element.empty());
    }
  }

  utree eval (scope const& env) const {
    BOOST_ASSERT(!elements.empty());
    actor_list::const_iterator end = elements.end();
    --end;
    boost::iterator_range<actor_list::const_iterator>
    head_elements(elements.begin(), end);
    BOOST_FOREACH(function const & element, head_elements) {
      element(env);
    }
    return (*end)(env);
  }
};

struct block_composite: composite<block_composite> {
  function compose (actor_list const& elements) const {
    return function(block_function(elements));
  }
};

block_composite const block = block_composite();

