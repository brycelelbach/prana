    template <typename Derived>
    struct composite
    {
        typedef function result_type;
        typedef composite<Derived> base_type;

        function operator()(actor_list const& elements) const
        {
            return derived().compose(elements);
        }

        template <typename A0>
        function operator()(A0 const& _0) const
        {
            actor_list elements;
            elements.push_back(as_function(_0));
            return derived().compose(elements);
        }

        template <typename A0, typename A1>
        function operator()(A0 const& _0, A1 const& _1) const
        {
            actor_list elements;
            elements.push_back(as_function(_0));
            elements.push_back(as_function(_1));
            return derived().compose(elements);
        }

        Derived const& derived() const
        {
            return *static_cast<Derived const*>(this);
        }

        template <typename T>
        static function as_function(T const& val)
        {
            return scheme::val(utree(val));
        }

        static function const& as_function(function const& f)
        {
            return f;
        }
    };
