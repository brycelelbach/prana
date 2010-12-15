template < bool scoped = true >
struct vararg_function : actor<vararg_function<scoped> > {
  std::size_t n;
  std::size_t level;
  vararg_function(std::size_t n, std::size_t level = 0)
    : n(n),
      level(level)
  {}

  utree eval(scope const& env) const {
    scope const* eptr = &env;

    while (level != eptr->level())
      eptr = eptr->outer();

    utree result;

    for (std::size_t i = n; i < eptr->size(); ++i) {
      utree const& arg = (*eptr)[i];

      if (arg.which() != utree_type::function_type)
        result.push_back(utree(boost::ref(arg)));
      else
        result.push_back(arg.eval(*eptr));
    }

    return result;
  }
};

template <> // scoped = false
struct vararg_function<false> : actor<vararg_function<false> > {
  std::size_t n;
  vararg_function(std::size_t n, std::size_t level = 0)
    : n(n)
  {}

  utree eval(scope const& env) const {
    scope const* eptr = &env;
    utree result;

    for (std::size_t i = n; i < eptr->size(); ++i) {
      utree const& arg = (*eptr)[i];

      if (arg.which() != utree_type::function_type)
        result.push_back(utree(boost::ref(arg)));
      else
        result.push_back(arg.eval(*eptr));
    }

    return result;
  }
};

template < bool scoped = true >
struct vararg {
  typedef function result_type;
  function operator()(std::size_t n, std::size_t level = 0) const {
    return function(vararg_function<scoped>(n, level));
  }
};

// scoped varg
vararg<true> const varg = {};

// unscoped varg
vararg<false> const unscoped_varg = {};

// unscoped vargs
function const _1_ = unscoped_varg(0);
function const _2_ = unscoped_varg(1);
function const _3_ = unscoped_varg(2);
function const _4_ = unscoped_varg(3);
function const _5_ = unscoped_varg(4);
function const _6_ = unscoped_varg(5);
function const _7_ = unscoped_varg(6);
function const _8_ = unscoped_varg(7);
function const _9_ = unscoped_varg(8);
function const _10_ = unscoped_varg(10);

