

// $$$ move this in its own file $$$
namespace scheme { namespace utree_functions
{
    ///////////////////////////////////////////////////////////////////////////
    // Extra functions
    ///////////////////////////////////////////////////////////////////////////
    inline utree rest(utree& x)
    {
        utree::iterator i = x.begin(); ++i;
        return utree(utree::range(i, x.end()), shallow);
    }

    inline utree rest(utree const& x)
    {
        utree::const_iterator i = x.begin(); ++i;
        return utree(utree::const_range(i, x.end()), shallow);
    }
}}
