    ///////////////////////////////////////////////////////////////////////////
    // unary_function
    ///////////////////////////////////////////////////////////////////////////
    template <typename Derived>
    struct unary_function : actor<unary_function<Derived> >
    {
        function a;
        typedef unary_function<Derived> base_type;

        unary_function(function const& a)
          : a(a)
        {
            BOOST_ASSERT(!a.empty());
        }

        utree eval(scope const& env) const
        {
            return derived().eval(a(env));
        }

        Derived const& derived() const
        {
            return *static_cast<Derived const*>(this);
        }
    };

    template <typename Function>
    struct unary_composite : composite<unary_composite<Function> >
    {
        function compose(actor_list const& elements) const
        {
            return function(Function(elements.front()));
        }
    };

