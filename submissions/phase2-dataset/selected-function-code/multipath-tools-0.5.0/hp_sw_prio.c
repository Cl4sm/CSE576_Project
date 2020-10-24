{
        unsigned char turCmdBlk[TUR_CMD_LEN] = { 0x00, 0, 0, 0, 0, 0 };
	unsigned char sb[128];
	struct sg_io_hdr io_hdr;
	int ret = HP_PATH_FAILED;

	memset(&io_hdr, 0, sizeof (struct sg_io_hdr));
	io_hdr.interface_id = 'S';
	io_hdr.cmd_len = sizeof (turCmdBlk);
	io_hdr.mx_sb_len = sizeof (sb);
	io_hdr.dxfer_direction = SG_DXFER_NONE;
	io_hdr.cmdp = turCmdBlk;
	io_hdr.sbp = sb;
	io_hdr.timeout = 60000;
	io_hdr.pack_id = 0;
 retry:
	if (ioctl(fd, SG_IO, &io_hdr) < 0) {
		pp_hp_sw_log(0, "sending tur command failed");
		goto out;
	}
        io_hdr.status &= 0x7e;
        if ((0 == io_hdr.status) && (0 == io_hdr.host_status) &&
            (0 == io_hdr.driver_status)) {
		/* Command completed normally, path is active */
                ret = HP_PATH_ACTIVE;
	}

        if ((SCSI_CHECK_CONDITION == io_hdr.status) ||
            (SCSI_COMMAND_TERMINATED == io_hdr.status) ||
            (SG_ERR_DRIVER_SENSE == (0xf & io_hdr.driver_status))) {
                if (io_hdr.sbp && (io_hdr.sb_len_wr > 2)) {
                        int sense_key, asc, asq;
                        unsigned char * sense_buffer = io_hdr.sbp;
                        if (sense_buffer[0] & 0x2) {
                                sense_key = sense_buffer[1] & 0xf;
				asc = sense_buffer[2];
				asq = sense_buffer[3];
			} else {
                                sense_key = sense_buffer[2] & 0xf;
				asc = sense_buffer[12];
				asq = sense_buffer[13];
			}
                        if(RECOVERED_ERROR == sense_key)
                                ret = HP_PATH_ACTIVE;
			if(NOT_READY == sense_key) {
				if (asc == 0x04 && asq == 0x02) {
					/* This is a standby path */
					ret = HP_PATH_STANDBY;
				}
			}
			if(UNIT_ATTENTION == sense_key) {
				if (asc == 0x29) {
					/* Retry for device reset */
					goto retry;
				}
			}
                }
        }
out:
	return(ret);
}
