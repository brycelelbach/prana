    template <typename Derived>
    struct actor
    {
        typedef utree result_type;
        typedef actor<Derived> base_type;

        utree operator()(scope const& env) const
        {
            return derived().eval(env);
        }

        utree operator()() const
        {
            return derived().eval(scope());
        }

        template <typename A0>
        utree operator()(A0 const& _0) const
        {
            boost::array<utree, 1> elements;
            elements[0] = _0;
            return derived().eval(get_range(elements));
        }

        template <typename A0, typename A1>
        utree operator()(A0 const& _0, A1 const& _1) const
        {
            boost::array<utree, 2> elements;
            elements[0] = _0;
            elements[1] = _1;
            return derived().eval(get_range(elements));
        }

        template <std::size_t n>
        static scope
        get_range(boost::array<utree, n>& array)
        {
            return scope(array.begin(), array.end());
        }

        Derived const& derived() const
        {
            return *static_cast<Derived const*>(this);
        }
    };

