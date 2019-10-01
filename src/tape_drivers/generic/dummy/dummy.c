/*
**
** Copyright (c) 2019 IBM Corp.
**
** This program is free software; you can redistribute it and/or modify it
**  under the terms of version 2.1 of the GNU Lesser General Public License
**  as published by the Free Software Foundation.
**
** This program is distributed in the hope that it will be useful, but
**  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
**  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
**  License for more details.
**
** You should have received a copy of the GNU General Public License along
**  with this program; if not, write to:
**    Free Software Foundation, Inc.
**    51 Franklin Street, Fifth Floor
**    Boston, MA 02110-1301, USA.
**
*************************************************************************************
**
** COMPONENT NAME:  LTFS 3rd party backend project
**
** FILE NAME:       dummy,c
**
** DESCRIPTION:     Dummy backend to check the compatibility with LTFS's backend I/F
**
** AUTHOR:          Atsushi Abe
**                  IBM Tokyo Lab., Japan
**                  piste@jp.ibm.com
**
*************************************************************************************
*/

#include <stdlib.h>

#include <fuse.h>

#include "libltfs/tape_ops.h"
#include "libltfs/ltfs_error.h"

#include "tape_drivers/tape_drivers.h"

/**
 * Emulator-specific data structures, used in lieu of a file descriptor
 */
struct dummy_data {
	int      drive_type;                   /**< drive type defined by ltfs */
	char     *serial_number;               /**< Serial number of this dummy tape device */
	char     *product_id;                  /**< Product ID of this dummy tape device */
};

static struct fuse_opt dummy_opts[] = {
	FUSE_OPT_END
};

int null_parser(void *state, const char *arg, int key, struct fuse_args *outargs)
{
	return 1;
}

int dummy_parse_opts(void *device, void *opt_args)
{
	struct dummy_data *state = (struct dummy_data *) device;
	struct fuse_args *args = (struct fuse_args *) opt_args;
	int ret;

	/* fuse_opt_parse can handle a NULL device parameter just fine */
	ret = fuse_opt_parse(args, state, dummy_opts, null_parser);
	if (ret < 0)
		return ret;

	return DEVICE_GOOD;
}

void dummy_help_message(const char *progname)
{
	return;
}

int dummy_open(const char *name, void **handle)
{
	struct dummy_data *state;

	state = (struct dummy_data *)calloc(1,sizeof(struct dummy_data));
	if (!state) {
		return -EDEV_NO_MEMORY;
	}

	*handle = (void *) state;
	return DEVICE_GOOD;
}

int dummy_reopen(const char *name, void *device)
{
	/* Do nothing */
	return DEVICE_GOOD;
}

int dummy_close(void *device)
{
	struct dummy_data *state = (struct dummy_data *)device;

	if (state)
		free(state);

	return DEVICE_GOOD;
}

int dummy_close_raw(void *device)
{
	return DEVICE_GOOD;
}

int dummy_is_connected(const char *devname)
{
	return DEVICE_GOOD;
}

int dummy_inquiry(void *device, struct tc_inq *inq)
{
	return DEVICE_GOOD;
}

int dummy_inquiry_page(void *device, unsigned char page, struct tc_inq_page *inq)
{
	return DEVICE_GOOD;
}

int dummy_test_unit_ready(void *device)
{
	return DEVICE_GOOD;
}

int dummy_read(void *device, char *buf, size_t count, struct tc_position *pos,
	const bool unusual_size)
{
	return DEVICE_GOOD;
}

int dummy_write(void *device, const char *buf, size_t count, struct tc_position *pos)
{
	return DEVICE_GOOD;
}

int dummy_writefm(void *device, size_t count, struct tc_position *pos, bool immed)
{
	return DEVICE_GOOD;
}

int dummy_rewind(void *device, struct tc_position *pos)
{
	return DEVICE_GOOD;
}

int dummy_locate(void *device, struct tc_position dest, struct tc_position *pos)
{
	return DEVICE_GOOD;
}

int dummy_space(void *device, size_t count, TC_SPACE_TYPE type, struct tc_position *pos)
{
	return DEVICE_GOOD;
}

int dummy_erase(void *device, struct tc_position *pos, bool long_erase)
{
	return DEVICE_GOOD;
}

int dummy_load(void *device, struct tc_position *pos)
{
	return DEVICE_GOOD;
}

int dummy_unload(void *device, struct tc_position *pos)
{
	return DEVICE_GOOD;
}

int dummy_readpos(void *device, struct tc_position *pos)
{
	return DEVICE_GOOD;
}

int dummy_setcap(void *device, uint16_t proportion)
{
	return DEVICE_GOOD;
}

int dummy_format(void *device, TC_FORMAT_TYPE format, const char *vol_name, const char *barcode_name, const char *vol_mam_uuid)
{
	return DEVICE_GOOD;
}

int dummy_remaining_capacity(void *device, struct tc_remaining_cap *cap)
{
	return DEVICE_GOOD;
}

int dummy_get_cartridge_health(void *device, struct tc_cartridge_health *cart_health)
{
	return DEVICE_GOOD;
}

int dummy_get_tape_alert(void *device, uint64_t *tape_alert)
{
	return DEVICE_GOOD;
}

int dummy_clear_tape_alert(void *device, uint64_t tape_alert)
{
	return DEVICE_GOOD;
}

int dummy_get_xattr(void *device, const char *name, char **buf)
{
	return DEVICE_GOOD;
}

int dummy_set_xattr(void *device, const char *name, const char *buf, size_t size)
{
	return DEVICE_GOOD;
}

int dummy_logsense(void *device, const uint8_t page, unsigned char *buf, const size_t size)
{
	return DEVICE_GOOD;
}

int dummy_modesense(void *device, const uint8_t page, const TC_MP_PC_TYPE pc, const uint8_t subpage, unsigned char *buf, const size_t size)
{
	return DEVICE_GOOD;
}

int dummy_modeselect(void *device, unsigned char *buf, const size_t size)
{
	return DEVICE_GOOD;
}

int dummy_reserve_unit(void *device)
{
	return DEVICE_GOOD;
}

int dummy_release_unit(void *device)
{
	return DEVICE_GOOD;
}

int dummy_prevent_medium_removal(void *device)
{
	return DEVICE_GOOD;
}

int dummy_allow_medium_removal(void *device)
{
	return DEVICE_GOOD;
}

int dummy_read_attribute(void *device, const tape_partition_t part, const uint16_t id
							 , unsigned char *buf, const size_t size)
{
	return DEVICE_GOOD;
}

int dummy_write_attribute(void *device, const tape_partition_t part
							  , const unsigned char *buf, const size_t size)
{
	return DEVICE_GOOD;
}

int dummy_allow_overwrite(void *device, const struct tc_position pos)
{
	return DEVICE_GOOD;
}

int dummy_get_eod_status(void *device, int partition)
{
	return EOD_GOOD;
}

int dummy_set_compression(void *device, bool enable_compression, struct tc_position *pos)
{
	return DEVICE_GOOD;
}

int dummy_set_default(void *device)
{
	return DEVICE_GOOD;
}

int dummy_get_parameters(void *device, struct tc_drive_param *params)
{
	return DEVICE_GOOD;
}

const char *dummy_default_device_name(void)
{
	return "DUMMY000";
}

int dummy_get_device_list(struct tc_drive_info *buf, int count)
{
	return DEVICE_GOOD;
}

int dummy_set_key(void *device, const unsigned char *keyalias, const unsigned char *key)
{
	return -EDEV_UNSUPPORTED_FUNCTION;
}

int dummy_get_keyalias(void *device, unsigned char **keyalias)
{
	return -EDEV_UNSUPPORTED_FUNCTION;
}

int dummy_takedump_drive(void *device, bool nonforced_dump)
{
	/* Do nothing */
	return DEVICE_GOOD;
}

int dummy_is_mountable(void *device, const char *barcode, const unsigned char cart_type,
							const unsigned char density)
{
	return MEDIUM_CANNOT_ACCESS;
}

bool dummy_is_readonly(void *device)
{
	return false;
}

int dummy_get_worm_status(void *device, bool *is_worm)
{
	return DEVICE_GOOD;
}

int dummy_get_serialnumber(void *device, char **result)
{
	return DEVICE_GOOD;
}

static int dummy_get_info(void *device, struct tc_drive_info *info)
{
	return DEVICE_GOOD;
}

int dummy_set_profiler(void *device, char *work_dir, bool enable)
{
	/* Do nohting: file backend does not support profiler */
	return DEVICE_GOOD;
}

int dummy_get_block_in_buffer(void *device, unsigned int *block)
{
	return DEVICE_GOOD;
}

struct tape_ops dummy_handler = {
	.open                   = dummy_open,
	.reopen                 = dummy_reopen,
	.close                  = dummy_close,
	.close_raw              = dummy_close_raw,
	.is_connected           = dummy_is_connected,
	.inquiry                = dummy_inquiry,
	.inquiry_page           = dummy_inquiry_page,
	.test_unit_ready        = dummy_test_unit_ready,
	.read                   = dummy_read,
	.write                  = dummy_write,
	.writefm                = dummy_writefm,
	.rewind                 = dummy_rewind,
	.locate                 = dummy_locate,
	.space                  = dummy_space,
	.erase                  = dummy_erase,
	.load                   = dummy_load,
	.unload                 = dummy_unload,
	.readpos                = dummy_readpos,
	.setcap                 = dummy_setcap,
	.format                 = dummy_format,
	.remaining_capacity     = dummy_remaining_capacity,
	.logsense               = dummy_logsense,
	.modesense              = dummy_modesense,
	.modeselect             = dummy_modeselect,
	.reserve_unit           = dummy_reserve_unit,
	.release_unit           = dummy_release_unit,
	.prevent_medium_removal = dummy_prevent_medium_removal,
	.allow_medium_removal   = dummy_allow_medium_removal,
	.write_attribute        = dummy_write_attribute,
	.read_attribute         = dummy_read_attribute,
	.allow_overwrite        = dummy_allow_overwrite,
	.set_compression        = dummy_set_compression,
	.set_default            = dummy_set_default,
	.get_cartridge_health   = dummy_get_cartridge_health,
	.get_tape_alert         = dummy_get_tape_alert,
	.clear_tape_alert       = dummy_clear_tape_alert,
	.get_xattr              = dummy_get_xattr,
	.set_xattr              = dummy_set_xattr,
	.get_parameters         = dummy_get_parameters,
	.get_eod_status         = dummy_get_eod_status,
	.get_device_list        = dummy_get_device_list,
	.help_message           = dummy_help_message,
	.parse_opts             = dummy_parse_opts,
	.default_device_name    = dummy_default_device_name,
	.set_key                = dummy_set_key,
	.get_keyalias           = dummy_get_keyalias,
	.takedump_drive         = dummy_takedump_drive,
	.is_mountable           = dummy_is_mountable,
	.get_worm_status        = dummy_get_worm_status,
	.get_serialnumber       = dummy_get_serialnumber,
	.get_info               = dummy_get_info,
	.set_profiler           = dummy_set_profiler,
	.get_block_in_buffer    = dummy_get_block_in_buffer,
	.is_readonly            = dummy_is_readonly,
};

struct tape_ops *tape_dev_get_ops(void)
{
	return &dummy_handler;
}

#ifndef mingw_PLATFORM
extern char tape_generic_dummy_dat[];
#endif

const char *tape_dev_get_message_bundle_name(void **message_data)
{
#ifndef mingw_PLATFORM
	*message_data = tape_generic_dummy_dat;
#else
	*message_data = NULL;
#endif
	return "tape_generic_dummy";
}
