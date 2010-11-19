
    ///////////////////////////////////////////////////////////////////////////
    // function
    ///////////////////////////////////////////////////////////////////////////
    struct function : actor<function>
    {
        utree f;
        function()
          : f() {}

        function(utree const& f)
          : f(f) {}

        template <typename F>
        function(F const& f)
          : f(stored_function<F>(f))
        {
        }

        bool empty() const
        {
            return f.which() != utree_type::function_type;
        }

        utree eval(scope const& env) const
        {
            return f.eval(env);
        }
    };

