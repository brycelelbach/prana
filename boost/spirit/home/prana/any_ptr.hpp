class any_ptr
{
  public:

    template <typename Ptr>
      typename boost::disable_if<
      boost::is_polymorphic<
      typename boost::remove_pointer<Ptr>::type>,
               Ptr>::type
                 get() const
                 {
                   if (*i == typeid(Ptr))
                   {
                     return static_cast<Ptr>(p);
                   }
                   throw std::bad_cast();
                 }

    template <typename T>
      any_ptr(T* p)
      : p(p), i(&typeid(T*))
      {}

    friend bool operator==(any_ptr const& a, any_ptr const& b)
    {
      return (a.p == b.p) && (*a.i == *b.i);
    }

  private:

    // constructor is private
    any_ptr(void* p, std::type_info const* i)
      : p(p), i(i) {}

    template <typename UTreeX, typename UTreeY>
      friend struct detail::visit_impl;

    friend class utree;

    void* p;
    std::type_info const* i;
};
