/*******************************************************************************
 *
 * Module Name: uterror - Various internal error/warning output functions
 *
 ******************************************************************************/

/*
 * Copyright (C) 2000 - 2014, Intel Corp.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions, and the following disclaimer,
 *    without modification.
 * 2. Redistributions in binary form must reproduce at minimum a disclaimer
 *    substantially similar to the "NO WARRANTY" disclaimer below
 *    ("Disclaimer") and any redistribution must be conditioned upon
 *    including a substantially similar Disclaimer requirement for further
 *    binary redistribution.
 * 3. Neither the names of the above-listed copyright holders nor the names
 *    of any contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * Alternatively, this software may be distributed under the terms of the
 * GNU General Public License ("GPL") version 2 as published by the Free
 * Software Foundation.
 *
 * NO WARRANTY
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDERS OR CONTRIBUTORS BE LIABLE FOR SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGES.
 */

#include <acpi/acpi.h>
#include "accommon.h"
#include "acnamesp.h"

#define _COMPONENT          ACPI_UTILITIES
ACPI_MODULE_NAME("uterror")

/*
 * This module contains internal error functions that may
 * be configured out.
 */
#if !defined (ACPI_NO_ERROR_MESSAGES)
/*******************************************************************************
 *
 * FUNCTION:    acpi_ut_predefined_warning
 *
 * PARAMETERS:  module_name     - Caller's module name (for error output)
 *              line_number     - Caller's line number (for error output)
 *              pathname        - Full pathname to the node
 *              node_flags      - From Namespace node for the method/object
 *              format          - Printf format string + additional args
 *
 * RETURN:      None
 *
 * DESCRIPTION: Warnings for the predefined validation module. Messages are
 *              only emitted the first time a problem with a particular
 *              method/object is detected. This prevents a flood of error
 *              messages for methods that are repeatedly evaluated.
 *
 ******************************************************************************/
void ACPI_INTERNAL_VAR_XFACE
acpi_ut_predefined_warning(const char *module_name,
			   u32 line_number,
			   char *pathname,
			   u8 node_flags, const char *format, ...)
{
	va_list arg_list;

	/*
	 * Warning messages for this method/object will be disabled after the
	 * first time a validation fails or an object is successfully repaired.
	 */
	if (node_flags & ANOBJ_EVALUATED) {
		return;
	}

	acpi_os_printf(ACPI_MSG_WARNING "%s: ", pathname);

	va_start(arg_list, format);
	acpi_os_vprintf(format, arg_list);
	ACPI_MSG_SUFFIX;
	va_end(arg_list);
}

/*******************************************************************************
 *
 * FUNCTION:    acpi_ut_predefined_info
 *
 * PARAMETERS:  module_name     - Caller's module name (for error output)
 *              line_number     - Caller's line number (for error output)
 *              pathname        - Full pathname to the node
 *              node_flags      - From Namespace node for the method/object
 *              format          - Printf format string + additional args
 *
 * RETURN:      None
 *
 * DESCRIPTION: Info messages for the predefined validation module. Messages
 *              are only emitted the first time a problem with a particular
 *              method/object is detected. This prevents a flood of
 *              messages for methods that are repeatedly evaluated.
 *
 ******************************************************************************/

void ACPI_INTERNAL_VAR_XFACE
acpi_ut_predefined_info(const char *module_name,
			u32 line_number,
			char *pathname, u8 node_flags, const char *format, ...)
{
	va_list arg_list;

	/*
	 * Warning messages for this method/object will be disabled after the
	 * first time a validation fails or an object is successfully repaired.
	 */
	if (node_flags & ANOBJ_EVALUATED) {
		return;
	}

	acpi_os_printf(ACPI_MSG_INFO "%s: ", pathname);

	va_start(arg_list, format);
	acpi_os_vprintf(format, arg_list);
	ACPI_MSG_SUFFIX;
	va_end(arg_list);
}

/*******************************************************************************
 *
 * FUNCTION:    acpi_ut_predefined_bios_error
 *
 * PARAMETERS:  module_name     - Caller's module name (for error output)
 *              line_number     - Caller's line number (for error output)
 *              pathname        - Full pathname to the node
 *              node_flags      - From Namespace node for the method/object
 *              format          - Printf format string + additional args
 *
 * RETURN:      None
 *
 * DESCRIPTION: BIOS error message for predefined names. Messages
 *              are only emitted the first time a problem with a particular
 *              method/object is detected. This prevents a flood of
 *              messages for methods that are repeatedly evaluated.
 *
 ******************************************************************************/

void ACPI_INTERNAL_VAR_XFACE
acpi_ut_predefined_bios_error(const char *module_name,
			      u32 line_number,
			      char *pathname,
			      u8 node_flags, const char *format, ...)
{
	va_list arg_list;

	/*
	 * Warning messages for this method/object will be disabled after the
	 * first time a validation fails or an object is successfully repaired.
	 */
	if (node_flags & ANOBJ_EVALUATED) {
		return;
	}

	acpi_os_printf(ACPI_MSG_BIOS_ERROR "%s: ", pathname);

	va_start(arg_list, format);
	acpi_os_vprintf(format, arg_list);
	ACPI_MSG_SUFFIX;
	va_end(arg_list);
}

/*******************************************************************************
 *
 * FUNCTION:    acpi_ut_namespace_error
 *
 * PARAMETERS:  module_name         - Caller's module name (for error output)
 *              line_number         - Caller's line number (for error output)
 *              internal_name       - Name or path of the namespace node
 *              lookup_status       - Exception code from NS lookup
 *
 * RETURN:      None
 *
 * DESCRIPTION: Print error message with the full pathname for the NS node.
 *
 ******************************************************************************/

void
acpi_ut_namespace_error(const char *module_name,
			u32 line_number,
			const char *internal_name, acpi_status lookup_status)
{
	acpi_status status;
	u32 bad_name;
	char *name = NULL;

	ACPI_MSG_REDIRECT_BEGIN;
	acpi_os_printf(ACPI_MSG_ERROR);

	if (lookup_status == AE_BAD_CHARACTER) {

		/* There is a non-ascii character in the name */

		ACPI_MOVE_32_TO_32(&bad_name,
				   ACPI_CAST_PTR(u32, internal_name));
		acpi_os_printf("[0x%.8X] (NON-ASCII)", bad_name);
	} else {
		/* Convert path to external format */

		status = acpi_ns_externalize_name(ACPI_UINT32_MAX,
						  internal_name, NULL, &name);

		/* Print target name */

		if (ACPI_SUCCESS(status)) {
			acpi_os_printf("[%s]", name);
		} else {
			acpi_os_printf("[COULD NOT EXTERNALIZE NAME]");
		}

		if (name) {
			ACPI_FREE(name);
		}
	}

	acpi_os_printf(" Namespace lookup failure, %s",
		       acpi_format_exception(lookup_status));

	ACPI_MSG_SUFFIX;
	ACPI_MSG_REDIRECT_END;
}

/*******************************************************************************
 *
 * FUNCTION:    acpi_ut_method_error
 *
 * PARAMETERS:  module_name         - Caller's module name (for error output)
 *              line_number         - Caller's line number (for error output)
 *              message             - Error message to use on failure
 *              prefix_node         - Prefix relative to the path
 *              path                - Path to the node (optional)
 *              method_status       - Execution status
 *
 * RETURN:      None
 *
 * DESCRIPTION: Print error message with the full pathname for the method.
 *
 ******************************************************************************/

void
acpi_ut_method_error(const char *module_name,
		     u32 line_number,
		     const char *message,
		     struct acpi_namespace_node *prefix_node,
		     const char *path, acpi_status method_status)
{
	acpi_status status;
	struct acpi_namespace_node *node = prefix_node;

	ACPI_MSG_REDIRECT_BEGIN;
	acpi_os_printf(ACPI_MSG_ERROR);

	if (path) {
		status =
		    acpi_ns_get_node(prefix_node, path, ACPI_NS_NO_UPSEARCH,
				     &node);
		if (ACPI_FAILURE(status)) {
			acpi_os_printf("[Could not get node by pathname]");
		}
	}

	acpi_ns_print_node_pathname(node, message);
	acpi_os_printf(", %s", acpi_format_exception(method_status));

	ACPI_MSG_SUFFIX;
	ACPI_MSG_REDIRECT_END;
}

#endif				/* ACPI_NO_ERROR_MESSAGES */
