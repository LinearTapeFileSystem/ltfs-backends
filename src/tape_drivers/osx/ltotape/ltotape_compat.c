int _ltotape_takedump_drive (void *device, bool capture_unforced);
int ltotape_get_serialnumber(void *device, char **result);
int ltotape_set_profiler(void *device, char *work_dir, bool enable);
int ltotape_get_block_in_buffer(void *device, uint32_t *block);
bool _ltotape_is_readonly(void *device);

int _ltotape_takedump_drive (void *device, bool capture_unforced)
{ 
        return ltotape_takedump_drive(device);
}

int _ltotape_is_mountable(void *device, const char *barcode,
                const unsigned char cart_type, const unsigned char density_code)
{
        return 1;
}

int ltotape_get_serialnumber(void *device, char **result) 
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

int ltotape_set_profiler(void *device, char *work_dir, bool enable)
{
	printf("uninmplemente\n");
	abort();
	return 0;
}

int ltotape_get_block_in_buffer(void *device, uint32_t *block)
{
	int					     status = 0;
	ltotape_scsi_io_type    *sio = (ltotape_scsi_io_type *) device;
	unsigned char buf[REDPOS_EXT_LEN];

	memset(buf, 0, sizeof(buf));

	/* Build CDB */
	sio->cdb[0] = READ_POSITION;
	sio->cdb[1] = 0x08; /* Extended Format */
	sio->cdb_length = 6;

	/* Set up the data part: */
	sio->data = buf;
	sio->data_length = sizeof(buf);
	sio->data_direction = HOST_READ;

	sio->timeout_ms = (sio->family == drivefamily_lto) ?
			LTO_READ_TIMEOUT : DAT_READ_TIMEOUT;

	status = ltotape_scsiexec (sio);
       
	if (status == 0) {
		*block = (buf[5] << 16) + (buf[6] << 8) + (int)buf[7];

		ltfsmsg(LTFS_DEBUG, "30398D", "blocks-in-buffer",
				(unsigned long long)*block, (unsigned long long)
0, (unsigned long long)0, sio->serialno);
	}

	return status;
}

bool ltotape_is_readonly(void *device)
{
	bool status;
	struct tc_drive_param drive_param;


	status = ltotape_get_parameters(device, &drive_param);
	if (!status)
		return status;

	return drive_param.write_protect;
}
