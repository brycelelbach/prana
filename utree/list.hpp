
    ///////////////////////////////////////////////////////////////////////////
    // Our POD double linked list. Straightforward implementation.
    // This implementation is very primitive and is not meant to be
    // used stand-alone. This is the internal data representation
    // of lists in our utree.
    ///////////////////////////////////////////////////////////////////////////
    struct list // keep this a POD!
    {
        struct node;

        template <typename Value>
        class node_iterator;

        void free();
        void copy(list const& other);
        void default_construct();

        template <typename T>
        void insert_before(T const& val, node* node);

        template <typename T>
        void insert_after(T const& val, node* node);

        template <typename T>
        void push_front(T const& val);

        template <typename T>
        void push_back(T const& val);

        void pop_front();
        void pop_back();
        node* erase(node* pos);

        node* first;
        node* last;
        std::size_t size;
    };

    struct list::node : boost::noncopyable
    {
        template <typename T>
        node(T const& val, node* next, node* prev)
          : val(val), next(next), prev(prev) {}

        void unlink()
        {
            prev->next = next;
            next->prev = prev;
        }

        utree val;
        node* next;
        node* prev;
    };

    template <typename Value>
    class list::node_iterator
      : public boost::iterator_facade<
            node_iterator<Value>
          , Value
          , boost::bidirectional_traversal_tag
        >
    {
    public:

        node_iterator()
          : node(0) {}

        node_iterator(list::node* node, list::node* prev)
          : node(node), prev(prev) {}

    private:

        friend class boost::iterator_core_access;
        friend class scheme::utree;

        void increment()
        {
            if (node != 0) // not at end
            {
                prev = node;
                node = node->next;
            }
        }

        void decrement()
        {
            if (prev != 0) // not at begin
            {
                node = prev;
                prev = prev->prev;
            }
        }

        bool equal(node_iterator const& other) const
        {
            return node == other.node;
        }

        typename node_iterator::reference dereference() const
        {
            return node->val;
        }

        list::node* node;
        list::node* prev;
    };

    template <typename Value>
    class list::node_iterator<boost::reference_wrapper<Value> >
      : public boost::iterator_facade<
            node_iterator<boost::reference_wrapper<Value> >
          , boost::reference_wrapper<Value>
          , boost::bidirectional_traversal_tag
        >
    {
    public:

        node_iterator()
          : node(0), prev(0), curr(nil_node) {}

        node_iterator(list::node* node, list::node* prev)
          : node(node), prev(prev), curr(node ? node->val : nil_node) {}

    private:

        friend class boost::iterator_core_access;
        friend class scheme::utree;

        void increment()
        {
            if (node != 0) // not at end
            {
                prev = node;
                node = node->next;
                curr = boost::ref(node ? node->val : nil_node);
            }
        }

        void decrement()
        {
            if (prev != 0) // not at begin
            {
                node = prev;
                prev = prev->prev;
                curr = boost::ref(node ? node->val : nil_node);
            }
        }

        bool equal(node_iterator const& other) const
        {
            return node == other.node;
        }

        typename node_iterator::reference dereference() const
        {
            return curr;
        }

        list::node* node;
        list::node* prev;

        static Value nil_node;
        mutable boost::reference_wrapper<Value> curr;
    };

    template <typename Value>
    Value list::node_iterator<boost::reference_wrapper<Value> >::nil_node = Value();

    inline void list::free()
    {
        node* p = first;
        while (p != last)
        {
            node* next = p->next;
            delete p;
            p = next;
        }
        first = last = 0;
        size = 0;
    }

    inline void list::copy(list const& other)
    {
        first = last = 0;
        size = 0;
        node* p = other.first;
        while (p != 0)
        {
            push_back(p->val);
            p = p->next;
        }
    }

    inline void list::default_construct()
    {
        first = last = 0;
        size = 0;
    }

    template <typename T>
    inline void list::insert_before(T const& val, node* np)
    {
        BOOST_ASSERT(np != 0);
        node* new_node = new node(val, np, np->prev);
        if (np->prev)
            np->prev->next = new_node;
        else
            first = new_node;
        np->prev = new_node;
        ++size;
    }

    template <typename T>
    inline void list::insert_after(T const& val, node* np)
    {
        BOOST_ASSERT(np != 0);
        node* new_node = new node(val, np->next, np);
        if (np->next)
            np->next->prev = new_node;
        else
            last = new_node;
        np->next = new_node;
        ++size;
    }

    template <typename T>
    inline void list::push_front(T const& val)
    {
        detail::list::node* new_node;
        if (first == 0)
        {
            new_node = new detail::list::node(val, 0, 0);
            first = last = new_node;
            ++size;
        }
        else
        {
             insert_before(val, first);
        }
    }

    template <typename T>
    inline void list::push_back(T const& val)
    {
        if (last == 0)
            push_front(val);
        else
            insert_after(val, last);
    }

    inline void list::pop_front()
    {
        BOOST_ASSERT(size != 0);
        if (first == last) // there's only one item
        {
            delete first;
            size = 0;
            first = last = 0;
        }
        else
        {
            node* np = first;
            first = first->next;
            first->prev = 0;
            delete np;
            --size;
        }
    }

    inline void list::pop_back()
    {
        BOOST_ASSERT(size != 0);
        if (first == last) // there's only one item
        {
            delete first;
            size = 0;
            first = last = 0;
        }
        else
        {
            node* np = last;
            last = last->prev;
            last->next = 0;
            delete np;
            --size;
        }
    }

    inline list::node* list::erase(node* pos)
    {
        BOOST_ASSERT(pos != 0);
        if (pos == first)
        {
            pop_front();
            return first;
        }
        else if (pos == last)
        {
            pop_back();
            return 0;
        }
        else
        {
            node* next(pos->next);
            pos->unlink();
            delete pos;
            --size;
            return next;
        }
    }
