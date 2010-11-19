    ///////////////////////////////////////////////////////////////////////////
    // nary_function
    ///////////////////////////////////////////////////////////////////////////
    template <typename Derived>
    struct nary_function : actor<nary_function<Derived> >
    {
        actor_list elements;
        typedef nary_function<Derived> base_type;

        nary_function(actor_list const& elements)
          : elements(elements)
        {
            BOOST_FOREACH(function const& element, elements)
            {
                BOOST_ASSERT(!element.empty());
            }
        }

        utree eval(scope const& env) const
        {
            BOOST_ASSERT(!elements.empty());
            actor_list::const_iterator i = elements.begin();
            utree result = (*i++)(env);
            boost::iterator_range<actor_list::const_iterator>
                rest(i++, elements.end());
            BOOST_FOREACH(function const& element, rest)
            {
                if (!derived().eval(result, element(env)))
                    break; // allow short-circuit evaluation
            }
            return result;
        }

        Derived const& derived() const
        {
            return *static_cast<Derived const*>(this);
        }
    };

    template <typename Function>
    struct nary_composite : composite<nary_composite<Function> >
    {
        function compose(actor_list const& elements) const
        {
            return function(Function(elements));
        }
    };
