
    template <typename T>
    struct is_iterator_range
      : boost::mpl::false_
    {};

    template <typename Iterator>
    struct is_iterator_range<boost::iterator_range<Iterator> >
      : boost::mpl::true_
    {};

    ///////////////////////////////////////////////////////////////////////////
    // A range of utree(s) using an iterator range (begin/end) of node(s)
    ///////////////////////////////////////////////////////////////////////////
    struct range
    {
        list::node* first;
        list::node* last;
    };

    ///////////////////////////////////////////////////////////////////////////
    // A range of char*s
    ///////////////////////////////////////////////////////////////////////////
    struct string_range
    {
        char const* first;
        char const* last;
    };
