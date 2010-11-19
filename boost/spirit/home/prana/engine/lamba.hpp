    ///////////////////////////////////////////////////////////////////////////
    // lambda
    ///////////////////////////////////////////////////////////////////////////
    struct lambda_function : actor<lambda_function>
    {
        int level;
        actor_list elements;
        // we must hold f by reference because functions can be recursive
        boost::reference_wrapper<function const> f;

        lambda_function(function const& f, actor_list const& elements, int level = 0)
          : elements(elements), f(f), level(level) {}

        typedef utree result_type;
        utree eval(scope const& env) const
        {
            // Get the parent scope
            scope const* outer = &env;
            while (level != outer->level())
                outer = outer->outer();

            if (!elements.empty())
            {
                boost::scoped_array<utree>
                    fargs(new utree[elements.size()]);
                std::size_t i = 0;
                BOOST_FOREACH(function const& element, elements)
                {
                    fargs[i++] = element(env);
                }
                utree* fi = fargs.get();
                return f.get()(scope(fi, fi+elements.size(), outer));
            }
            else
            {
                return f.get()(scope(0, 0, outer));
            }
        }
    };

    struct lambda : composite<lambda>
    {
        function f;

        lambda() : f() {}
        lambda(function const& f) : f(f) {}

        function compose(actor_list const& elements) const
        {
            return function(lambda_function(f, elements));
        }

        lambda& operator=(lambda const& other)
        {
            f = other.f;
            return *this;
        }

        lambda& operator=(function const& f_)
        {
            f = f_;
            return *this;
        }
    };
