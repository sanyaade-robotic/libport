/**
 ** \file libport/file-library.hxx
 ** \brief implements inline function of libport/file-library.hh
 */

#ifndef LIBPORT_FILE_LIBRARY_HXX
# define LIBPORT_FILE_LIBRARY_HXX

# include <libport/foreach.hh>
# include <libport/range.hh>

namespace libport
{

  template <class ForwardRange>
  file_library::file_library(const ForwardRange& r, const char* sep)
  {
    push_cwd();
    push_back(r, sep);
  }

  template <class ForwardRange>
  void
  file_library::push_back(const ForwardRange& r, const char* sep)
  {
    bool inserted = false;
    typename boost::range_iterator<const ForwardRange>::type
      first = boost::const_begin(r);
    if (first != boost::const_end(r))
    {
      if (first->empty())
        // Insert the following search path component.
        push_back(skip_first(r), sep);
      else
        foreach (const std::string& s, split(*first, sep))
        {
          if (!s.empty())
            push_back(s);
          else if (!inserted)
          {
            // Insert the following search path component.
            push_back(skip_first(r), sep);
            inserted = true;
          }
        }
    }
  }

  inline void
  file_library::append(const path& p)
  {
    push_back(p);
  }

  inline void
  file_library::prepend(const path& p)
  {
    push_front(p);
  }

  inline const file_library::path_list_type&
  file_library::search_path_get() const
  {
    return search_path_;
  }

  inline std::ostream&
  operator<<(std::ostream& ostr, const file_library& l)
  {
    return l.dump(ostr);
  }
}

#endif // !LIBPORT_FILE_LIBRARY_HXX
