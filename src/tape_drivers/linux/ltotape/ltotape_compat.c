static int ltotape_wrapper_get_parameters (void *, struct tc_drive_param *);

static inline void
ltotape_wrapper_copy_device(ltotape_scsi_io_type *dst,
			    ltotape_scsi_io_type *src)
{
	memcpy(dst, src, sizeof(*dst));
	return;
}

static int
_ltotape_takedump_drive (void *device, bool capture_unforced)
{
	ltotape_scsi_io_type sio;
	int status;

	ltotape_wrapper_copy_device(&sio, (ltotape_scsi_io_type *)device);
        status = ltotape_takedump_drive(device);
	ltotape_wrapper_copy_device((ltotape_scsi_io_type *)device, &sio);

	return status;
}

static int
_ltotape_is_mountable(void *device, const char *barcode,
		      const unsigned char cart_type,
		      const unsigned char density_code)
{
        return 1;
}

static int
ltotape_get_serialnumber(void *device, char **result)
{
	ltotape_scsi_io_type    *sio = (ltotape_scsi_io_type *) device;

	CHECK_ARG_NULL(device, -LTFS_NULL_ARG);
	CHECK_ARG_NULL(result, -LTFS_NULL_ARG);

	*result = strdup((const char *) sio->serialno);
	if (! *result) {
		ltfsmsg(LTFS_ERR, "10001E", "ltotape_get_serialnumber: result");
		return -EDEV_NO_MEMORY;
	}

	return 0;
}

static int
ltotape_set_profiler(void *device, char *work_dir, bool enable)
{
	printf("uninmplemente\n");
	abort();
	return 0;
}

static int
ltotape_get_block_in_buffer(void *device, uint32_t *block)
{
	ltotape_scsi_io_type sio;
	int status = 0;
	unsigned char buf[REDPOS_EXT_LEN];

	ltotape_wrapper_copy_device(&sio, (ltotape_scsi_io_type *)device);
	memset(buf, 0, sizeof(buf));

	/* Build CDB */
	sio.cdb[0] = READ_POSITION;
	sio.cdb[1] = 0x08; /* Extended Format */
	sio.cdb_length = 6;

	/* Set up the data part: */
	sio.data = buf;
	sio.data_length = sizeof(buf);
	sio.data_direction = HOST_READ;

	sio.timeout_ms = (sio.family == drivefamily_lto) ?
			LTO_READ_TIMEOUT : DAT_READ_TIMEOUT;

	status = ltotape_scsiexec(&sio);

	if (status == 0) {
		*block = (buf[5] << 16) + (buf[6] << 8) + (int)buf[7];

		ltfsmsg(LTFS_DEBUG, "30398D", "blocks-in-buffer",
			(unsigned long long)*block,
			(unsigned long long)0,
			(unsigned long long)0, sio.serialno);
	}

	ltotape_wrapper_copy_device((ltotape_scsi_io_type *)device, &sio);
	return status;
}


/*
 * We cache the value un a static but does not cause thread-unsafety
 * because the first call is done at a time when ltfs in not yet
 * multithreaded.
 */
static bool
ltotape_is_readonly(void *device)
{
	static bool cached = false;
	static bool is_read_only = false;
	bool status;
	struct tc_drive_param drive_param;

	if (!cached) {
		status = ltotape_wrapper_get_parameters(device, &drive_param);
		if (status < 0)
			is_read_only = true;
		else
			is_read_only = drive_param.write_protect;

		cached = true;
	}

	/*
	 * TODO: Need to return true when the tape and the drive type
	 * mismatch (e.g.: LTO5 tape in LTO7 drive)
	 */
	return is_read_only;
}


static int
ltotape_wrapper_close_raw (void *device)
{
        ltotape_scsi_io_type sio;
        int status;

        ltotape_wrapper_copy_device(&sio, (ltotape_scsi_io_type *)device);
        status = ltotape_close_raw(&sio);
        ltotape_wrapper_copy_device((ltotape_scsi_io_type *)device, &sio);

        return status;
}

static int
ltotape_wrapper_inquiry (void *device, struct tc_inq *inq)  {
        ltotape_scsi_io_type sio;
        int status;

        ltotape_wrapper_copy_device(&sio, (ltotape_scsi_io_type *)device);
        status = ltotape_inquiry(&sio, inq);
        ltotape_wrapper_copy_device((ltotape_scsi_io_type *)device, &sio);

        return status;
}

static int
ltotape_wrapper_inquiry_page (void *device, unsigned char page,
			      struct tc_inq_page *inq)
{
        ltotape_scsi_io_type sio;
        int status;

        ltotape_wrapper_copy_device(&sio, (ltotape_scsi_io_type *)device);
        status = ltotape_inquiry_page(&sio, page, inq);
        ltotape_wrapper_copy_device((ltotape_scsi_io_type *)device, &sio);

        return status;
}

static int
ltotape_wrapper_test_unit_ready (void *device)
{
        ltotape_scsi_io_type sio;
        int status;

        ltotape_wrapper_copy_device(&sio, (ltotape_scsi_io_type *)device);
        status = ltotape_test_unit_ready(&sio);
        ltotape_wrapper_copy_device((ltotape_scsi_io_type *)device, &sio);

        return status;
}

static int
ltotape_wrapper_read (void *device, char *buf, size_t count,
		      struct tc_position *pos, const bool unusual_size)
{
        ltotape_scsi_io_type sio;
        int status;

        ltotape_wrapper_copy_device(&sio, (ltotape_scsi_io_type *)device);
        status = ltotape_read(&sio, buf, count, pos, unusual_size);
        ltotape_wrapper_copy_device((ltotape_scsi_io_type *)device, &sio);

        return status;
}

static int
ltotape_wrapper_write (void *device, const char *buf, size_t count,
		       struct tc_position *pos)
{
        ltotape_scsi_io_type sio;
        int status;

        ltotape_wrapper_copy_device(&sio, (ltotape_scsi_io_type *)device);
        status = ltotape_write(&sio, buf, count, pos);
        ltotape_wrapper_copy_device((ltotape_scsi_io_type *)device, &sio);

        return status;
}

static int
ltotape_wrapper_writefm (void *device, size_t count,
			 struct tc_position *pos, bool immed)
{
        ltotape_scsi_io_type sio;
        int status;

        ltotape_wrapper_copy_device(&sio, (ltotape_scsi_io_type *)device);
        status = ltotape_writefm(&sio, count, pos, immed);
        ltotape_wrapper_copy_device((ltotape_scsi_io_type *)device, &sio);

        return status;
}

static int
ltotape_wrapper_rewind (void *device, struct tc_position *pos)
{
        ltotape_scsi_io_type sio;
        int status;

        ltotape_wrapper_copy_device(&sio, (ltotape_scsi_io_type *)device);
        status = ltotape_rewind(&sio, pos);
        ltotape_wrapper_copy_device((ltotape_scsi_io_type *)device, &sio);

        return status;
}

static int
ltotape_wrapper_locate (void *device, struct tc_position dest,
			struct tc_position *pos)
{
        ltotape_scsi_io_type sio;
        int status;

        ltotape_wrapper_copy_device(&sio, (ltotape_scsi_io_type *)device);
        status = ltotape_locate(&sio, dest, pos);
        ltotape_wrapper_copy_device((ltotape_scsi_io_type *)device, &sio);

        return status;
}

static int
ltotape_wrapper_space (void *device, size_t count,
		       TC_SPACE_TYPE type, struct tc_position *pos)
{
        ltotape_scsi_io_type sio;
        int status;

        ltotape_wrapper_copy_device(&sio, (ltotape_scsi_io_type *)device);
        status = ltotape_space(&sio, count, type, pos);
        ltotape_wrapper_copy_device((ltotape_scsi_io_type *)device, &sio);

        return status;
}

static int
ltotape_wrapper_erase (void *device, struct tc_position *pos,
		       bool long_erase)
{
        ltotape_scsi_io_type sio;
        int status;

        ltotape_wrapper_copy_device(&sio, (ltotape_scsi_io_type *)device);
        status = ltotape_erase(&sio, pos, long_erase);
        ltotape_wrapper_copy_device((ltotape_scsi_io_type *)device, &sio);

        return status;
}

static int
ltotape_wrapper_load (void *device, struct tc_position *pos)
{
        ltotape_scsi_io_type sio;
        int status;

        ltotape_wrapper_copy_device(&sio, (ltotape_scsi_io_type *)device);
        status = ltotape_load(&sio, pos);
        ltotape_wrapper_copy_device((ltotape_scsi_io_type *)device, &sio);

        return status;
}

static int
ltotape_wrapper_unload(void *device, struct tc_position *pos)
{
        ltotape_scsi_io_type sio;
        int status;

        ltotape_wrapper_copy_device(&sio, (ltotape_scsi_io_type *)device);
        status = ltotape_unload(&sio, pos);
        ltotape_wrapper_copy_device((ltotape_scsi_io_type *)device, &sio);

        return status;
}

static int
ltotape_wrapper_readposition (void *device, struct tc_position *pos)
{
        ltotape_scsi_io_type sio;
        int status;

        ltotape_wrapper_copy_device(&sio, (ltotape_scsi_io_type *)device);
        status = ltotape_readposition(&sio, pos);
        ltotape_wrapper_copy_device((ltotape_scsi_io_type *)device, &sio);

        return status;
}

static int
ltotape_wrapper_setcap (void *device, uint16_t proportion)
{
        ltotape_scsi_io_type sio;
        int status;

        ltotape_wrapper_copy_device(&sio, (ltotape_scsi_io_type *)device);
        status = ltotape_setcap(&sio, proportion);
        ltotape_wrapper_copy_device((ltotape_scsi_io_type *)device, &sio);

        return status;
}

static int
ltotape_wrapper_format (void *device, TC_FORMAT_TYPE format,
			const char *vol_name, const char *barcode_name,
			const char *vol_mam_uuid)
{
        ltotape_scsi_io_type sio;
        int status;

        ltotape_wrapper_copy_device(&sio, (ltotape_scsi_io_type *)device);
        status = ltotape_format(&sio, format, vol_name, barcode_name,
				vol_mam_uuid);
        ltotape_wrapper_copy_device((ltotape_scsi_io_type *)device, &sio);

        return status;
}

static int
ltotape_wrapper_remaining_capacity (void *device,
				    struct tc_remaining_cap *cap)
{
        ltotape_scsi_io_type sio;
        int status;

        ltotape_wrapper_copy_device(&sio, (ltotape_scsi_io_type *)device);
        status = ltotape_remaining_capacity(&sio, cap);
        ltotape_wrapper_copy_device((ltotape_scsi_io_type *)device, &sio);

        return status;
}

static int
ltotape_wrapper_logsense (void *device, const uint8_t page,
			  unsigned char *buf, const size_t size)
{
        ltotape_scsi_io_type sio;
        int status;

        ltotape_wrapper_copy_device(&sio, (ltotape_scsi_io_type *)device);
        status = ltotape_logsense(&sio, page, buf, size);
        ltotape_wrapper_copy_device((ltotape_scsi_io_type *)device, &sio);

        return status;
}

static int
ltotape_wrapper_modesense (void *device, const uint8_t page,
			   const TC_MP_PC_TYPE pc, const uint8_t subpage,
			   unsigned char *buf, const size_t size)
{
        ltotape_scsi_io_type sio;
        int status;

        ltotape_wrapper_copy_device(&sio, (ltotape_scsi_io_type *)device);
        status = ltotape_modesense(&sio, page, pc, subpage, buf, size);
        ltotape_wrapper_copy_device((ltotape_scsi_io_type *)device, &sio);

        return status;
}

static int
ltotape_wrapper_modeselect (void *device, unsigned char *buf,
			    const size_t size)
{
        ltotape_scsi_io_type sio;
        int status;

        ltotape_wrapper_copy_device(&sio, (ltotape_scsi_io_type *)device);
        status = ltotape_modeselect (&sio, buf, size);
        ltotape_wrapper_copy_device((ltotape_scsi_io_type *)device, &sio);

        return status;
}

static int
ltotape_wrapper_reserve_unit (void *device)
{
        ltotape_scsi_io_type sio;
        int status;

        ltotape_wrapper_copy_device(&sio, (ltotape_scsi_io_type *)device);
        status = ltotape_reserve_unit (&sio);
        ltotape_wrapper_copy_device((ltotape_scsi_io_type *)device, &sio);

        return status;
}

static int
ltotape_wrapper_release_unit (void *device)
{
        ltotape_scsi_io_type sio;
        int status;

        ltotape_wrapper_copy_device(&sio, (ltotape_scsi_io_type *)device);
        status = ltotape_release_unit (&sio);
        ltotape_wrapper_copy_device((ltotape_scsi_io_type *)device, &sio);

        return status;
}

static int
ltotape_wrapper_prevent_medium_removal (void *device)
{
        ltotape_scsi_io_type sio;
        int status;

        ltotape_wrapper_copy_device(&sio, (ltotape_scsi_io_type *)device);
        status = ltotape_prevent_medium_removal (&sio);
        ltotape_wrapper_copy_device((ltotape_scsi_io_type *)device, &sio);

        return status;
}

static int
ltotape_wrapper_allow_medium_removal (void *device)
{
        ltotape_scsi_io_type sio;
        int status;

        ltotape_wrapper_copy_device(&sio, (ltotape_scsi_io_type *)device);
        status = ltotape_allow_medium_removal(&sio);
        ltotape_wrapper_copy_device((ltotape_scsi_io_type *)device, &sio);

        return status;
}

static int
ltotape_wrapper_read_attribute (void *device,
				const tape_partition_t part,
				const uint16_t id, unsigned char *buf,
				const size_t size)
{
        ltotape_scsi_io_type sio;
        int status;

        ltotape_wrapper_copy_device(&sio, (ltotape_scsi_io_type *)device);
        status = ltotape_read_attribute(&sio, part, id, buf, size);
        ltotape_wrapper_copy_device((ltotape_scsi_io_type *)device, &sio);

        return status;
}

static int
ltotape_wrapper_write_attribute (void *device, const tape_partition_t part,
				 const unsigned char *buf,
				 const size_t size)
{
        ltotape_scsi_io_type sio;
        int status;

        ltotape_wrapper_copy_device(&sio, (ltotape_scsi_io_type *)device);
        status = ltotape_write_attribute(&sio, part, buf, size);
        ltotape_wrapper_copy_device((ltotape_scsi_io_type *)device, &sio);

        return status;
}

static int
ltotape_wrapper_allow_overwrite (void *device, const struct tc_position pos)
{
        ltotape_scsi_io_type sio;
        int status;

        ltotape_wrapper_copy_device(&sio, (ltotape_scsi_io_type *)device);
        status = ltotape_allow_overwrite(&sio, pos);
        ltotape_wrapper_copy_device((ltotape_scsi_io_type *)device, &sio);

        return status;
}

static int
ltotape_wrapper_set_compression (void *device,
				 const bool enable_compression,
				 struct tc_position *pos)
{
        ltotape_scsi_io_type sio;
        int status;

        ltotape_wrapper_copy_device(&sio, (ltotape_scsi_io_type *)device);
        status = ltotape_set_compression(&sio, enable_compression, pos);
        ltotape_wrapper_copy_device((ltotape_scsi_io_type *)device, &sio);

        return status;
}

static int
ltotape_wrapper_set_default (void *device)
{
        ltotape_scsi_io_type sio;
        int status;

        ltotape_wrapper_copy_device(&sio, (ltotape_scsi_io_type *)device);
        status = ltotape_set_default(&sio);
        ltotape_wrapper_copy_device((ltotape_scsi_io_type *)device, &sio);

        return status;
}

static int
ltotape_wrapper_get_cartridge_health (void *device,
				      struct tc_cartridge_health *cart_health)
{
        ltotape_scsi_io_type sio;
        int status;

        ltotape_wrapper_copy_device(&sio, (ltotape_scsi_io_type *)device);
        status = ltotape_get_cartridge_health(&sio, cart_health);
        ltotape_wrapper_copy_device((ltotape_scsi_io_type *)device, &sio);

        return status;
}

static int
ltotape_wrapper_get_tape_alert (void *device, uint64_t* taflags)
{
        ltotape_scsi_io_type sio;
        int status;

        ltotape_wrapper_copy_device(&sio, (ltotape_scsi_io_type *)device);
        status = ltotape_get_tape_alert(&sio, taflags);
        ltotape_wrapper_copy_device((ltotape_scsi_io_type *)device, &sio);

        return status;
}

static int
ltotape_wrapper_clear_tape_alert (void *device, uint64_t tape_alert)
{
        ltotape_scsi_io_type sio;
        int status;

        ltotape_wrapper_copy_device(&sio, (ltotape_scsi_io_type *)device);
        status = ltotape_clear_tape_alert(&sio, tape_alert);
        ltotape_wrapper_copy_device((ltotape_scsi_io_type *)device, &sio);

        return status;
}

static int
ltotape_wrapper_get_xattr (void *device, const char *name, char **buf)
{
        ltotape_scsi_io_type sio;
        int status;

        ltotape_wrapper_copy_device(&sio, (ltotape_scsi_io_type *)device);
        status = ltotape_get_xattr(&sio, name, buf);
        ltotape_wrapper_copy_device((ltotape_scsi_io_type *)device, &sio);

        return status;
}

static int
ltotape_wrapper_set_xattr (void *device, const char *name,
			       const char *buf, size_t size)
{
        ltotape_scsi_io_type sio;
        int status;

        ltotape_wrapper_copy_device(&sio, (ltotape_scsi_io_type *)device);
        status = ltotape_set_xattr(&sio, name, buf, size);
        ltotape_wrapper_copy_device((ltotape_scsi_io_type *)device, &sio);

        return status;
}

static int
ltotape_wrapper_get_eod_status (void *device, int part)
{
        ltotape_scsi_io_type sio;
        int status;

        ltotape_wrapper_copy_device(&sio, (ltotape_scsi_io_type *)device);
        status = ltotape_get_eod_status(&sio, part);
        ltotape_wrapper_copy_device((ltotape_scsi_io_type *)device, &sio);

        return status;
}

static int
ltotape_wrapper_get_parameters (void *device,
				struct tc_drive_param *drive_param)
{
        ltotape_scsi_io_type sio;
        int status;

        ltotape_wrapper_copy_device(&sio, (ltotape_scsi_io_type *)device);
        status = ltotape_get_parameters(&sio, drive_param);
        ltotape_wrapper_copy_device((ltotape_scsi_io_type *)device, &sio);

        return status;
}

static int
ltotape_wrapper_parse_opts (void *device, void *opt_args)
{
        ltotape_scsi_io_type sio;
        int status;

        ltotape_wrapper_copy_device(&sio, (ltotape_scsi_io_type *)device);
        status = ltotape_parse_opts(&sio, opt_args);
        ltotape_wrapper_copy_device((ltotape_scsi_io_type *)device, &sio);

        return status;
}

static int
ltotape_wrapper_set_key(void *device, const unsigned char *keyalias,
			const unsigned char *key)
{
	ltotape_scsi_io_type sio;
	int status;

	ltotape_wrapper_copy_device(&sio, (ltotape_scsi_io_type *)device);
	status = ltotape_set_key(&sio, keyalias, key);
	ltotape_wrapper_copy_device((ltotape_scsi_io_type *)device, &sio);

	return status;
}

static int
ltotape_wrapper_get_keyalias (void *device, unsigned char **keyalias)
{
	ltotape_scsi_io_type sio;
	int status;

	ltotape_wrapper_copy_device(&sio, (ltotape_scsi_io_type *)device);
	status = ltotape_get_keyalias(&sio, keyalias);
	ltotape_wrapper_copy_device((ltotape_scsi_io_type *)device, &sio);

	return status;
}

static int
ltotape_wrapper_get_worm_status (void *device, bool *is_worm)
{
	ltotape_scsi_io_type sio;
	int status;

	ltotape_wrapper_copy_device(&sio, (ltotape_scsi_io_type *)device);
	status = ltotape_get_worm_status(&sio, is_worm);
	ltotape_wrapper_copy_device((ltotape_scsi_io_type *)device, &sio);

	return status;
}

struct tape_ops ltotape_drive_handler = {
        .open                   = ltotape_open,
        .reopen                 = ltotape_reopen,
        .close                  = ltotape_close,
        .close_raw              = ltotape_wrapper_close_raw,
        .is_connected           = ltotape_is_connected,
        .inquiry                = ltotape_wrapper_inquiry,
        .inquiry_page           = ltotape_wrapper_inquiry_page,
        .test_unit_ready        = ltotape_wrapper_test_unit_ready,
        .read                   = ltotape_wrapper_read,
        .write                  = ltotape_wrapper_write,
        .writefm                = ltotape_wrapper_writefm,
        .rewind                 = ltotape_wrapper_rewind,
        .locate                 = ltotape_wrapper_locate,
        .space                  = ltotape_wrapper_space,
        .erase                  = ltotape_wrapper_erase,
        .load                   = ltotape_wrapper_load,
        .unload                 = ltotape_wrapper_unload,
        .readpos                = ltotape_wrapper_readposition,
        .setcap                 = ltotape_wrapper_setcap,
        .format                 = ltotape_wrapper_format,
        .remaining_capacity     = ltotape_wrapper_remaining_capacity,
        .logsense               = ltotape_wrapper_logsense,
        .modesense              = ltotape_wrapper_modesense,
        .modeselect             = ltotape_wrapper_modeselect,
        .reserve_unit           = ltotape_wrapper_reserve_unit,
        .release_unit           = ltotape_wrapper_release_unit,
        .prevent_medium_removal = ltotape_wrapper_prevent_medium_removal,
        .allow_medium_removal   = ltotape_wrapper_allow_medium_removal,
        .read_attribute         = ltotape_wrapper_read_attribute,
        .write_attribute        = ltotape_wrapper_write_attribute,
        .allow_overwrite        = ltotape_wrapper_allow_overwrite,
        .set_compression        = ltotape_wrapper_set_compression,
        .set_default            = ltotape_wrapper_set_default,
        .get_cartridge_health   = ltotape_wrapper_get_cartridge_health,
        .get_tape_alert         = ltotape_wrapper_get_tape_alert,
        .clear_tape_alert       = ltotape_wrapper_clear_tape_alert,
        .get_xattr              = ltotape_wrapper_get_xattr,
        .set_xattr              = ltotape_wrapper_set_xattr,
        .get_eod_status         = ltotape_wrapper_get_eod_status,
        .get_parameters         = ltotape_wrapper_get_parameters,
        .get_device_list        = ltotape_get_device_list,
        .help_message           = ltotape_help_message,
        .parse_opts             = ltotape_wrapper_parse_opts,
        .default_device_name    = ltotape_default_device_name,
        .set_key                = ltotape_wrapper_set_key,
        .get_keyalias           = ltotape_wrapper_get_keyalias,
        .takedump_drive         = _ltotape_takedump_drive,
        .is_mountable           = _ltotape_is_mountable,
        .get_worm_status        = ltotape_wrapper_get_worm_status,
	.get_serialnumber       = ltotape_get_serialnumber,
	.set_profiler           = ltotape_set_profiler,
	.get_block_in_buffer    = ltotape_get_block_in_buffer,
	.is_readonly            = ltotape_is_readonly,
};
