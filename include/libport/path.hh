/**
 ** \file libport/path.hh
 ** \brief path: represents an absolute path in the filesystem
 */

#ifndef LIBPORT_PATH_HH
# define LIBPORT_PATH_HH

# include <string>
# include <list>

namespace libport
{
  /** \brief Helper for handling paths
   **/
  class path
  {
  public:
    /// \name Constructors.
    /// \{

    path ();

    /// Init object with \a path.
    path (std::string p);

    // FIXME: Why does constructor with std::string is not enough?
    /// Init object with \a p.
    path (const char* p);

    /// \}

    /// \name Operations on path.
    /// \{
#ifdef SWIG
    %rename (assign) operator= (const path& rhs);
#endif
    path& operator= (const path& rhs);
    path& operator/= (const path& rhs);
    path operator/ (const std::string& rhs) const;
    bool operator== (const path& rhs) const;

    std::string basename () const;
    path dirname () const;
    bool exists () const;

    /// \}

    /// \name Printing and converting.
    /// \{
#ifdef SWIG
    %rename (__str__) operator std::string () const;
#endif
    std::string to_string () const;
    operator std::string () const;
    std::ostream& dump (std::ostream& ostr) const;

    /// \}

    bool
    absolute_get () const;

  private:
    /// path is represented with a list of directories.
    typedef std::list<std::string> path_type;

    /// Append a single directory \a dir.
    void append_dir (std::string dir);

    /// Init object with path \a p.
    void init (std::string p);

    /// Represented path.
    path_type path_;

    /// "absolute" flag.
    bool absolute_;
  };

  std::ostream&
  operator<< (std::ostream& ostr, const path& p);
}

# include "libport/path.hxx"

#endif // !LIBPORT_PATH_HH
