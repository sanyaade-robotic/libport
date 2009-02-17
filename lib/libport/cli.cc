#include <iostream>

#include <libport/cli.hh>
#include <libport/sysexits.hh>
#include <libport/program-name.hh>

namespace libport
{

  void
  usage_error (const std::string& opt, const std::string& err)
  {
    std::cerr
      << program_name() << ": " << opt << ": " << err
      << std::endl
      << "Try `" << program_name() << " --help' for more information."
      << std::endl
      << libport::exit (EX_USAGE);
  }

  void
  required_argument (const std::string& opt)
  {
    usage_error (opt, "requires an argument");
  }

  void
  missing_argument (const std::string& opt)
  {
    usage_error (opt, "missing argument");
  }

  void
  invalid_option (const std::string& opt)
  {
    usage_error (opt, "invalid option");
  }

  void
  invalid_option_val (const std::string& opt, const std::string& arg)
  {
    usage_error (opt, "invalid argument: " + arg);
  }

}
