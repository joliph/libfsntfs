/*
 * Attribute functions
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
#include <memory.h>
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#include "libfsntfs_attribute.h"
#include "libfsntfs_attribute_list_entry.h"
#include "libfsntfs_bitmap_values.h"
#include "libfsntfs_cluster_block.h"
#include "libfsntfs_cluster_block_stream.h"
#include "libfsntfs_cluster_block_vector.h"
#include "libfsntfs_data_run.h"
#include "libfsntfs_debug.h"
#include "libfsntfs_definitions.h"
#include "libfsntfs_file_name_values.h"
#include "libfsntfs_io_handle.h"
#include "libfsntfs_libbfio.h"
#include "libfsntfs_libcdata.h"
#include "libfsntfs_libcerror.h"
#include "libfsntfs_libcnotify.h"
#include "libfsntfs_libcthreads.h"
#include "libfsntfs_libfcache.h"
#include "libfsntfs_libfdata.h"
#include "libfsntfs_logged_utility_stream_values.h"
#include "libfsntfs_mft_attribute.h"
#include "libfsntfs_object_identifier_values.h"
#include "libfsntfs_reparse_point_values.h"
#include "libfsntfs_security_descriptor_values.h"
#include "libfsntfs_standard_information_values.h"
#include "libfsntfs_txf_data_values.h"
#include "libfsntfs_unused.h"
#include "libfsntfs_volume_information_values.h"
#include "libfsntfs_volume_name_values.h"

/* Creates an attribute
 * Make sure the value attribute is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_attribute_initialize(
     libfsntfs_attribute_t **attribute,
     libfsntfs_mft_attribute_t *mft_attribute,
     libfsntfs_attribute_list_entry_t *attribute_list_entry,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute = NULL;
	static char *function                              = "libfsntfs_attribute_initialize";

	if( attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute.",
		 function );

		return( -1 );
	}
	if( *attribute != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid attribute value already set.",
		 function );

		return( -1 );
	}
	if( ( ( mft_attribute == NULL )
	  &&  ( attribute_list_entry == NULL ) )
	 || ( ( mft_attribute != NULL )
	  &&  ( attribute_list_entry != NULL ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT attribute and attribute list entry.",
		 function );

		return( -1 );
	}
	internal_attribute = memory_allocate_structure(
	                      libfsntfs_internal_attribute_t );

	if( internal_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create attribute.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_attribute,
	     0,
	     sizeof( libfsntfs_internal_attribute_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear attribute.",
		 function );

		memory_free(
		 internal_attribute );

		return( -1 );
	}
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_initialize(
	     &( internal_attribute->read_write_lock ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize read/write lock.",
		 function );

		goto on_error;
	}
#endif
	internal_attribute->mft_attribute        = mft_attribute;
	internal_attribute->attribute_list_entry = attribute_list_entry;

	*attribute = (libfsntfs_attribute_t *) internal_attribute;

	return( 1 );

on_error:
	if( internal_attribute != NULL )
	{
		memory_free(
		 internal_attribute );
	}
	return( -1 );
}

/* Frees an attribute
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_attribute_free(
     libfsntfs_attribute_t **attribute,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_attribute_free";

	if( attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute.",
		 function );

		return( -1 );
	}
	if( *attribute != NULL )
	{
		*attribute = NULL;
	}
	return( 1 );
}

/* Frees an attribute
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_internal_attribute_free(
     libfsntfs_internal_attribute_t **internal_attribute,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_internal_attribute_free";
	int result            = 1;

	if( internal_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute.",
		 function );

		return( -1 );
	}
	if( *internal_attribute != NULL )
	{
#if defined( HAVE_LIBFSNTFS_MULTI_THREAD_SUPPORT )
		if( libcthreads_read_write_lock_free(
		     &( ( *internal_attribute )->read_write_lock ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free read/write lock.",
			 function );

			result = -1;
		}
#endif
		if( ( *internal_attribute )->mft_attribute != NULL )
		{
			if( libfsntfs_mft_attribute_free(
			     &( ( *internal_attribute )->mft_attribute ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free MFT attribute.",
				 function );

				result = -1;
			}
		}
		if( ( *internal_attribute )->value != NULL )
		{
			if( ( *internal_attribute )->free_value != NULL )
			{
				if( ( *internal_attribute )->free_value(
				     &( ( *internal_attribute )->value ),
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free attribute value.",
					 function );

					result = -1;
				}
			}
		}
		memory_free(
		 *internal_attribute );

		*internal_attribute = NULL;
	}
	return( result );
}

/* Compares attributes by their file reference
 * Returns LIBCDATA_COMPARE_LESS, LIBCDATA_COMPARE_EQUAL, LIBCDATA_COMPARE_GREATER if successful or -1 on error
 */
int libfsntfs_attribute_compare_by_file_reference(
     libfsntfs_internal_attribute_t *first_attribute,
     libfsntfs_internal_attribute_t *second_attribute,
     libcerror_error_t **error )
{
	static char *function           = "libfsntfs_attribute_compare_by_file_reference";
	uint64_t first_mft_entry_index  = 0;
	uint64_t second_mft_entry_index = 0;

	if( first_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid first attribute.",
		 function );

		return( -1 );
	}
	if( second_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid second attribute.",
		 function );

		return( -1 );
	}
	first_mft_entry_index  = first_attribute->file_reference & 0xffffffffffffUL;
	second_mft_entry_index = second_attribute->file_reference & 0xffffffffffffUL;

	if( first_mft_entry_index < second_mft_entry_index )
	{
		return( LIBCDATA_COMPARE_LESS );
	}
	else if( first_mft_entry_index > second_mft_entry_index )
	{
		return( LIBCDATA_COMPARE_GREATER );
	}
	return( LIBCDATA_COMPARE_EQUAL );
}

/* Reads the attribute value
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_attribute_read_value(
     libfsntfs_attribute_t *attribute,
     libfsntfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint8_t flags,
     libcerror_error_t **error )
{
	libfcache_cache_t *cluster_block_cache             = NULL;
	libfdata_stream_t *cluster_block_stream            = NULL;
	libfdata_vector_t *cluster_block_vector            = NULL;
	libfsntfs_cluster_block_t *cluster_block           = NULL;
	libfsntfs_internal_attribute_t *internal_attribute = NULL;
	uint8_t *resident_data                             = 0;
	static char *function                              = "libfsntfs_attribute_read_value";
	size_t resident_data_size                          = 0;
	uint32_t attribute_type                            = 0;
	uint16_t data_flags                                = 0;
	int cluster_block_index                            = 0;
	int number_of_cluster_blocks                       = 0;
	int result                                         = 0;

	if( attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute.",
		 function );

		return( -1 );
	}
	internal_attribute = (libfsntfs_internal_attribute_t *) attribute;

	/* Value already set ignore */
	if( internal_attribute->value != NULL )
	{
		return( 1 );
	}
	if( libfsntfs_internal_attribute_get_type(
	     internal_attribute,
	     &attribute_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve type from attribute.",
		 function );

		goto on_error;
	}
	if( libfsntfs_internal_attribute_get_data(
	     internal_attribute,
	     &resident_data,
	     &resident_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data from attribute.",
		 function );

		goto on_error;
	}
	if( resident_data != NULL )
	{
		switch( attribute_type )
		{
			case LIBFSNTFS_ATTRIBUTE_TYPE_BITMAP:
				internal_attribute->free_value = (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_bitmap_values_free;

				if( libfsntfs_bitmap_values_initialize(
				     (libfsntfs_bitmap_values_t **) &( internal_attribute->value ),
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create bitmap values.",
					 function );

					goto on_error;
				}
				if( libfsntfs_bitmap_values_read_data(
				     (libfsntfs_bitmap_values_t *) internal_attribute->value,
				     resident_data,
				     resident_data_size,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read bitmap values.",
					 function );

					goto on_error;
				}
				break;

			case LIBFSNTFS_ATTRIBUTE_TYPE_FILE_NAME:
				internal_attribute->free_value = (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_file_name_values_free;

				if( libfsntfs_file_name_values_initialize(
				     (libfsntfs_file_name_values_t **) &( internal_attribute->value ),
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create file name values.",
					 function );

					goto on_error;
				}
				if( libfsntfs_file_name_values_read_data(
				     (libfsntfs_file_name_values_t *) internal_attribute->value,
				     resident_data,
				     resident_data_size,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read file name values.",
					 function );

					goto on_error;
				}
				break;

			case LIBFSNTFS_ATTRIBUTE_TYPE_LOGGED_UTILITY_STREAM:
				result = libfsntfs_internal_attribute_compare_name_with_utf8_string(
					  internal_attribute,
					  (uint8_t *) "$TXF_DATA",
					  9,
					  error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GENERIC,
					 "%s: unable to compare UTF-8 string with attribute name.",
					 function );

					goto on_error;
				}
				else if( result != 0 )
				{
					internal_attribute->free_value = (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_txf_data_values_free;

					if( libfsntfs_txf_data_values_initialize(
					     (libfsntfs_txf_data_values_t **) &( internal_attribute->value ),
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
						 "%s: unable to create TxF data values.",
						 function );

						goto on_error;
					}
					if( libfsntfs_txf_data_values_read_data(
					     (libfsntfs_txf_data_values_t *) internal_attribute->value,
					     resident_data,
					     resident_data_size,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_IO,
						 LIBCERROR_IO_ERROR_READ_FAILED,
						 "%s: unable to read TxF data values.",
						 function );

						goto on_error;
					}
				}
				else
				{
					internal_attribute->free_value = (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_logged_utility_stream_values_free;

					if( libfsntfs_logged_utility_stream_values_initialize(
					     (libfsntfs_logged_utility_stream_values_t **) &( internal_attribute->value ),
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
						 "%s: unable to create logged utility stream values.",
						 function );

						goto on_error;
					}
					if( libfsntfs_logged_utility_stream_values_read_data(
					     (libfsntfs_logged_utility_stream_values_t *) internal_attribute->value,
					     resident_data,
					     resident_data_size,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_IO,
						 LIBCERROR_IO_ERROR_READ_FAILED,
						 "%s: unable to read logged utility stream values.",
						 function );

						goto on_error;
					}
				}
				break;

			case LIBFSNTFS_ATTRIBUTE_TYPE_OBJECT_IDENTIFIER:
				internal_attribute->free_value = (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_object_identifier_values_free;

				if( libfsntfs_object_identifier_values_initialize(
				     (libfsntfs_object_identifier_values_t **) &( internal_attribute->value ),
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create object identifier values.",
					 function );

					goto on_error;
				}
				if( libfsntfs_object_identifier_values_read_data(
				     (libfsntfs_object_identifier_values_t *) internal_attribute->value,
				     resident_data,
				     resident_data_size,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read object identifier values.",
					 function );

					goto on_error;
				}
				break;

			case LIBFSNTFS_ATTRIBUTE_TYPE_REPARSE_POINT:
				internal_attribute->free_value = (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_reparse_point_values_free;

				if( libfsntfs_reparse_point_values_initialize(
				     (libfsntfs_reparse_point_values_t **) &( internal_attribute->value ),
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create reparse point values.",
					 function );

					goto on_error;
				}
				if( libfsntfs_reparse_point_values_read_data(
				     (libfsntfs_reparse_point_values_t *) internal_attribute->value,
				     resident_data,
				     resident_data_size,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read reparse point values.",
					 function );

					goto on_error;
				}
				break;

			case LIBFSNTFS_ATTRIBUTE_TYPE_SECURITY_DESCRIPTOR:
				internal_attribute->free_value = (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_security_descriptor_values_free;

				if( libfsntfs_security_descriptor_values_initialize(
				     (libfsntfs_security_descriptor_values_t **) &( internal_attribute->value ),
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create security descriptor values.",
					 function );

					goto on_error;
				}
				if( libfsntfs_security_descriptor_values_read_buffer(
				     (libfsntfs_security_descriptor_values_t *) internal_attribute->value,
				     resident_data,
				     resident_data_size,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read security descriptor values.",
					 function );

					goto on_error;
				}
				break;

			case LIBFSNTFS_ATTRIBUTE_TYPE_STANDARD_INFORMATION:
				internal_attribute->free_value = (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_standard_information_values_free;

				if( libfsntfs_standard_information_values_initialize(
				     (libfsntfs_standard_information_values_t **) &( internal_attribute->value ),
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create standard information values.",
					 function );

					goto on_error;
				}
				if( libfsntfs_standard_information_values_read_data(
				     (libfsntfs_standard_information_values_t *) internal_attribute->value,
				     resident_data,
				     resident_data_size,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read standard information values.",
					 function );

					goto on_error;
				}
				break;

			case LIBFSNTFS_ATTRIBUTE_TYPE_VOLUME_INFORMATION:
				internal_attribute->free_value = (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_volume_information_values_free;

				if( libfsntfs_volume_information_values_initialize(
				     (libfsntfs_volume_information_values_t **) &( internal_attribute->value ),
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create volume information values.",
					 function );

					goto on_error;
				}
				if( libfsntfs_volume_information_values_read_data(
				     (libfsntfs_volume_information_values_t *) internal_attribute->value,
				     resident_data,
				     resident_data_size,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read volume information values.",
					 function );

					goto on_error;
				}
				break;

			case LIBFSNTFS_ATTRIBUTE_TYPE_VOLUME_NAME:
				internal_attribute->free_value = (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_volume_name_values_free;

				if( libfsntfs_volume_name_values_initialize(
				     (libfsntfs_volume_name_values_t **) &( internal_attribute->value ),
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create volume name values.",
					 function );

					goto on_error;
				}
				if( libfsntfs_volume_name_values_read_data(
				     (libfsntfs_volume_name_values_t *) internal_attribute->value,
				     resident_data,
				     resident_data_size,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read volume name values.",
					 function );

					goto on_error;
				}
				break;

			case LIBFSNTFS_ATTRIBUTE_TYPE_INDEX_ALLOCATION:
			case LIBFSNTFS_ATTRIBUTE_TYPE_INDEX_ROOT:
			default:
				break;
		}
	}
	else
	{
		switch( attribute_type )
		{
			case LIBFSNTFS_ATTRIBUTE_TYPE_BITMAP:
				internal_attribute->free_value = (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_bitmap_values_free;

				if( libfsntfs_bitmap_values_initialize(
				     (libfsntfs_bitmap_values_t **) &( internal_attribute->value ),
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create bitmap values.",
					 function );

					goto on_error;
				}
				break;

			case LIBFSNTFS_ATTRIBUTE_TYPE_SECURITY_DESCRIPTOR:
				internal_attribute->free_value = (int (*)(intptr_t **, libcerror_error_t **)) &libfsntfs_security_descriptor_values_free;

				if( libfsntfs_security_descriptor_values_initialize(
				     (libfsntfs_security_descriptor_values_t **) &( internal_attribute->value ),
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create security descriptor values.",
					 function );

					goto on_error;
				}
				break;

			case LIBFSNTFS_ATTRIBUTE_TYPE_FILE_NAME:
			case LIBFSNTFS_ATTRIBUTE_TYPE_OBJECT_IDENTIFIER:
			case LIBFSNTFS_ATTRIBUTE_TYPE_REPARSE_POINT:
			case LIBFSNTFS_ATTRIBUTE_TYPE_STANDARD_INFORMATION:
			case LIBFSNTFS_ATTRIBUTE_TYPE_VOLUME_INFORMATION:
			case LIBFSNTFS_ATTRIBUTE_TYPE_VOLUME_NAME:
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported non-resident attribute.",
				 function );

				goto on_error;

			case LIBFSNTFS_ATTRIBUTE_TYPE_INDEX_ALLOCATION:
			case LIBFSNTFS_ATTRIBUTE_TYPE_INDEX_ROOT:
			case LIBFSNTFS_ATTRIBUTE_TYPE_LOGGED_UTILITY_STREAM:
			default:
				break;
		}
		if( ( ( flags & LIBFSNTFS_FILE_ENTRY_FLAGS_MFT_ONLY ) == 0 )
		 && ( internal_attribute->value != NULL ) )
		{
			if( libfsntfs_internal_attribute_get_data_flags(
			     internal_attribute,
			     &data_flags,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve data flags.",
				 function );

				goto on_error;
			}
			if( ( data_flags & LIBFSNTFS_ATTRIBUTE_FLAG_COMPRESSION_MASK ) != 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported compressed attribute data.",
				 function );

				goto on_error;
			}
			if( attribute_type == LIBFSNTFS_ATTRIBUTE_TYPE_SECURITY_DESCRIPTOR )
			{
				if( libfsntfs_cluster_block_stream_initialize(
				     &cluster_block_stream,
				     io_handle,
				     internal_attribute->mft_attribute,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create cluster block stream.",
					 function );

					goto on_error;
				}
				if( libfsntfs_security_descriptor_values_read_stream(
				     (libfsntfs_security_descriptor_values_t *) internal_attribute->value,
				     file_io_handle,
				     cluster_block_stream,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read security descriptor values from stream.",
					 function );

					goto on_error;
				}
				if( libfdata_stream_free(
				     &cluster_block_stream,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free cluster block stream.",
					 function );

					goto on_error;
				}
			}
			else
			{
/* TODO pass mft_attribute to function */
				if( libfsntfs_cluster_block_vector_initialize(
				     &cluster_block_vector,
				     io_handle,
				     internal_attribute->mft_attribute,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create cluster block vector.",
					 function );

					goto on_error;
				}
				if( libfcache_cache_initialize(
				     &cluster_block_cache,
				     1,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create cluster block cache.",
					 function );

					goto on_error;
				}
/* TODO refactor read from vector into bitmap values */
				if( libfdata_vector_get_number_of_elements(
				     cluster_block_vector,
				     &number_of_cluster_blocks,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve number of cluster blocks.",
					 function );

					goto on_error;
				}
				for( cluster_block_index = 0;
				     cluster_block_index < number_of_cluster_blocks;
				     cluster_block_index++ )
				{
					if( libfdata_vector_get_element_value_by_index(
					     cluster_block_vector,
					     (intptr_t *) file_io_handle,
					     (libfdata_cache_t *) cluster_block_cache,
					     cluster_block_index,
					     (intptr_t **) &cluster_block,
					     0,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
						 "%s: unable to retrieve cluster block: %d from vector.",
						 function,
						 cluster_block_index );

						goto on_error;
					}
					if( cluster_block == NULL )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
						 "%s: missing cluster block: %d.",
						 function,
						 cluster_block_index );

						goto on_error;
					}
					if( cluster_block->data == NULL )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
						 "%s: invalid cluster block: %d - missing data.",
						 function,
						 cluster_block_index );

						goto on_error;
					}
					switch( attribute_type )
					{
						case LIBFSNTFS_ATTRIBUTE_TYPE_BITMAP:
							if( libfsntfs_bitmap_values_read_data(
							     (libfsntfs_bitmap_values_t *) internal_attribute->value,
							     cluster_block->data,
							     cluster_block->data_size,
							     error ) != 1 )
							{
								libcerror_error_set(
								 error,
								 LIBCERROR_ERROR_DOMAIN_IO,
								 LIBCERROR_IO_ERROR_READ_FAILED,
								 "%s: unable to read bitmap values.",
								 function );

								goto on_error;
							}
							break;
					}
				}
				if( libfdata_vector_free(
				     &cluster_block_vector,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free cluster block vector.",
					 function );

					goto on_error;
				}
				if( libfcache_cache_free(
				     &cluster_block_cache,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free cluster block cache.",
					 function );

					goto on_error;
				}
			}
		}
	}
/* TODO print trailing data */
	return( 1 );

on_error:
	if( cluster_block_cache != NULL )
	{
		libfcache_cache_free(
		 &cluster_block_cache,
		 NULL );
	}
	if( cluster_block_vector != NULL )
	{
		libfdata_vector_free(
		 &cluster_block_vector,
		 NULL );
	}
	if( cluster_block_stream != NULL )
	{
		libfdata_stream_free(
		 &cluster_block_stream,
		 NULL );
	}
	if( ( internal_attribute->value != NULL )
	 && ( internal_attribute->free_value != NULL ) )
	{
		internal_attribute->free_value(
		 &( internal_attribute->value ),
		 NULL );
	}
	return( -1 );
}

/* Retrieves the type
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_internal_attribute_get_type(
     libfsntfs_internal_attribute_t *internal_attribute,
     uint32_t *type,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_internal_attribute_get_type";

	if( internal_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute.",
		 function );

		return( -1 );
	}
	if( internal_attribute->mft_attribute != NULL )
	{
		if( libfsntfs_mft_attribute_get_type(
		     internal_attribute->mft_attribute,
		     type,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve type from MFT attribute.",
			 function );

			return( -1 );
		}
	}
	else
	{
		if( libfsntfs_attribute_list_entry_get_type(
		     internal_attribute->attribute_list_entry,
		     type,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve type from attribute list entry.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves the type
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_attribute_get_type(
     libfsntfs_attribute_t *attribute,
     uint32_t *type,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute = NULL;
	static char *function                              = "libfsntfs_attribute_get_type";
	int result                                         = 1;

	if( attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute.",
		 function );

		return( -1 );
	}
	internal_attribute = (libfsntfs_internal_attribute_t *) attribute;

#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_attribute->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libfsntfs_internal_attribute_get_type(
	     internal_attribute,
	     type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve type.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_attribute->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the data flags
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_internal_attribute_get_data_flags(
     libfsntfs_internal_attribute_t *internal_attribute,
     uint16_t *data_flags,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_internal_attribute_get_data_flags";

	if( internal_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute.",
		 function );

		return( -1 );
	}
	if( internal_attribute->mft_attribute != NULL )
	{
		if( libfsntfs_mft_attribute_get_data_flags(
		     internal_attribute->mft_attribute,
		     data_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data flags from MFT attribute.",
			 function );

			return( -1 );
		}
	}
	else
	{
		if( data_flags == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid data flags.",
			 function );

			return( -1 );
		}
		*data_flags = 0;
	}
	return( 1 );
}

/* Retrieves the data flags
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_attribute_get_data_flags(
     libfsntfs_attribute_t *attribute,
     uint16_t *data_flags,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute = NULL;
	static char *function                              = "libfsntfs_attribute_get_data_flags";
	int result                                         = 1;

	if( attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute.",
		 function );

		return( -1 );
	}
	internal_attribute = (libfsntfs_internal_attribute_t *) attribute;

#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_attribute->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libfsntfs_internal_attribute_get_data_flags(
	     internal_attribute,
	     data_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data flags.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_attribute->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the value
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_attribute_get_value(
     libfsntfs_attribute_t *attribute,
     intptr_t **value,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute = NULL;
	static char *function                              = "libfsntfs_attribute_get_value";

	if( attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute.",
		 function );

		return( -1 );
	}
	internal_attribute = (libfsntfs_internal_attribute_t *) attribute;

	if( value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_attribute->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	*value = internal_attribute->value;

#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_attribute->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_attribute_get_utf8_name_size(
     libfsntfs_attribute_t *attribute,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute = NULL;
	static char *function                              = "libfsntfs_attribute_get_utf8_name_size";
	int result                                         = 1;

	if( attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute.",
		 function );

		return( -1 );
	}
	internal_attribute = (libfsntfs_internal_attribute_t *) attribute;

#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_attribute->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_attribute->mft_attribute != NULL )
	{
		if( libfsntfs_mft_attribute_get_utf8_name_size(
		     internal_attribute->mft_attribute,
		     utf8_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-8 name size from MFT attribute.",
			 function );

			result = -1;
		}
	}
	else
	{
		if( libfsntfs_attribute_list_entry_get_utf8_name_size(
		     internal_attribute->attribute_list_entry,
		     utf8_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-8 name size from attribute list entry.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_attribute->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the UTF-8 encoded name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_attribute_get_utf8_name(
     libfsntfs_attribute_t *attribute,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute = NULL;
	static char *function                              = "libfsntfs_attribute_get_utf8_name";
	int result                                         = 1;

	if( attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute.",
		 function );

		return( -1 );
	}
	internal_attribute = (libfsntfs_internal_attribute_t *) attribute;

#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_attribute->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_attribute->mft_attribute != NULL )
	{
		if( libfsntfs_mft_attribute_get_utf8_name(
		     internal_attribute->mft_attribute,
		     utf8_string,
		     utf8_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-8 name from MFT attribute.",
			 function );

			result = -1;
		}
	}
	else
	{
		if( libfsntfs_attribute_list_entry_get_utf8_name(
		     internal_attribute->attribute_list_entry,
		     utf8_string,
		     utf8_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-8 name from attribute list entry.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_attribute->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the size of the UTF-16 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_attribute_get_utf16_name_size(
     libfsntfs_attribute_t *attribute,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute = NULL;
	static char *function                              = "libfsntfs_attribute_get_utf16_name_size";
	int result                                         = 1;

	if( attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute.",
		 function );

		return( -1 );
	}
	internal_attribute = (libfsntfs_internal_attribute_t *) attribute;

#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_attribute->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_attribute->mft_attribute != NULL )
	{
		if( libfsntfs_mft_attribute_get_utf16_name_size(
		     internal_attribute->mft_attribute,
		     utf16_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-16 name size from MFT attribute.",
			 function );

			result = -1;
		}
	}
	else
	{
		if( libfsntfs_attribute_list_entry_get_utf16_name_size(
		     internal_attribute->attribute_list_entry,
		     utf16_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-16 name size from attribute list entry.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_attribute->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the UTF-16 encoded name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_attribute_get_utf16_name(
     libfsntfs_attribute_t *attribute,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute = NULL;
	static char *function                              = "libfsntfs_attribute_get_utf16_name";
	int result                                         = 1;

	if( attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute.",
		 function );

		return( -1 );
	}
	internal_attribute = (libfsntfs_internal_attribute_t *) attribute;

#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_attribute->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_attribute->mft_attribute != NULL )
	{
		if( libfsntfs_mft_attribute_get_utf16_name(
		     internal_attribute->mft_attribute,
		     utf16_string,
		     utf16_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-16 name from MFT attribute.",
			 function );

			result = -1;
		}
	}
	else
	{
		if( libfsntfs_attribute_list_entry_get_utf16_name(
		     internal_attribute->attribute_list_entry,
		     utf16_string,
		     utf16_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-16 name from attribute list entry.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_attribute->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the data VCN range
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsntfs_attribute_get_data_vcn_range(
     libfsntfs_attribute_t *attribute,
     uint64_t *data_first_vcn,
     uint64_t *data_last_vcn,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute = NULL;
	static char *function                              = "libfsntfs_attribute_get_data_vcn_range";
	int result                                         = 0;

	if( attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute.",
		 function );

		return( -1 );
	}
	internal_attribute = (libfsntfs_internal_attribute_t *) attribute;

	if( data_first_vcn == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data first VCN.",
		 function );

		return( -1 );
	}
	if( data_last_vcn == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data last VCN.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_attribute->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_attribute->mft_attribute != NULL )
	{
		result = libfsntfs_mft_attribute_get_data_vcn_range(
		          internal_attribute->mft_attribute,
		          data_first_vcn,
		          data_last_vcn,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data VCN range from MFT attribute.",
			 function );

			result = -1;
		}
	}
/* TODO add support for attribute list entry ? */

#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_attribute->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the file references as an MFT entry index and sequence number
 * If the value sequence_number is NULL it will be ignored
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_attribute_get_file_reference(
     libfsntfs_attribute_t *attribute,
     uint64_t *mft_entry_index,
     uint16_t *sequence_number,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute = NULL;
	static char *function                              = "libfsntfs_attribute_get_file_reference";

	if( attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute.",
		 function );

		return( -1 );
	}
	internal_attribute = (libfsntfs_internal_attribute_t *) attribute;

	if( mft_entry_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid MFT entry index.",
		 function );

		return( -1 );
	}
	*mft_entry_index = internal_attribute->file_reference & 0xffffffffffffUL;

	if( sequence_number != NULL )
	{
		*sequence_number = (uint16_t) ( internal_attribute->file_reference >> 48 );
	}
	return( 1 );
}

/* Compares the name with an UTF-8 encoded string
 * Returns 1 if the strings are equal, 0 if not or -1 on error
 */
int libfsntfs_internal_attribute_compare_name_with_utf8_string(
     libfsntfs_internal_attribute_t *internal_attribute,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_internal_attribute_compare_name_with_utf8_string";
	int result            = 0;

	if( internal_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute.",
		 function );

		return( -1 );
	}
	if( internal_attribute->mft_attribute != NULL )
	{
		result = libfsntfs_mft_attribute_compare_name_with_utf8_string(
		          internal_attribute->mft_attribute,
		          utf8_string,
		          utf8_string_length,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to compare UTF-8 string with MFT attribute name.",
			 function );

			return( -1 );
		}
	}
	else
	{
		result = libfsntfs_attribute_list_entry_compare_name_with_utf8_string(
		          internal_attribute->attribute_list_entry,
		          utf8_string,
		          utf8_string_length,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to compare UTF-8 string with attribute list entry name.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Compares the name with an UTF-16 encoded string
 * Returns 1 if the strings are equal, 0 if not or -1 on error
 */
int libfsntfs_internal_attribute_compare_name_with_utf16_string(
     libfsntfs_internal_attribute_t *internal_attribute,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_internal_attribute_compare_name_with_utf16_string";
	int result            = 0;

	if( internal_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute.",
		 function );

		return( -1 );
	}
	if( internal_attribute->mft_attribute != NULL )
	{
		result = libfsntfs_mft_attribute_compare_name_with_utf16_string(
		          internal_attribute->mft_attribute,
		          utf16_string,
		          utf16_string_length,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to compare UTF-16 string with MFT attribute name.",
			 function );

			return( -1 );
		}
	}
	else
	{
		result = libfsntfs_attribute_list_entry_compare_name_with_utf16_string(
		          internal_attribute->attribute_list_entry,
		          utf16_string,
		          utf16_string_length,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to compare UTF-16 string with attribute list entry name.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the data size
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_internal_attribute_get_data_size(
     libfsntfs_internal_attribute_t *internal_attribute,
     size64_t *data_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_internal_attribute_get_data_size";

	if( internal_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute.",
		 function );

		return( -1 );
	}
	if( internal_attribute->mft_attribute != NULL )
	{
		if( libfsntfs_mft_attribute_get_data_size(
		     internal_attribute->mft_attribute,
		     (uint64_t *) data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data size.",
			 function );

			return( -1 );
		}
	}
	else
	{
		if( data_size == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid data size.",
			 function );

			return( -1 );
		}
		*data_size = 0;
	}
	return( 1 );
}

/* Retrieves the data size
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_attribute_get_data_size(
     libfsntfs_attribute_t *attribute,
     size64_t *data_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute = NULL;
	static char *function                              = "libfsntfs_attribute_get_data_size";
	int result                                         = 1;

	if( attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute.",
		 function );

		return( -1 );
	}
	internal_attribute = (libfsntfs_internal_attribute_t *) attribute;

#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_attribute->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libfsntfs_internal_attribute_get_data_size(
	     internal_attribute,
	     data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data size.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_attribute->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the data
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_internal_attribute_get_data(
     libfsntfs_internal_attribute_t *internal_attribute,
     uint8_t **data,
     size64_t *data_size,
     libcerror_error_t **error )
{
	static char *function = "libfsntfs_internal_attribute_get_data";

	if( internal_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute.",
		 function );

		return( -1 );
	}
	if( internal_attribute->mft_attribute != NULL )
	{
		if( libfsntfs_mft_attribute_get_data(
		     internal_attribute->mft_attribute,
		     data,
		     data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data.",
			 function );

			return( -1 );
		}
	}
	else
	{
		if( data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid data.",
			 function );

			return( -1 );
		}
		if( data_size == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid data size.",
			 function );

			return( -1 );
		}
		*data      = NULL;
		*data_size = 0;
	}
	return( 1 );
}

/* Retrieves the valid data size
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_attribute_get_valid_data_size(
     libfsntfs_attribute_t *attribute,
     size64_t *valid_data_size,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute = NULL;
	static char *function                              = "libfsntfs_attribute_get_valid_data_size";

	if( attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute.",
		 function );

		return( -1 );
	}
	internal_attribute = (libfsntfs_internal_attribute_t *) attribute;

	if( internal_attribute->mft_attribute != NULL )
	{
		if( libfsntfs_mft_attribute_get_valid_data_size(
		     internal_attribute->mft_attribute,
		     (uint64_t *) valid_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve valid data size.",
			 function );

			return( -1 );
		}
	}
	else
	{
		if( valid_data_size == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid valid data size.",
			 function );

			return( -1 );
		}
		*valid_data_size = 0;
	}
	return( 1 );
}

/* Retrieves the number of data runs
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_attribute_get_number_of_data_runs(
     libfsntfs_attribute_t *attribute,
     int *number_of_data_runs,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute = NULL;
	static char *function                              = "libfsntfs_attribute_get_number_of_data_runs";

	if( attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute.",
		 function );

		return( -1 );
	}
	internal_attribute = (libfsntfs_internal_attribute_t *) attribute;

	if( internal_attribute->mft_attribute != NULL )
	{
		if( libfsntfs_mft_attribute_get_number_of_data_runs(
		     internal_attribute->mft_attribute,
		     number_of_data_runs,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of data runs.",
			 function );

			return( -1 );
		}
	}
	else
	{
		if( number_of_data_runs == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid number of data runs.",
			 function );

			return( -1 );
		}
		*number_of_data_runs = 0;
	}
	return( 1 );
}

/* Retrieves a specific data run
 * Returns 1 if successful or -1 on error
 */
int libfsntfs_attribute_get_data_run_by_index(
     libfsntfs_attribute_t *attribute,
     int data_run_index,
     libfsntfs_data_run_t **data_run,
     libcerror_error_t **error )
{
	libfsntfs_internal_attribute_t *internal_attribute = NULL;
	static char *function                              = "libfsntfs_attribute_get_data_run_by_index";

	if( attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid attribute.",
		 function );

		return( -1 );
	}
	internal_attribute = (libfsntfs_internal_attribute_t *) attribute;

	if( libfsntfs_mft_attribute_get_data_run_by_index(
	     internal_attribute->mft_attribute,
	     data_run_index,
	     data_run,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data run: %d.",
		 function,
		 data_run_index );

		return( -1 );
	}
	return( 1 );
}

