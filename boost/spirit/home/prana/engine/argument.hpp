    ///////////////////////////////////////////////////////////////////////////
    // arguments
    ///////////////////////////////////////////////////////////////////////////
    template <bool scoped = true>
    struct argument_function : actor<argument_function<scoped> >
    {
        std::size_t n;
        std::size_t level;
        argument_function(std::size_t n, std::size_t level = 0)
          : n(n),
            level(level)
        {}

        utree eval(scope const& env) const
        {
            scope const* eptr = &env;
            while (level != eptr->level())
                eptr = eptr->outer();

            utree const& arg = (*eptr)[n];
            if (arg.which() != utree_type::function_type)
                return utree(boost::ref(arg));
            else
                return arg.eval(*eptr);
        }
    };

    template <> // scoped = false
    struct argument_function<false> : actor<argument_function<false> >
    {
        std::size_t n;
        argument_function(std::size_t n, std::size_t level = 0)
          : n(n)
        {}

        utree eval(scope const& env) const
        {
            scope const* eptr = &env;
            utree const& arg = (*eptr)[n];
            if (arg.which() != utree_type::function_type)
                return utree(boost::ref(arg));
            else
                return arg.eval(*eptr);
        }
    };

    template <bool scoped = true>
    struct argument
    {
        typedef function result_type;
        function operator()(std::size_t n, std::size_t level = 0) const
        {
            return function(argument_function<scoped>(n, level));
        }
    };

    // scoped arg
    argument<true> const arg = {};

    // unscoped arg
    argument<false> const unscoped_arg = {};

    // unscoped args
    function const _1 = unscoped_arg(0);
    function const _2 = unscoped_arg(1);
    function const _3 = unscoped_arg(2);
    function const _4 = unscoped_arg(3);
    function const _5 = unscoped_arg(4);
    function const _6 = unscoped_arg(5);
    function const _7 = unscoped_arg(6);
    function const _8 = unscoped_arg(7);
    function const _9 = unscoped_arg(8);
    function const _10 = unscoped_arg(10);

