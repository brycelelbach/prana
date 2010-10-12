
    ///////////////////////////////////////////////////////////////////////////
    // The scope
    ///////////////////////////////////////////////////////////////////////////
    class scope : public boost::iterator_range<utree*>
    {
    public:

        scope(utree* first = 0,
            utree* last = 0,
            scope const* parent = 0)
          : boost::iterator_range<utree*>(first, last),
            parent(parent),
            depth(parent? parent->depth + 1 : 0)
        {}

        scope const* outer() const { return parent; }
        int level() const { return depth; }

    private:

        scope const* parent;
        int depth;
    };
