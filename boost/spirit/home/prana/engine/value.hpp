    ///////////////////////////////////////////////////////////////////////////
    // values
    ///////////////////////////////////////////////////////////////////////////
    struct value_function : actor<value_function>
    {
        utree val;
        value_function(utree const& val) : val(val) {}

        utree eval(scope const& /*env*/) const
        {
            return utree(boost::ref(val));
        }
    };

    struct value
    {
        typedef function result_type;
        function operator()(utree const& val) const
        {
            return function(value_function(val));
        }
    };

    value const val = {};

    inline function protect(function const& f)
    {
        return val(f.f);
    }

