
// simple binder for binary visitation (we don't want to bring in the big guns)
template<typename F, typename X>
struct binder {
 private:
  X& x; // always by reference
  F f;

 public:
  typedef typename F::result_type result_type;

  binder (F f, X& x): x(x), f(f) { }

  template<typename Y>
  typename F::result_type operator() (Y& y) const { return f(x, y); }

  template<typename Y>
  typename F::result_type operator() (Y const& y) const { return f(x, y); }
};

template<typename F, typename X>
binder<F, X const> bind(F f, X const& x) { return binder<F, X const>(f, x); }

template<typename F, typename X>
binder<F, X> bind(F f, X& x) { return binder<F, X>(f, x); }
