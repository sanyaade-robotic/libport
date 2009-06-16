#include <sstream>

#include <libport/assert.hh>
#include <libport/unit-test.hh>

using libport::test_suite;
using namespace libport;

void
check_assert_op()
{
  // I'd like to check the error messages, but how?
  BOOST_CHECK_NO_THROW(assert_eq(0, 0));
  BOOST_CHECK_NO_THROW(assert_ge(2, -2));
  BOOST_CHECK_NO_THROW(assert_gt(2, -2));
  BOOST_CHECK_NO_THROW(assert_le(0.1, 0.1));
  BOOST_CHECK_NO_THROW(assert_lt(0.1, 0.2));
  BOOST_CHECK_NO_THROW(assert_ne("foo", "bar"));
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("assert.hh");
  suite->add(BOOST_TEST_CASE(check_assert_op));
  return suite;
}
