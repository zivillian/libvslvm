/*
 * Library open close testing program
 *
 * Copyright (C) 2014-2016, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <common.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include <stdio.h>

#include "vslvm_test_libcerror.h"
#include "vslvm_test_libcstring.h"
#include "vslvm_test_libvslvm.h"

/* Tests single open and close of a handle
 * Returns 1 if successful, 0 if not or -1 on error
 */
int vslvm_test_single_open_close_handle(
     libcstring_system_character_t *filename,
     int access_flags,
     int expected_result )
{
	libcerror_error_t *error  = NULL;
	libvslvm_handle_t *handle = NULL;
	static char *function     = "vslvm_test_single_open_close_handle";
	int result                = 0;

	if( libvslvm_handle_initialize(
	     &handle,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create handle.",
		 function );

		return( -1 );
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	result = libvslvm_handle_open_wide(
	          handle,
	          filename,
	          access_flags,
	          &error );
#else
	result = libvslvm_handle_open(
	          handle,
	          filename,
	          access_flags,
	          &error );
#endif
	if( result == 1 )
	{
		if( libvslvm_handle_close(
		     handle,
		     &error ) != 0 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close handle.",
			 function );

			result = -1;
		}
	}
	if( libvslvm_handle_free(
	     &handle,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free handle.",
		 function );

		result = -1;
	}
	result = ( expected_result == result );

	if( result == 1 )
	{
		fprintf(
		 stdout,
		 "(PASS)" );
	}
	else
	{
		fprintf(
		 stdout,
		 "(FAIL)" );
	}
	fprintf(
	 stdout,
	 "\n" );

	if( error != NULL )
	{
		if( result != 1 )
		{
			libcerror_error_backtrace_fprint(
			 error,
			 stderr );
		}
		libcerror_error_free(
		 &error );
	}
	return( result );
}

/* Tests multiple open and close of a handle
 * Returns 1 if successful, 0 if not or -1 on error
 */
int vslvm_test_multi_open_close_handle(
     libcstring_system_character_t *filename,
     int access_flags,
     int expected_result )
{
	libcerror_error_t *error  = NULL;
	libvslvm_handle_t *handle = NULL;
	static char *function     = "vslvm_test_multi_open_close_handle";
	int result                = 0;

	if( libvslvm_handle_initialize(
	     &handle,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create handle.",
		 function );

		return( -1 );
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	result = libvslvm_handle_open_wide(
	          handle,
	          filename,
	          access_flags,
	          &error );
#else
	result = libvslvm_handle_open(
	          handle,
	          filename,
	          access_flags,
	          &error );
#endif
	if( result == 1 )
	{
		if( libvslvm_handle_close(
		     handle,
		     &error ) != 0 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close handle.",
			 function );

			result = -1;
		}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		result = libvslvm_handle_open_wide(
		          handle,
		          filename,
		          access_flags,
		          &error );
#else
		result = libvslvm_handle_open(
		          handle,
		          filename,
		          access_flags,
		          &error );
#endif
		if( result == 1 )
		{
			if( libvslvm_handle_close(
			     handle,
			     &error ) != 0 )
			{
				libcerror_error_set(
				 &error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_CLOSE_FAILED,
				 "%s: unable to close handle.",
				 function );

				result = -1;
			}
		}
	}
	if( libvslvm_handle_free(
	     &handle,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free handle.",
		 function );

		result = -1;
	}
	result = ( expected_result == result );

	if( result == 1 )
	{
		fprintf(
		 stdout,
		 "(PASS)" );
	}
	else
	{
		fprintf(
		 stdout,
		 "(FAIL)" );
	}
	fprintf(
	 stdout,
	 "\n" );

	if( error != NULL )
	{
		if( result != 1 )
		{
			libcerror_error_backtrace_fprint(
			 error,
			 stderr );
		}
		libcerror_error_free(
		 &error );
	}
	return( result );
}

/* The main program
 */
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	if( argc != 2 )
	{
		fprintf(
		 stderr,
		 "Unsupported number of arguments.\n" );

		return( EXIT_FAILURE );
	}
	/* Case 0: single open and close of a handle using filename
	 */
	fprintf(
	 stdout,
	 "Testing single open close of: %s with access: read\t",
	 argv[ 1 ] );

	if( vslvm_test_single_open_close_handle(
	     argv[ 1 ],
	     LIBVSLVM_OPEN_READ,
	     1 ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test single open close.\n" );

		return( EXIT_FAILURE );
	}
	fprintf(
	 stdout,
	 "Testing single open close of: NULL with access: read\t" );

	if( vslvm_test_single_open_close_handle(
	     NULL,
	     LIBVSLVM_OPEN_READ,
	     -1 ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test single open close.\n" );

		return( EXIT_FAILURE );
	}
	fprintf(
	 stdout,
	 "Testing single open close of: %s with access: write\t",
	 argv[ 1 ] );

	if( vslvm_test_single_open_close_handle(
	     argv[ 1 ],
	     LIBVSLVM_OPEN_WRITE,
	     -1 ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test single open close.\n" );

		return( EXIT_FAILURE );
	}
	/* Case 1: multiple open and close of a handle using filename
	 */
	fprintf(
	 stdout,
	 "Testing multi open close of: %s with access: read\t",
	 argv[ 1 ] );

	if( vslvm_test_multi_open_close_handle(
	     argv[ 1 ],
	     LIBVSLVM_OPEN_READ,
	     1 ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to test multi open close.\n" );

		return( EXIT_FAILURE );
	}
	return( EXIT_SUCCESS );
}

