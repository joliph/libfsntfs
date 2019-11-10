/*
 * Library attribute_list_entry type test program
 *
 * Copyright (C) 2010-2019, Joachim Metz <joachim.metz@gmail.com>
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
#include <byte_stream.h>
#include <file_stream.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "fsntfs_test_libcdata.h"
#include "fsntfs_test_libcerror.h"
#include "fsntfs_test_libfsntfs.h"
#include "fsntfs_test_macros.h"
#include "fsntfs_test_memory.h"
#include "fsntfs_test_unused.h"

#include "../libfsntfs/libfsntfs_attribute_list_entry.h"

uint8_t fsntfs_test_attribute_list_entry_data1[ 40 ] = {
	0x80, 0x00, 0x00, 0x00, 0x28, 0x00, 0x04, 0x1a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xc8, 0x08, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x24, 0x00, 0x53, 0x00, 0x44, 0x00,
	0x53, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

#if defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT )

/* Tests the libfsntfs_attribute_list_entry_initialize function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_attribute_list_entry_initialize(
     void )
{
	libcerror_error_t *error                               = NULL;
	libfsntfs_attribute_list_entry_t *attribute_list_entry = NULL;
	int result                                             = 0;

#if defined( HAVE_FSNTFS_TEST_MEMORY )
	int number_of_malloc_fail_tests                        = 1;
	int number_of_memset_fail_tests                        = 1;
	int test_number                                        = 0;
#endif

	/* Test regular cases
	 */
	result = libfsntfs_attribute_list_entry_initialize(
	          &attribute_list_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "attribute_list_entry",
	 attribute_list_entry );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_attribute_list_entry_free(
	          &attribute_list_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "attribute_list_entry",
	 attribute_list_entry );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_attribute_list_entry_initialize(
	          NULL,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	attribute_list_entry = (libfsntfs_attribute_list_entry_t *) 0x12345678UL;

	result = libfsntfs_attribute_list_entry_initialize(
	          &attribute_list_entry,
	          &error );

	attribute_list_entry = NULL;

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

#if defined( HAVE_FSNTFS_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libfsntfs_attribute_list_entry_initialize with malloc failing
		 */
		fsntfs_test_malloc_attempts_before_fail = test_number;

		result = libfsntfs_attribute_list_entry_initialize(
		          &attribute_list_entry,
		          &error );

		if( fsntfs_test_malloc_attempts_before_fail != -1 )
		{
			fsntfs_test_malloc_attempts_before_fail = -1;

			if( attribute_list_entry != NULL )
			{
				libfsntfs_attribute_list_entry_free(
				 &attribute_list_entry,
				 NULL );
			}
		}
		else
		{
			FSNTFS_TEST_ASSERT_EQUAL_INT(
			 "result",
			 result,
			 -1 );

			FSNTFS_TEST_ASSERT_IS_NULL(
			 "attribute_list_entry",
			 attribute_list_entry );

			FSNTFS_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
	for( test_number = 0;
	     test_number < number_of_memset_fail_tests;
	     test_number++ )
	{
		/* Test libfsntfs_attribute_list_entry_initialize with memset failing
		 */
		fsntfs_test_memset_attempts_before_fail = test_number;

		result = libfsntfs_attribute_list_entry_initialize(
		          &attribute_list_entry,
		          &error );

		if( fsntfs_test_memset_attempts_before_fail != -1 )
		{
			fsntfs_test_memset_attempts_before_fail = -1;

			if( attribute_list_entry != NULL )
			{
				libfsntfs_attribute_list_entry_free(
				 &attribute_list_entry,
				 NULL );
			}
		}
		else
		{
			FSNTFS_TEST_ASSERT_EQUAL_INT(
			 "result",
			 result,
			 -1 );

			FSNTFS_TEST_ASSERT_IS_NULL(
			 "attribute_list_entry",
			 attribute_list_entry );

			FSNTFS_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
#endif /* defined( HAVE_FSNTFS_TEST_MEMORY ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( attribute_list_entry != NULL )
	{
		libfsntfs_attribute_list_entry_free(
		 &attribute_list_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsntfs_attribute_list_entry_free function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_attribute_list_entry_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfsntfs_attribute_list_entry_free(
	          NULL,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libfsntfs_attribute_list_entry_read_data function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_attribute_list_entry_read_data(
     void )
{
	libcerror_error_t *error                               = NULL;
	libfsntfs_attribute_list_entry_t *attribute_list_entry = NULL;
	int result                                             = 0;

	/* Initialize test
	 */
	result = libfsntfs_attribute_list_entry_initialize(
	          &attribute_list_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "attribute_list_entry",
	 attribute_list_entry );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfsntfs_attribute_list_entry_read_data(
	          attribute_list_entry,
	          fsntfs_test_attribute_list_entry_data1,
	          40,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_attribute_list_entry_read_data(
	          attribute_list_entry,
	          fsntfs_test_attribute_list_entry_data1,
	          40,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Clean up
	 */
	result = libfsntfs_attribute_list_entry_free(
	          &attribute_list_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "attribute_list_entry",
	 attribute_list_entry );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize test
	 */
	result = libfsntfs_attribute_list_entry_initialize(
	          &attribute_list_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "attribute_list_entry",
	 attribute_list_entry );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_attribute_list_entry_read_data(
	          NULL,
	          fsntfs_test_attribute_list_entry_data1,
	          40,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	attribute_list_entry->name = (uint8_t *) 0x12345678UL;

	result = libfsntfs_attribute_list_entry_read_data(
	          attribute_list_entry,
	          fsntfs_test_attribute_list_entry_data1,
	          40,
	          &error );

	attribute_list_entry->name = NULL;

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsntfs_attribute_list_entry_read_data(
	          attribute_list_entry,
	          NULL,
	          40,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsntfs_attribute_list_entry_read_data(
	          attribute_list_entry,
	          fsntfs_test_attribute_list_entry_data1,
	          (size_t) SSIZE_MAX + 1,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsntfs_attribute_list_entry_read_data(
	          attribute_list_entry,
	          fsntfs_test_attribute_list_entry_data1,
	          0,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

#if defined( HAVE_FSNTFS_TEST_MEMORY )

	/* Test libfsntfs_attribute_list_entry_read_data with malloc failing
	 */
	fsntfs_test_malloc_attempts_before_fail = 0;

	result = libfsntfs_attribute_list_entry_read_data(
	          attribute_list_entry,
	          fsntfs_test_attribute_list_entry_data1,
	          40,
	          &error );

	if( fsntfs_test_malloc_attempts_before_fail != -1 )
	{
		fsntfs_test_malloc_attempts_before_fail = -1;
	}
	else
	{
		FSNTFS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		FSNTFS_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#if defined( OPTIMIZATION_DISABLED )

	/* Test libfsntfs_attribute_list_entry_read_data with memcpy failing
	 */
	fsntfs_test_memcpy_attempts_before_fail = 0;

	result = libfsntfs_attribute_list_entry_read_data(
	          attribute_list_entry,
	          fsntfs_test_attribute_list_entry_data1,
	          40,
	          &error );

	if( fsntfs_test_memcpy_attempts_before_fail != -1 )
	{
		fsntfs_test_memcpy_attempts_before_fail = -1;
	}
	else
	{
		FSNTFS_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		FSNTFS_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( OPTIMIZATION_DISABLED ) */
#endif /* defined( HAVE_FSNTFS_TEST_MEMORY ) */

	/* Test error case where size value is invalid
	 */
	byte_stream_copy_from_uint16_little_endian(
	 &( fsntfs_test_attribute_list_entry_data1[ 4 ] ),
	 0xffff );

	result = libfsntfs_attribute_list_entry_read_data(
	          attribute_list_entry,
	          fsntfs_test_attribute_list_entry_data1,
	          40,
	          &error );

	byte_stream_copy_from_uint16_little_endian(
	 &( fsntfs_test_attribute_list_entry_data1[ 4 ] ),
	 0x0028 );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Test error case where name offset value is invalid
	 */
	fsntfs_test_attribute_list_entry_data1[ 7 ] = 0xff;

	result = libfsntfs_attribute_list_entry_read_data(
	          attribute_list_entry,
	          fsntfs_test_attribute_list_entry_data1,
	          40,
	          &error );

	fsntfs_test_attribute_list_entry_data1[ 7 ] = 0x1a;

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Test error case where name size value is invalid
	 */
	fsntfs_test_attribute_list_entry_data1[ 6 ] = 0xff;

	result = libfsntfs_attribute_list_entry_read_data(
	          attribute_list_entry,
	          fsntfs_test_attribute_list_entry_data1,
	          40,
	          &error );

	fsntfs_test_attribute_list_entry_data1[ 6 ] = 0x04;

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Clean up
	 */
	result = libfsntfs_attribute_list_entry_free(
	          &attribute_list_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "attribute_list_entry",
	 attribute_list_entry );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( attribute_list_entry != NULL )
	{
		libfsntfs_attribute_list_entry_free(
		 &attribute_list_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsntfs_attribute_list_entry_compare_by_file_reference function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_attribute_list_entry_compare_by_file_reference(
     void )
{
	libcerror_error_t *error                                      = NULL;
	libfsntfs_attribute_list_entry_t *first_attribute_list_entry  = NULL;
	libfsntfs_attribute_list_entry_t *second_attribute_list_entry = NULL;
	int result                                                    = 0;

	/* Initialize test
	 */
	result = libfsntfs_attribute_list_entry_initialize(
	          &first_attribute_list_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "first_attribute_list_entry",
	 first_attribute_list_entry );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_attribute_list_entry_initialize(
	          &second_attribute_list_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "second_attribute_list_entry",
	 second_attribute_list_entry );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	first_attribute_list_entry->file_reference  = 1;
	second_attribute_list_entry->file_reference = 2;

	result = libfsntfs_attribute_list_entry_compare_by_file_reference(
	          first_attribute_list_entry,
	          second_attribute_list_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBCDATA_COMPARE_LESS );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	first_attribute_list_entry->file_reference  = 1;
	second_attribute_list_entry->file_reference = 1;

	result = libfsntfs_attribute_list_entry_compare_by_file_reference(
	          first_attribute_list_entry,
	          second_attribute_list_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBCDATA_COMPARE_EQUAL );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	first_attribute_list_entry->file_reference  = 2;
	second_attribute_list_entry->file_reference = 1;

	result = libfsntfs_attribute_list_entry_compare_by_file_reference(
	          first_attribute_list_entry,
	          second_attribute_list_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBCDATA_COMPARE_GREATER );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_attribute_list_entry_compare_by_file_reference(
	          NULL,
	          second_attribute_list_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsntfs_attribute_list_entry_compare_by_file_reference(
	          first_attribute_list_entry,
	          NULL,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Clean up
	 */
	result = libfsntfs_attribute_list_entry_free(
	          &second_attribute_list_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "second_attribute_list_entry",
	 second_attribute_list_entry );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_attribute_list_entry_free(
	          &first_attribute_list_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "first_attribute_list_entry",
	 first_attribute_list_entry );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( second_attribute_list_entry != NULL )
	{
		libfsntfs_attribute_list_entry_free(
		 &second_attribute_list_entry,
		 NULL );
	}
	if( first_attribute_list_entry != NULL )
	{
		libfsntfs_attribute_list_entry_free(
		 &first_attribute_list_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfsntfs_attribute_list_entry_get_type function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_attribute_list_entry_get_type(
     libfsntfs_attribute_list_entry_t *attribute_list_entry )
{
	libcerror_error_t *error = NULL;
	uint32_t type            = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsntfs_attribute_list_entry_get_type(
	          attribute_list_entry,
	          &type,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_EQUAL_UINT32(
	 "type",
	 type,
	 (uint32_t) 0x00000080UL );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_attribute_list_entry_get_type(
	          NULL,
	          &type,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsntfs_attribute_list_entry_get_type(
	          attribute_list_entry,
	          NULL,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libfsntfs_attribute_list_entry_get_file_reference function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_attribute_list_entry_get_file_reference(
     libfsntfs_attribute_list_entry_t *attribute_list_entry )
{
	libcerror_error_t *error = NULL;
	uint64_t mft_entry_index = 0;
	uint16_t sequence_number = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsntfs_attribute_list_entry_get_file_reference(
	          attribute_list_entry,
	          &mft_entry_index,
	          &sequence_number,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_attribute_list_entry_get_file_reference(
	          NULL,
	          &mft_entry_index,
	          &sequence_number,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsntfs_attribute_list_entry_get_file_reference(
	          attribute_list_entry,
	          NULL,
	          &sequence_number,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsntfs_attribute_list_entry_get_file_reference(
	          attribute_list_entry,
	          &mft_entry_index,
	          NULL,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libfsntfs_attribute_list_entry_get_utf8_name_size function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_attribute_list_entry_get_utf8_name_size(
     libfsntfs_attribute_list_entry_t *attribute_list_entry )
{
	uint8_t *name            = NULL;
	libcerror_error_t *error = NULL;
	size_t utf8_name_size    = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsntfs_attribute_list_entry_get_utf8_name_size(
	          attribute_list_entry,
	          &utf8_name_size,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_EQUAL_SIZE(
	 "utf8_name_size",
	 utf8_name_size,
	 (size_t) 5 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	name = attribute_list_entry->name;

	attribute_list_entry->name = NULL;

	result = libfsntfs_attribute_list_entry_get_utf8_name_size(
	          attribute_list_entry,
	          &utf8_name_size,
	          &error );

	attribute_list_entry->name = name;

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_EQUAL_SIZE(
	 "utf8_name_size",
	 utf8_name_size,
	 (size_t) 0 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_attribute_list_entry_get_utf8_name_size(
	          NULL,
	          &utf8_name_size,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsntfs_attribute_list_entry_get_utf8_name_size(
	          attribute_list_entry,
	          NULL,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	name = attribute_list_entry->name;

	attribute_list_entry->name = NULL;

	result = libfsntfs_attribute_list_entry_get_utf8_name_size(
	          attribute_list_entry,
	          NULL,
	          &error );

	attribute_list_entry->name = name;

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libfsntfs_attribute_list_entry_get_utf8_name function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_attribute_list_entry_get_utf8_name(
     libfsntfs_attribute_list_entry_t *attribute_list_entry )
{
	uint8_t utf8_name[ 16 ];

	uint8_t *name            = NULL;
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsntfs_attribute_list_entry_get_utf8_name(
	          attribute_list_entry,
	          utf8_name,
	          16,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_attribute_list_entry_get_utf8_name(
	          NULL,
	          utf8_name,
	          16,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	name = attribute_list_entry->name;

	attribute_list_entry->name = NULL;

	result = libfsntfs_attribute_list_entry_get_utf8_name(
	          attribute_list_entry,
	          utf8_name,
	          16,
	          &error );

	attribute_list_entry->name = name;

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsntfs_attribute_list_entry_get_utf8_name(
	          attribute_list_entry,
	          NULL,
	          16,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsntfs_attribute_list_entry_get_utf8_name(
	          attribute_list_entry,
	          utf8_name,
	          0,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsntfs_attribute_list_entry_get_utf8_name(
	          attribute_list_entry,
	          utf8_name,
	          (size_t) SSIZE_MAX + 1,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libfsntfs_attribute_list_entry_get_utf16_name_size function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_attribute_list_entry_get_utf16_name_size(
     libfsntfs_attribute_list_entry_t *attribute_list_entry )
{
	uint8_t *name            = NULL;
	libcerror_error_t *error = NULL;
	size_t utf16_name_size   = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsntfs_attribute_list_entry_get_utf16_name_size(
	          attribute_list_entry,
	          &utf16_name_size,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_EQUAL_SIZE(
	 "utf16_name_size",
	 utf16_name_size,
	 (size_t) 5 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	name = attribute_list_entry->name;

	attribute_list_entry->name = NULL;

	result = libfsntfs_attribute_list_entry_get_utf16_name_size(
	          attribute_list_entry,
	          &utf16_name_size,
	          &error );

	attribute_list_entry->name = name;

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_EQUAL_SIZE(
	 "utf16_name_size",
	 utf16_name_size,
	 (size_t) 0 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_attribute_list_entry_get_utf16_name_size(
	          NULL,
	          &utf16_name_size,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsntfs_attribute_list_entry_get_utf16_name_size(
	          attribute_list_entry,
	          NULL,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	name = attribute_list_entry->name;

	attribute_list_entry->name = NULL;

	result = libfsntfs_attribute_list_entry_get_utf16_name_size(
	          attribute_list_entry,
	          NULL,
	          &error );

	attribute_list_entry->name = name;

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libfsntfs_attribute_list_entry_get_utf16_name function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_attribute_list_entry_get_utf16_name(
     libfsntfs_attribute_list_entry_t *attribute_list_entry )
{
	uint16_t utf16_name[ 16 ];

	uint8_t *name            = NULL;
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsntfs_attribute_list_entry_get_utf16_name(
	          attribute_list_entry,
	          utf16_name,
	          16,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_attribute_list_entry_get_utf16_name(
	          NULL,
	          utf16_name,
	          16,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	name = attribute_list_entry->name;

	attribute_list_entry->name = NULL;

	result = libfsntfs_attribute_list_entry_get_utf16_name(
	          attribute_list_entry,
	          utf16_name,
	          16,
	          &error );

	attribute_list_entry->name = name;

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsntfs_attribute_list_entry_get_utf16_name(
	          attribute_list_entry,
	          NULL,
	          16,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsntfs_attribute_list_entry_get_utf16_name(
	          attribute_list_entry,
	          utf16_name,
	          0,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsntfs_attribute_list_entry_get_utf16_name(
	          attribute_list_entry,
	          utf16_name,
	          (size_t) SSIZE_MAX + 1,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libfsntfs_attribute_list_entry_compare_name_with_utf8_string function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_attribute_list_entry_compare_name_with_utf8_string(
     libfsntfs_attribute_list_entry_t *attribute_list_entry )
{
	uint8_t utf8_name1[ 5 ]  = { '$', 'S', 'D', 'S', 0 };
	uint8_t utf8_name2[ 6 ]  = { 'b', 'o', 'g', 'u', 's', 0 };
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfsntfs_attribute_list_entry_compare_name_with_utf8_string(
	          attribute_list_entry,
	          utf8_name1,
	          4,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_attribute_list_entry_compare_name_with_utf8_string(
	          attribute_list_entry,
	          utf8_name2,
	          5,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_attribute_list_entry_compare_name_with_utf8_string(
	          NULL,
	          utf8_name1,
	          4,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsntfs_attribute_list_entry_compare_name_with_utf8_string(
	          attribute_list_entry,
	          NULL,
	          4,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsntfs_attribute_list_entry_compare_name_with_utf8_string(
	          attribute_list_entry,
	          utf8_name1,
	          (size_t) SSIZE_MAX + 1,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libfsntfs_attribute_list_entry_compare_name_with_utf16_string function
 * Returns 1 if successful or 0 if not
 */
int fsntfs_test_attribute_list_entry_compare_name_with_utf16_string(
     libfsntfs_attribute_list_entry_t *attribute_list_entry )
{
	uint16_t utf16_name1[ 5 ] = { '$', 'S', 'D', 'S', 0 };
	uint16_t utf16_name2[ 6 ] = { 'b', 'o', 'g', 'u', 's', 0 };
	libcerror_error_t *error  = NULL;
	int result                = 0;

	/* Test regular cases
	 */
	result = libfsntfs_attribute_list_entry_compare_name_with_utf16_string(
	          attribute_list_entry,
	          utf16_name1,
	          4,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_attribute_list_entry_compare_name_with_utf16_string(
	          attribute_list_entry,
	          utf16_name2,
	          5,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfsntfs_attribute_list_entry_compare_name_with_utf16_string(
	          NULL,
	          utf16_name1,
	          4,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsntfs_attribute_list_entry_compare_name_with_utf16_string(
	          attribute_list_entry,
	          NULL,
	          4,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfsntfs_attribute_list_entry_compare_name_with_utf16_string(
	          attribute_list_entry,
	          utf16_name1,
	          (size_t) SSIZE_MAX + 1,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

#endif /* defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT ) */

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain(
     int argc FSNTFS_TEST_ATTRIBUTE_UNUSED,
     wchar_t * const argv[] FSNTFS_TEST_ATTRIBUTE_UNUSED )
#else
int main(
     int argc FSNTFS_TEST_ATTRIBUTE_UNUSED,
     char * const argv[] FSNTFS_TEST_ATTRIBUTE_UNUSED )
#endif
{
#if defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT )

	libcerror_error_t *error                               = NULL;
	libfsntfs_attribute_list_entry_t *attribute_list_entry = NULL;
	int result                                             = 0;

#endif /* defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT ) */

	FSNTFS_TEST_UNREFERENCED_PARAMETER( argc )
	FSNTFS_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT )

	FSNTFS_TEST_RUN(
	 "libfsntfs_attribute_list_entry_initialize",
	 fsntfs_test_attribute_list_entry_initialize );

	FSNTFS_TEST_RUN(
	 "libfsntfs_attribute_list_entry_free",
	 fsntfs_test_attribute_list_entry_free );

	FSNTFS_TEST_RUN(
	 "libfsntfs_attribute_list_entry_read_data",
	 fsntfs_test_attribute_list_entry_read_data );

	FSNTFS_TEST_RUN(
	 "libfsntfs_attribute_list_entry_compare_by_file_reference",
	 fsntfs_test_attribute_list_entry_compare_by_file_reference );

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )

	/* Initialize test
	 */
	result = libfsntfs_attribute_list_entry_initialize(
	          &attribute_list_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NOT_NULL(
	 "attribute_list_entry",
	 attribute_list_entry );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfsntfs_attribute_list_entry_read_data(
	          attribute_list_entry,
	          fsntfs_test_attribute_list_entry_data1,
	          40,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Run tests
	 */
	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_attribute_list_entry_get_type",
	 fsntfs_test_attribute_list_entry_get_type,
	 attribute_list_entry );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_attribute_list_entry_get_file_reference",
	 fsntfs_test_attribute_list_entry_get_file_reference,
	 attribute_list_entry );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_attribute_list_entry_get_utf8_name_size",
	 fsntfs_test_attribute_list_entry_get_utf8_name_size,
	 attribute_list_entry );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_attribute_list_entry_get_utf8_name",
	 fsntfs_test_attribute_list_entry_get_utf8_name,
	 attribute_list_entry );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_attribute_list_entry_get_utf16_name_size",
	 fsntfs_test_attribute_list_entry_get_utf16_name_size,
	 attribute_list_entry );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_attribute_list_entry_get_utf16_name",
	 fsntfs_test_attribute_list_entry_get_utf16_name,
	 attribute_list_entry );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_attribute_list_entry_compare_name_with_utf8_string",
	 fsntfs_test_attribute_list_entry_compare_name_with_utf8_string,
	 attribute_list_entry );

	FSNTFS_TEST_RUN_WITH_ARGS(
	 "libfsntfs_attribute_list_entry_compare_name_with_utf16_string",
	 fsntfs_test_attribute_list_entry_compare_name_with_utf16_string,
	 attribute_list_entry );

	/* Clean up
	 */
	result = libfsntfs_attribute_list_entry_free(
	          &attribute_list_entry,
	          &error );

	FSNTFS_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "attribute_list_entry",
	 attribute_list_entry );

	FSNTFS_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

#endif /* !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 ) */

#endif /* defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
#if defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT )

	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( attribute_list_entry != NULL )
	{
		libfsntfs_attribute_list_entry_free(
		 &attribute_list_entry,
		 NULL );
	}
#endif /* defined( __GNUC__ ) && !defined( LIBFSNTFS_DLL_IMPORT ) */

	return( EXIT_FAILURE );
}

