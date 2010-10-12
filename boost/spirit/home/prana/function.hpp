
    template <typename F>
    stored_function<F>::stored_function(F f)
      : f(f)
    {
    }

    template <typename F>
    stored_function<F>::~stored_function()
    {
    };

    template <typename F>
    utree stored_function<F>::operator()(scope const& env) const
    {
        return f(env);
    }

    template <typename F>
    function_base*
    stored_function<F>::clone() const
    {
        return new stored_function<F>(*this);
    }
