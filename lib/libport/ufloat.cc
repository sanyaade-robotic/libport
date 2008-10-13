#include <iostream>
#include <cmath>

#include <libport/ufloat.hh>

#ifdef LIBPORT_URBI_UFLOAT_LONG_LONG
# include <libport/ull-fixed-point.cc>
#endif

#ifdef LIBPORT_URBI_UFLOAT_FLOATING
# include <libport/uffloat.cc>
#endif

#include <boost/mpl/integral_c.hpp>
#include <boost/numeric/conversion/converter.hpp>

namespace libport
{
#ifdef LIBPORT_URBI_UFLOAT_TABULATED

# ifndef SINTABLE_POWER
  // The tables will containe 2^sintable_power elements.
#  define SINTABLE_POWER 10
# endif

  void buildSinusTable(int powersize);
  class DummyInit
  {
  public:
    DummyInit()
    {
      buildSinusTable(SINTABLE_POWER);
    }
  };

  DummyInit _dummyInit__;

  static ufloat* sinTable=0; //we store [O, PI/2[
  static ufloat* asinTable=0; //we store [O, 1[

  static unsigned long tableSize; //must be a power of two
  static int tableShift;

  void buildSinusTable(int powersize)
  {
    int size = 1<<powersize;
    tableShift = powersize;
    //don't use a step-based generation or errors will accumulate
    delete [] sinTable;
    delete [] asinTable;

    sinTable = new ufloat[size];
    asinTable = new ufloat[size];

    tableSize = size;
    for (int i=0;i<size;i++)
      {
	double idx = (double)i*(M_PI/2.0)/(double)size;
	float val = ::sin(idx);
	sinTable[i]=val;

	double idx2 = (double)i/(double)size;
	asinTable[i] = ::asin(idx2);
      }
  }

#endif


#if defined LIBPORT_URBI_UFLOAT_TABULATED \
 && !defined LIBPORT_URBI_UFLOAT_FLOATING

  ufloat tabulatedSin(ufloat val)
  {
    ufloat fidx = (val*(ufloat)tableSize / (M_PI/2.0));
    int idx = (int) fidx;
    ufloat rem = fidx -(ufloat)idx;

    idx = idx & (tableSize-1);

    if (fmod(val, M_PI) >= M_PI/2)
      idx = (tableSize-idx-1); //sin(pi/2+x) = sin(pi/2-x)
    ufloat interp = sinTable(idx)*(1.0-rem)+sinTable[(idx+1)%tableSize]*rem;

    if (fmod(val, M_PI*2) > M_PI)
      return -interp;
    else
      return interp;
  };

  ufloat tabulatedCos(ufloat val)
  {
    ufloat fidx = (val*(ufloat)tableSize / (M_PI/2.0));
    int idx = (int) fidx;
    ufloat rem = fidx -(ufloat)idx;

    idx = idx & (tableSize-1);

    if (fmod(val, M_PI) < M_PI/2)
      idx = (tableSize-idx-1); //sin(pi/2+x) = sin(pi/2-x)

    ufloat interp = sinTable(idx)*(1.0-rem)+sinTable[(idx+1)%tableSize]*rem;

    if (fmod(val, M_PI*2) > M_PI)
      return -interp;
    else
      return interp;
  };


  ufloat tabulatedASin(ufloat val)
  {
    ufloat fidx = val *(ufloat)tableSize;
    int idx =(int) fidx;
    ufloat rem = fidx -(ufloat)idx;
    idx = idx & (tableSize-1);
    ufloat interp = asinTable(idx)*(1.0-rem)+asinTable[(idx+1)%tableSize]*rem;

    if (val < 0.0)
      return -interp;
    else
      return interp;
  }

#endif

/*-------------------.
| ufloat converter.  |
`-------------------*/

  template<typename S>
  struct ExactFloat2IntRounderPolicy
  {
    typedef S source_type;
    typedef S argument_type;

    static source_type nearbyint(argument_type s)
    {
      if (s != ceil(s))
	throw boost::numeric::bad_numeric_cast();
      return s;
    }

    typedef boost::mpl::integral_c<std::float_round_style,std::round_to_nearest>
      round_style ;
  };


  template <typename T>
  inline
  T
  ufloat_cast(ufloat val)
  {
    try
    {
      static boost::numeric::converter
        <T,
        ufloat,
        boost::numeric::conversion_traits<T, ufloat>,
        boost::numeric::def_overflow_handler,
        ExactFloat2IntRounderPolicy<ufloat> > converter;

      return converter(val);
    }
    catch (boost::numeric::bad_numeric_cast&)
    {
      throw bad_numeric_cast();
    }
  }

  /// Convert a libport::ufloat to a int. raise
  /// libport::bad_numeric_cast if the provided argument is directly
  /// convertible to an integer.
  int ufloat_to_int(ufloat val)
  {
    return ufloat_cast<int>(val);
  }

} // namespace libport
