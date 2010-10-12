
    struct index_impl
    {
        static utree& apply(list::node* node, std::size_t i)
        {
            for (; i > 0; --i)
                node = node->next;
            return node->val;
        }

        static utree const& apply(list::node const* node, std::size_t i)
        {
            for (; i > 0; --i)
                node = node->next;
            return node->val;
        }
    };
