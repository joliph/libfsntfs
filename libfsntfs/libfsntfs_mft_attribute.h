/*
 * Master File Table (MFT) attribute functions
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

#if !defined( _LIBFSNTFS_MFT_ATTRIBUTE_H )
#define _LIBFSNTFS_MFT_ATTRIBUTE_H

#include <common.h>
#include <types.h>

#include "libfsntfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsntfs_mft_attribute libfsntfs_mft_attribute_t;

struct libfsntfs_mft_attribute
{
	/* The type
	 */
	uint32_t type;

	/* The size
	 */
	uint32_t size;

	/* The non-resident flag
	 */
	uint8_t non_resident_flag;

	/* The name size
	 */
	uint16_t name_size;

	/* The data flags
	 */
	uint16_t data_flags;

	/* The identifier
	 */
	uint16_t identifier;

	/* The data size
	 */
	uint64_t data_size;

	/* The data offset
	 */
	uint16_t data_offset;

	/* The data first VCN
	 */
	uint64_t data_first_vcn;

	/* The data last VCN
	 */
	uint64_t data_last_vcn;

	/* The data runs offset
	 */
	uint16_t data_runs_offset;

	/* The compression unit size
	 */
	size_t compression_unit_size;

	/* The allocated data size
	 */
	uint64_t allocated_data_size;

	/* The valid data size
	 */
	uint64_t valid_data_size;

	/* The name
	 */
	uint8_t *name;
};

int libfsntfs_mft_attribute_initialize(
     libfsntfs_mft_attribute_t **mft_attribute,
     libcerror_error_t **error );

int libfsntfs_mft_attribute_free(
     libfsntfs_mft_attribute_t **mft_attribute,
     libcerror_error_t **error );

int libfsntfs_mft_attribute_read_data(
     libfsntfs_mft_attribute_t *mft_attribute,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSNTFS_MFT_ATTRIBUTE_H ) */
