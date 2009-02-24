#ifndef SCHED_PTHREAD_CORO_HH
# define SCHED_PTHREAD_CORO_HH

# include <libport/config.h>

// Define LIBPORT_SCHED_CORO_OSTHREAD to use the os-thread implementation
// of coros.
# ifdef LIBPORT_SCHED_CORO_OSTHREAD
#  include <libport/semaphore.hh>

class Coro
{
public:
  Coro();
  libport::Semaphore sem_;
  bool started_;
  bool die_;
  pthread_t thread_;
};

#  include <sched/pthread-coro.hxx>

# endif // LIBPORT_SCHED_CORO_OSTHREAD

#endif // !SCHED_PTHREAD_CORO_HH
