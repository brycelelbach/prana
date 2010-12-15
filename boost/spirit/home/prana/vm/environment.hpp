class environment {
  public:

    environment(environment* parent = 0)
      : outer(parent),
        depth(parent ? parent->depth + 1 : 0)
    {}

    template <typename Function>
    void define(std::string const& name, Function const& f, int arity, bool fixed) {
      if (definitions.find(name) != definitions.end())
        throw duplicate_identifier(name);

      definitions[name] = boost::make_tuple(compiled_function(f), arity, fixed);
    }

    boost::tuple<compiled_function*, int, bool>
    find(std::string const& name) {
      std::map<std::string, map_element>::iterator
      i = definitions.find(name);

      if (i != definitions.end())
        return boost::make_tuple(
            &boost::get<0>(i->second),
            boost::get<1>(i->second),
            boost::get<2>(i->second)
            );
      else if (outer != 0)
        return outer->find(name);

      return boost::make_tuple((compiled_function*)0, 0, false);
    }

    void undefine(std::string const& name) {
      definitions.erase(name);
    }

    bool defined(std::string const& name) {
      return definitions.find(name) != definitions.end();
    }

    void forward_declare(std::string const& name, function* f) {
      forwards[name] = f;
    }

    function* find_forward(std::string const& name) {
      std::map<std::string, function*>::iterator
      iter = forwards.find(name);

      if (iter == forwards.end())
        return 0;
      else
        return iter->second;
    }

    environment* parent() const {
      return outer;
    }
    int level() const {
      return depth;
    }

  private:

    typedef boost::tuple<compiled_function, int, bool> map_element;

    environment* outer;
    std::map<std::string, map_element> definitions;
    std::map<std::string, function*> forwards;
    int depth;
};


void build_basic_environment(environment& env) {
  env.define("if", if_, 3, true);
  env.define("begin", block, 1, false);
  env.define("list", list, 1, false);
  env.define("display", display, 1, true);
  env.define("front", front, 1, true);
  env.define("back", back, 1, true);
  env.define("rest", rest, 1, true);
  env.define("=", equal, 2, true);
  env.define("<", less_than, 2, true);
  env.define("<=", less_than_equal, 2, true);
  env.define("+", plus, 2, false);
  env.define("-", minus, 2, false);
  env.define("*", times, 2, false);
  env.define("/", divide, 2, false);
}

