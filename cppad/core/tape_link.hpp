# ifndef CPPAD_CORE_TAPE_LINK_HPP
# define CPPAD_CORE_TAPE_LINK_HPP

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

# include <cppad/core/define.hpp>
# include <cppad/utility/thread_alloc.hpp>
# include <cppad/core/cppad_assert.hpp>

// needed before one can use CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL
# include <cppad/utility/thread_alloc.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\file tape_link.hpp
Routines that Link AD<Base> and local::ADTape<Base> Objects.

The routines that connect the AD<Base> class to the corresponding tapes
(one for each thread).
*/

/*!
Pointer to the tape identifier for this AD<Base> class and the specific thread.

\tparam Base
is the base type for this AD<Base> class.

\param thread
is the thread number. The following condition must hold
\code
(! thread_alloc::in_parallel()) || thread == thread_alloc::thread_num()
\endcode

\return
is a pointer to the tape identifier for this thread and AD<Base> class.
*/
template <class Base>
inline tape_id_t* AD<Base>::tape_id_ptr(size_t thread)
{	CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;
	static tape_id_t tape_id_table[CPPAD_MAX_NUM_THREADS];
	CPPAD_ASSERT_UNKNOWN(
		(! thread_alloc::in_parallel()) || thread == thread_alloc::thread_num()
	);
	return tape_id_table + thread;
}

/*!
Handle for the tape for this AD<Base> class and the specific thread.

\tparam Base
is the base type for this AD<Base> class.


\param thread
is the thread number; i.e.,
\code
(! thread_alloc::in_parallel()) || thread == thread_alloc::thread_num()
\endcode

\return
is a handle for the tape for this AD<Base> class and the specified thread.
*/
template <class Base>
inline local::ADTape<Base>** AD<Base>::tape_handle(size_t thread)
{	CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;
	static local::ADTape<Base>* tape_table[CPPAD_MAX_NUM_THREADS];
	CPPAD_ASSERT_UNKNOWN(
		(! thread_alloc::in_parallel()) || thread == thread_alloc::thread_num()
	);
	return tape_table + thread;
}

/*!
Pointer for the tape for this AD<Base> class and the current thread.

\code
thread == thread_alloc::thread_num()
\endcode

\tparam Base
is the base type corresponding to AD<Base> operations.

\return
is a pointer to the tape that is currently recording AD<Base> operations
for the current thread.
If this value is CPPAD_NULL, there is no tape currently
recording AD<Base> operations for this thread.
*/
template <class Base>
inline local::ADTape<Base>* AD<Base>::tape_ptr(void)
{	size_t thread = thread_alloc::thread_num();
	return *tape_handle(thread);
}

/*!
Pointer for the tape for this AD<Base> class and the specified tape
identifier.

\tparam Base
is the base type corresponding to AD<Base> operations.

\param tape_id
is the identifier for the tape that is currently recording
AD<Base> operations for the current thread.
It must hold that the current thread is
\code
	thread = size_t( tape_id % CPPAD_MAX_NUM_THREADS )
\endcode
and that there is a tape recording AD<Base> operations
for this thread.
If this is not the currently executing thread,
a variable from a different thread is being recorded on the
tape for this thread which is a user error.

\return
is a pointer to the tape that is currently recording AD<Base> operations
for the current thread (and it is not \c CPPAD_NULL).

\par Restrictions
This routine should only be called if there is a tape recording operaitons
for the specified thread.
*/
template <class Base>
inline local::ADTape<Base>* AD<Base>::tape_ptr(tape_id_t tape_id)
{	size_t thread = size_t( tape_id % CPPAD_MAX_NUM_THREADS );
	CPPAD_ASSERT_KNOWN(
		thread == thread_alloc::thread_num(),
		"Attempt to use an AD variable with two different threads."
	);
	CPPAD_ASSERT_UNKNOWN( tape_id == *tape_id_ptr(thread) );
	CPPAD_ASSERT_UNKNOWN( *tape_handle(thread) != CPPAD_NULL );
	return *tape_handle(thread);
}

/*!
Create and delete tapes that record AD<Base> operations for current thread.

\par thread
the current thread is given by
\code
thread = thread_alloc::thread_num()
\endcode

\tparam Base
is the base type corresponding to AD<Base> operations.

\param job
This argument determines if we are creating a new tape, or deleting an
old one.

- \c tape_manage_new :
Creates and a new tape.
It is assumed that there is no tape recording AD<Base> operations
for this thread when \c tape_manage is called.

- \c tape_manage_delete :
It is assumed that there is a tape recording AD<Base> operations
for this thread when \c tape_manage is called.
The value of <tt>*tape_id_ptr(thread)</tt> will be advanced by
\c CPPAD_MAX_NUM_THREADS.


\return
- <tt>job == tape_manage_new</tt>: a pointer to the new tape is returned.
- <tt>job == tape_manage_delete</tt>: the value \c CPPAD_NULL is returned.
*/
template <class Base>
local::ADTape<Base>*  AD<Base>::tape_manage(tape_manage_job job)
{	// this routine has static variables so first call cannot be in parallel
	CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL

	// Pointer to the tape for each thread
	static local::ADTape<Base>* tape_table[CPPAD_MAX_NUM_THREADS];

	// Thread corresponding to this call
	size_t thread        = thread_alloc::thread_num();

	// tape_manage_clear
	if( job == tape_manage_clear )
	{	// This operation cannot be done in parallel
		CPPAD_ASSERT_UNKNOWN(thread == 0 && (! thread_alloc::in_parallel()));
		for(thread = 0; thread < CPPAD_MAX_NUM_THREADS; thread++)
		{	// if this thread has a tape
			if( tape_table[thread] != CPPAD_NULL )
			{
				delete tape_table[thread];
				tape_table[thread]   = CPPAD_NULL;
			}
		}
		return CPPAD_NULL;
	}

	// id and tape for this thread
	tape_id_t*            tape_id_p  = tape_id_ptr(thread);
	local::ADTape<Base>** tape_h     = tape_handle(thread);

	// check if there is no tape currently attached to this thread
	if( tape_table[thread] == CPPAD_NULL )
	{	// allocate separate memroy to avoid false sharing
		tape_table[thread] = new local::ADTape<Base>();

		// if tape id is zero, initialize it so that
		// thread == tape id % CPPAD_MAX_NUM_THREADS
		if( *tape_id_p == 0 )
		{	size_t new_tape_id = thread + CPPAD_MAX_NUM_THREADS;
			CPPAD_ASSERT_KNOWN(
				size_t( std::numeric_limits<tape_id_t>::max() ) >= new_tape_id,
				"cppad_tape_id_type maximum value has been execeeded"
			);
			*tape_id_p = static_cast<tape_id_t>( new_tape_id );
		}
	}

	// make sure tape_id value is valid for this thread
	CPPAD_ASSERT_UNKNOWN(
		size_t( *tape_id_p % CPPAD_MAX_NUM_THREADS ) == thread
	);

	switch(job)
	{	case tape_manage_new:
		// tape for this thread must be null at the start
		CPPAD_ASSERT_UNKNOWN( *tape_h  == CPPAD_NULL );
		// current id and pointer to this tape
		tape_table[thread]->id_ = *tape_id_p;
		//
		*tape_h = tape_table[thread];
		break;

		case tape_manage_delete:
		CPPAD_ASSERT_UNKNOWN( *tape_h  == tape_table[thread] );
		CPPAD_ASSERT_KNOWN(
			std::numeric_limits<CPPAD_TAPE_ID_TYPE>::max()
			- CPPAD_MAX_NUM_THREADS > *tape_id_p,
			"To many different tapes given the type used for "
			"CPPAD_TAPE_ID_TYPE"
		);
		// advance tape identfier so all AD<Base> variables become parameters
		*tape_id_p  += CPPAD_MAX_NUM_THREADS;
		// free memory corresponding to recording in the old tape
		tape_table[thread]->Rec_.free();
		// inform rest of CppAD that no tape recording for this thread
		*tape_h = CPPAD_NULL;
		break;

		case tape_manage_clear:
		CPPAD_ASSERT_UNKNOWN(false);
	}
	return *tape_h;
}

/*!
Get a pointer to tape that records AD<Base> operations for the current thread.

\tparam Base
is the base type corresponding to AD<Base> operations.

\par thread
The current thread must be given by
\code
	thread = this->tape_id_ % CPPAD_MAX_NUM_THREADS
\endcode

\return
is a pointer to the tape that is currently recording AD<Base> operations
for the current thread.
This value must not be \c CPPAD_NULL; i.e., there must be a tape currently
recording AD<Base> operations for this thread.
*/

template <class Base>
inline local::ADTape<Base> *AD<Base>::tape_this(void) const
{
	size_t thread = size_t( tape_id_ % CPPAD_MAX_NUM_THREADS );
	CPPAD_ASSERT_UNKNOWN( tape_id_ == *tape_id_ptr(thread) );
	CPPAD_ASSERT_UNKNOWN( *tape_handle(thread) != CPPAD_NULL );
	return *tape_handle(thread);
}

} // END_CPPAD_NAMESPACE
# endif
