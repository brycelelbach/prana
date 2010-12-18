struct compiler {
  environment& env;
  actor_list& fragments;
  int line;
  std::string source_file;

  typedef function result_type;

  compiler (environment& env, actor_list& fragments,
            std::size_t line = 1, std::string const& source_file = "");

  function operator() (nil) const;
  function operator() (utf8_symbol_range const&) const;
  function operator() (utree::const_range const&) const;
  function operator() (function_base const&) const;
  template<class Value>
    function operator() (Value const&) const;

  function define_function (std::string const& name,
                            std::vector<std::string>& args,
                            bool fixed_arity, utree const& body) const;
};

inline bool is_definition (utree const&);

inline std::string get_symbol (utree const&);

inline bool compile_function (utree const& ast, environment& env,
                              function& result, actor_list& fragments,
                              std::size_t parent_line = 1,
                              std::string const& source_file = "<string>")

inline bool compile_program (utree const& ast, environment& env,
                             actor_list& result, actor_list& fragments,
                             std::string const& source_file = "<string>");

