struct interpreter {
  template<class Source>
  interpreter (Source& in, std::string const& source_file = "<string>",
               environment* envp = 0);

  interpreter (utree const& program, environment* envp = 0);

  function operator[] (std::string const& name);

  environment env;
  utree program;
  actor_list fragments;
  actor_list flist;
};

