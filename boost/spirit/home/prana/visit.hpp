template<typename UTreeX, typename UTreeY = UTreeX>
struct visitor {
 public:
  template<typename F> // single dispatch
  typename F::result_type static apply (UTreeX& x, F f) { 
    typedef typename boost::mpl::if_<
      boost::is_const<UTreeX>,
      typename UTreeX::const_iterator,
      typename UTreeX::iterator
    >::type iterator;

    typedef boost::iterator_range<iterator> list_range;
    typedef utree_type type;

    switch (x.get_type()) {
      case type::nil_type:
        return f(nil());

      case type::bool_type:
        return f(x.b);

      case type::int_type:
        return f(x.i);

      case type::double_type:
        return f(x.d);

      case type::list_type:
        return f(list_range(iterator(x.l.first, 0), iterator(0, x.l.last)));

      case type::range_type:
        return f(list_range(iterator(x.r.first, 0), iterator(0, x.r.last)));

      case type::string_type:
        return f(utf8_string_range(x.s.str(), x.s.size()));

      case type::string_range_type:
        return f(utf8_string_range(x.sr.first, x.sr.last));

      case type::symbol_type:
        return f(utf8_symbol_range(x.s.str(), x.s.size()));

      case type::binary_type:
        return f(binary_range(x.s.str(), x.s.size()));

      case type::reference_type:
        return apply(*x.p, f);

      case type::any_type:
        return f(any_ptr(x.v.p, x.v.i));

      case type::function_type:
        return f(*x.pf);
    }
  }

  template <typename F>
  typename F::result_type
  static apply(UTreeX& x, UTreeY& y, F f) // double dispatch
  {
      typedef typename
          boost::mpl::if_<boost::is_const<UTreeX>,
          typename UTreeX::const_iterator,
          typename UTreeX::iterator>::type
      iterator;

      typedef boost::iterator_range<iterator> list_range;
      typedef utree_type type;

      switch (x.get_type())
      {
          default:
              BOOST_ASSERT(false); // can't happen

          case type::nil_type:
              nil x_;
              return visit_impl::apply(y, detail::bind(f, x_));

          case type::bool_type:
              return visit_impl::apply(y, detail::bind(f, x.b));

          case type::int_type:
              return visit_impl::apply(y, detail::bind(f, x.i));

          case type::double_type:
              return visit_impl::apply(y, detail::bind(f, x.d));

          case type::list_type:
              return visit_impl::apply(
                  y, detail::bind<F, list_range>(f,
                  list_range(iterator(x.l.first, 0), iterator(0, x.l.last))));

          case type::range_type:
              return visit_impl::apply(
                  y, detail::bind<F, list_range>(f,
                  list_range(iterator(x.r.first, 0), iterator(0, x.r.last))));

          case type::string_type:
              return visit_impl::apply(y, detail::bind(
                  f, utf8_string_range(x.s.str(), x.s.size())));

          case type::string_range_type:
              return visit_impl::apply(y, detail::bind(
                  f, utf8_string_range(x.sr.first, x.sr.last)));

          case type::symbol_type:
              return visit_impl::apply(y, detail::bind(
                  f, utf8_symbol_range(x.s.str(), x.s.size())));

          case type::binary_type:
              return visit_impl::apply(y, detail::bind(
                  f, binary_range(x.s.str(), x.s.size())));

          case type::reference_type:
              return apply(*x.p, y, f);

          case type::any_type:
              return visit_impl::apply(
                  y, detail::bind(f, any_ptr(x.v.p, x.v.i)));

          case type::function_type:
              return visit_impl::apply(y, detail::bind(f, *x.pf));

      }
  }
};
