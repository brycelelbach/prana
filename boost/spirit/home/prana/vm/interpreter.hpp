struct interpreter {
  template <typename Source>
  interpreter(
    Source& in,
    std::string const& source_file = "<string>",
    environment* envp = 0) {
    if (envp == 0)
      build_basic_environment(env);
    else
      env = *envp;

    if (input::parse_sexpr_list(in, program, source_file)) {
      compile_all(program, env, flist, fragments, source_file);
    }
  }

  interpreter(
    utree const& program,
    environment* envp = 0) {
    if (envp == 0)
      build_basic_environment(env);
    else
      env = *envp;

    compile_all(program, env, flist, fragments);
  }

  function operator[](std::string const& name) {
    boost::tuple<compiled_function*, int, bool> r = env.find(name);

    if (boost::get<0>(r)) {
      compiled_function* cf = boost::get<0>(r);
      int arity = boost::get<1>(r);
      bool fixed_arity = boost::get<2>(r);
      actor_list flist;

      if (arity > 0) {
        for (int i = 0; i < (arity - 1); ++i)
          flist.push_back(arg(i));

        if (fixed_arity)
          flist.push_back(arg(arity - 1));
        else
          flist.push_back(varg(arity - 1));
      }

      return (*cf)(flist);
    } else {
      std::cerr
          << " : Error! scheme: Function "
              << name
              << " not found."
              << std::endl;
      return function();
    }
  }

  environment env;
  utree program;
  actor_list fragments;
  actor_list flist;
};

