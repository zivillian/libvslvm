/*
 * The metadata area functions
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
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libvslvm_definitions.h"
#include "libvslvm_libbfio.h"
#include "libvslvm_libcdata.h"
#include "libvslvm_libcerror.h"
#include "libvslvm_libcnotify.h"
#include "libvslvm_libcsplit.h"
#include "libvslvm_libcstring.h"
#include "libvslvm_libfvalue.h"
#include "libvslvm_metadata.h"
#include "libvslvm_metadata_area.h"
#include "libvslvm_raw_location_descriptor.h"

#include "vslvm_metadata_area.h"

const char *vslvm_metadata_area_signature = "\x20LVM2\x20x[5A%r0N*>";

const uint8_t vslvm_empty_raw_location_descriptor[ 24 ] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0 };

/* Creates a metadata area
 * Make sure the value metadata_area is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libvslvm_metadata_area_initialize(
     libvslvm_metadata_area_t **metadata_area,
     libcerror_error_t **error )
{
	static char *function = "libvslvm_metadata_area_initialize";

	if( metadata_area == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid metadata area.",
		 function );

		return( -1 );
	}
	if( *metadata_area != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid metadata area value already set.",
		 function );

		return( -1 );
	}
	*metadata_area = memory_allocate_structure(
	                  libvslvm_metadata_area_t );

	if( *metadata_area == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create metadata area.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *metadata_area,
	     0,
	     sizeof( libvslvm_metadata_area_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear metadata area.",
		 function );

		memory_free(
		 *metadata_area );

		*metadata_area = NULL;

		return( -1 );
	}
	if( libcdata_array_initialize(
	     &( ( *metadata_area )->raw_location_descriptors_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create raw location descriptors array.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *metadata_area != NULL )
	{
		memory_free(
		 *metadata_area );

		*metadata_area = NULL;
	}
	return( -1 );
}

/* Frees a metadata area
 * Returns 1 if successful or -1 on error
 */
int libvslvm_metadata_area_free(
     libvslvm_metadata_area_t **metadata_area,
     libcerror_error_t **error )
{
	static char *function = "libvslvm_metadata_area_free";
	int result            = 1;

	if( metadata_area == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid metadata area.",
		 function );

		return( -1 );
	}
	if( *metadata_area != NULL )
	{
		if( libcdata_array_free(
		     &( ( *metadata_area )->raw_location_descriptors_array ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libvslvm_raw_location_descriptor_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free raw location descriptors array.",
			 function );

			result = -1;
		}
		memory_free(
		 *metadata_area );

		*metadata_area = NULL;
	}
	return( result );
}

/* Reads the metadata area
 * Returns 1 if successful or -1 on error
 */
int libvslvm_metadata_area_read(
     libvslvm_metadata_area_t *metadata_area,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error )
{
	vslvm_raw_location_descriptor_t raw_location_descriptor_data;
	vslvm_metadata_area_header_t metadata_area_header;

	libvslvm_raw_location_descriptor_t *raw_location_descriptor = NULL;
	static char *function                                       = "libvslvm_metadata_area_read";
	ssize_t read_count                                          = 0;
	uint64_t offset                                             = 0;
	uint64_t size                                               = 0;
	uint32_t checksum                                           = 0;
	uint32_t flags                                              = 0;
	int entry_index                                             = 0;
	int result                                                  = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit                                        = 0;
	uint32_t value_32bit                                        = 0;
#endif

	if( metadata_area == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid metadata area.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading metadata area at offset: %" PRIi64 "\n",
		 function,
		 file_offset );
	}
#endif
	if( libbfio_handle_seek_offset(
	     file_io_handle,
	     file_offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		goto on_error;
	}
	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              (uint8_t *) &metadata_area_header,
	              sizeof( vslvm_metadata_area_header_t ),
	              error );

	if( read_count != (ssize_t) sizeof( vslvm_metadata_area_header_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read metadata area header.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: metadata area header data:\n",
		 function );
		libcnotify_print_data(
		 (uint8_t *) &metadata_area_header,
		 sizeof( vslvm_metadata_area_header_t ),
		 0 );
	}
#endif
	if( memory_compare(
	     metadata_area_header.signature,
	     vslvm_metadata_area_signature,
	     8 ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported metadata area signature.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		byte_stream_copy_to_uint32_little_endian(
		 metadata_area_header.checksum,
		 value_32bit );
		libcnotify_printf(
		 "%s: checksum\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: signature\t\t\t\t\t: %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",
		 function,
		 metadata_area_header.signature[ 0 ],
		 metadata_area_header.signature[ 1 ],
		 metadata_area_header.signature[ 2 ],
		 metadata_area_header.signature[ 3 ],
		 metadata_area_header.signature[ 4 ],
		 metadata_area_header.signature[ 5 ],
		 metadata_area_header.signature[ 6 ],
		 metadata_area_header.signature[ 7 ],
		 metadata_area_header.signature[ 8 ],
		 metadata_area_header.signature[ 9 ],
		 metadata_area_header.signature[ 10 ],
		 metadata_area_header.signature[ 11 ],
		 metadata_area_header.signature[ 12 ],
		 metadata_area_header.signature[ 13 ],
		 metadata_area_header.signature[ 14 ],
		 metadata_area_header.signature[ 15 ] );

		byte_stream_copy_to_uint32_little_endian(
		 metadata_area_header.version,
		 value_32bit );
		libcnotify_printf(
		 "%s: version\t\t\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint64_little_endian(
		 metadata_area_header.data_offset,
		 value_64bit );
		libcnotify_printf(
		 "%s: data offset\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 metadata_area_header.data_size,
		 value_64bit );
		libcnotify_printf(
		 "%s: data size\t\t\t\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		libcnotify_printf(
		 "\n" );
	}
#endif
/* TODO calculate checksum */

	do
	{
		read_count = libbfio_handle_read_buffer(
		              file_io_handle,
		              (uint8_t *) &raw_location_descriptor_data,
		              sizeof( vslvm_raw_location_descriptor_t ),
		              error );

		if( read_count != (ssize_t) sizeof( vslvm_raw_location_descriptor_t ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read raw location descriptor.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: raw location descriptor data:\n",
			 function );
			libcnotify_print_data(
			 (uint8_t *) &raw_location_descriptor_data,
			 sizeof( vslvm_raw_location_descriptor_t ),
			 0 );
		}
#endif
		result = memory_compare(
		          (uint8_t *) &raw_location_descriptor_data,
		          vslvm_empty_raw_location_descriptor,
		          sizeof( vslvm_raw_location_descriptor_t ) );

		if( result != 0 )
		{
			byte_stream_copy_to_uint64_little_endian(
			 raw_location_descriptor_data.offset,
			 offset );

			byte_stream_copy_to_uint64_little_endian(
			 raw_location_descriptor_data.size,
			 size );

			byte_stream_copy_to_uint32_little_endian(
			 raw_location_descriptor_data.checksum,
			 checksum );

			byte_stream_copy_to_uint32_little_endian(
			 raw_location_descriptor_data.flags,
			 flags );

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: offset\t\t\t\t\t: 0x%08" PRIx64 "\n",
				 function,
				 offset );

				libcnotify_printf(
				 "%s: size\t\t\t\t\t: %" PRIu64 "\n",
				 function,
				 size );

				libcnotify_printf(
				 "%s: checksum\t\t\t\t\t: 0x%08" PRIx32 "\n",
				 function,
				 checksum );

				libcnotify_printf(
				 "%s: flags\t\t\t\t\t: 0x%08" PRIx32 "\n",
				 function,
				 flags );

				libcnotify_printf(
				 "\n" );
			}
#endif
			if( ( flags & LIBVSLVM_RAW_LOCATION_DESCRIPTOR_FLAG_IGNORE ) == 0 )
			{
				if( libvslvm_raw_location_descriptor_initialize(
				     &raw_location_descriptor,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create raw location descriptor.",
					 function );

					goto on_error;
				}
				if( libvslvm_raw_location_descriptor_set(
				     raw_location_descriptor,
				     (off64_t) ( file_offset + offset ),
				     (size64_t) size,
				     checksum,
				     flags,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to set raw location descriptor.",
					 function );

					goto on_error;
				}
				if( libcdata_array_append_entry(
				     metadata_area->raw_location_descriptors_array,
				     &entry_index,
				     (intptr_t *) raw_location_descriptor,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
					 "%s: unable to append entry to raw location descriptors array.",
					 function );

					goto on_error;
				}
				raw_location_descriptor = NULL;
			}
		}
	}
	while( result != 0 );

	return( 1 );

on_error:
	if( raw_location_descriptor != NULL )
	{
		libvslvm_raw_location_descriptor_free(
		 &raw_location_descriptor,
		 NULL );
	}
	libcdata_array_empty(
	 metadata_area->raw_location_descriptors_array,
	 (int (*)(intptr_t **, libcerror_error_t **)) &libvslvm_raw_location_descriptor_free,
	 NULL );

	return( -1 );
}

