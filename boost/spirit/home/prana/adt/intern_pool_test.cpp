#include <boost/spirit/home/prana/adt/intern_pool.hpp>

#include <boost/assert.hpp>

using namespace boost::spirit::prana;

int main (void) {
  typedef intern_pool<std::string::iterator> pool;

  std::string foo("foobar");
  
  pool p;

  p.insert(foo.begin(), foo.end()); 

  std::string::iterator it = foo.begin();

  BOOST_ASSERT(p.find(it, foo.end()));
  BOOST_ASSERT(it == foo.end());

  p.erase(foo.begin(), foo.end());

  p.insert(foo.begin(), foo.begin() + 2);

  it = foo.begin();

  pool::pointer r = 0;
  BOOST_ASSERT((r = p.find(it, foo.end())));
 
  BOOST_ASSERT(r->begin() == foo.begin());
  BOOST_ASSERT(r->end() == it);
 
  BOOST_ASSERT(it == (foo.begin() + 2));
}
