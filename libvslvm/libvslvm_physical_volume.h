/*
 * Physical volume functions
 *
 * Copyright (c) 2014, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBVSLVM_INTERNAL_PHYSICAL_VOLUME_H )
#define _LIBVSLVM_INTERNAL_PHYSICAL_VOLUME_H

#include <common.h>
#include <types.h>

#include "libvslvm_extern.h"
#include "libvslvm_libcerror.h"
#include "libvslvm_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libvslvm_internal_physical_volume libvslvm_internal_physical_volume_t;

struct libvslvm_internal_physical_volume
{
	/* The name
	 */
	char *name;

	/* The name size
	 */
	size_t name_size;

	/* The identfier
	 */
	char identifier[ 39 ];

	/* The device path
	 */
	char *device_path;

	/* The device path size
	 */
	size_t device_path_size;

	/* The size
	 */
	size64_t size;
};

int libvslvm_physical_volume_initialize(
     libvslvm_physical_volume_t **physical_volume,
     libcerror_error_t **error );

LIBVSLVM_EXTERN \
int libvslvm_physical_volume_free(
     libvslvm_physical_volume_t **physical_volume,
     libcerror_error_t **error );

int libvslvm_internal_physical_volume_free(
     libvslvm_internal_physical_volume_t **internal_physical_volume,
     libcerror_error_t **error );

LIBVSLVM_EXTERN \
int libvslvm_physical_volume_get_name_size(
     libvslvm_physical_volume_t *physical_volume,
     size_t *name_size,
     libcerror_error_t **error );

LIBVSLVM_EXTERN \
int libvslvm_physical_volume_get_name(
     libvslvm_physical_volume_t *physical_volume,
     char *name,
     size_t name_size,
     libcerror_error_t **error );

int libvslvm_physical_volume_set_name(
     libvslvm_physical_volume_t *physical_volume,
     const char *name,
     size_t name_size,
     libcerror_error_t **error );

LIBVSLVM_EXTERN \
int libvslvm_physical_volume_get_identifier_size(
     libvslvm_physical_volume_t *physical_volume,
     size_t *identifier_size,
     libcerror_error_t **error );

LIBVSLVM_EXTERN \
int libvslvm_physical_volume_get_identifier(
     libvslvm_physical_volume_t *physical_volume,
     char *identifier,
     size_t identifier_size,
     libcerror_error_t **error );

int libvslvm_physical_volume_set_identifier(
     libvslvm_physical_volume_t *physical_volume,
     const char *identifier,
     size_t identifier_size,
     libcerror_error_t **error );

LIBVSLVM_EXTERN \
int libvslvm_physical_volume_get_device_path_size(
     libvslvm_physical_volume_t *physical_volume,
     size_t *device_path_size,
     libcerror_error_t **error );

LIBVSLVM_EXTERN \
int libvslvm_physical_volume_get_device_path(
     libvslvm_physical_volume_t *physical_volume,
     char *device_path,
     size_t device_path_size,
     libcerror_error_t **error );

int libvslvm_physical_volume_set_device_path(
     libvslvm_physical_volume_t *physical_volume,
     const char *device_path,
     size_t device_path_size,
     libcerror_error_t **error );

LIBVSLVM_EXTERN \
int libvslvm_physical_volume_get_size(
     libvslvm_physical_volume_t *physical_volume,
     size64_t *size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif
