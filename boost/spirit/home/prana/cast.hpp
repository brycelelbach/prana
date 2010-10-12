template<typename To>
struct cast {
  typedef To result_type;

  template<typename From>
  To dispatch (From const& val, boost::mpl::true_) const {
    return To(val); // From is convertible to To
  }

  template<typename From>
  To dispatch (From const& val, boost::mpl::false_) const {
    // From is NOT convertible to To !!!
    throw std::bad_cast();
    return To();
  }

  template<typename From>
  To operator() (From const& val) const {
    // boost::iterator_range has a templated constructor, accepting
    // any argument and hence any type is 'convertible' to it.
    typedef typename boost::mpl::eval_if<
      is_iterator_range<To>,
      boost::is_same<From, To>,
      boost::is_convertible<From, To>
    >::type is_convertible;
    return dispatch(val, is_convertible());
  }
};

template <typename T>
struct cast<T*> {
  typedef T* result_type;

  template<typename From>
  T* operator() (From const& val) const {
    // From is NOT convertible to T !!!
    throw std::bad_cast();
    return 0;
  }

  T* operator() (any_ptr const& p) const {
    return p.get<T*>();
  }
};

