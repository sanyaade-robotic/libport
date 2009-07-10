#include <libport/containers.hh>
#include <libport/unit-test.hh>

using libport::test_suite;

static void
check_operator_lt_lt()
{
  typedef std::vector<std::string> string_list;
  string_list s1, s2;

  s1 << "a" << "b";
  BOOST_CHECK_EQUAL(s1.size(), 2);
  BOOST_CHECK_EQUAL(s1[0], "a");
  BOOST_CHECK_EQUAL(s1[1], "b");

  s2 << "c" << "d";
  s1 << s2;
  BOOST_CHECK_EQUAL(s1.size(), 4);
  BOOST_CHECK_EQUAL(s1[2], "c");
  BOOST_CHECK_EQUAL(s1[3], "d");

  typedef std::set<std::string> string_set;
  string_set s3;
  s3 << s1;
  BOOST_CHECK_EQUAL(s3.size(), 4);
  s3 << s1;
  BOOST_CHECK_EQUAL(s3.size(), 4);
}

test_suite*
init_test_suite()
{
  test_suite* suite = BOOST_TEST_SUITE("libport.containers test suite");
  suite->add(BOOST_TEST_CASE(check_operator_lt_lt));
  return suite;
}
