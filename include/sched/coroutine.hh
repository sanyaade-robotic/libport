#ifndef SCHED_COROUTINE_HH
# define SCHED_COROUTINE_HH

# include <libport/config.h>
# include <sched/export.hh>

// Define LIBPORT_SCHED_CORO_OSTHREAD to use the os-thread implementation
//  of coros.
# ifdef LIBPORT_SCHED_CORO_OSTHREAD
#  include <sched/pthread-coro.hh>
// In the pthread implementation, these functions are compiled in
// the library, otherwise they are inlined.
#  define SCHED_CORO_API SCHED_API
# else
#  include <sched/libcoroutine/Coro.h>
#  define SCHED_CORO_API
# endif


/// This package provides an interface to the \c libcoroutine. Using this
/// interface allows for various checks and instrumentations to be
/// easily added without modifying the imported \c libcoroutine.

/// Create a new coroutine with a specified stack size.
/// \param stack_size Stack size in bytes or default stack size if 0.
SCHED_CORO_API
Coro* coroutine_new(size_t stack_size = 0);

/// Free the space used by a coroutine. Cannot be called from the coroutine
/// itself.
/// \param Coro The coroutine to destroy.
SCHED_CORO_API
void coroutine_free(Coro*);

/// Start a coroutine.
/// \param self The coroutine of the calling task.
/// \param other The coroutine to start.
/// \param callback The function used to launch the coroutine. This function
///        must never return.
/// \param context An opaque data to pass to \a callback.
template<typename T>
void coroutine_start(Coro* self, Coro* other, void (*callback)(T*), T* context);

/// Switch to a coroutine
/// \param self The coroutine of the calling task.
/// \param next The coroutine to schedule.
SCHED_CORO_API
void coroutine_switch_to(Coro* self, Coro* next);

/// Check whether the stack space is sufficient or near exhaustion.
/// \param coro The coroutine to check, can either be the current one
///        or any other coroutine.
SCHED_CORO_API
bool coroutine_stack_space_almost_gone(Coro* coro);

/// Initialize the main coroutine.
/// \param coro The coroutine structure that will be used for the main
///        task. This coroutine must never be destroyed.
SCHED_CORO_API
void coroutine_initialize_main(Coro* coro);

# if !defined LIBPORT_SCHED_CORO_OSTHREAD
#  include <sched/coroutine.hxx>
# endif
#endif // SCHED_COROUTINE_HH
