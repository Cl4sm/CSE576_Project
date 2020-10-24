tur_check(int fd, unsigned int timeout, char *msg)
{
	struct sg_io_hdr io_hdr;
	unsigned char turCmdBlk[TUR_CMD_LEN] = { 0x00, 0, 0, 0, 0, 0 };
	unsigned char sense_buffer[32];
	int retry_tur = 5;

 retry:
	memset(&io_hdr, 0, sizeof (struct sg_io_hdr));
	memset(&sense_buffer, 0, 32);
	io_hdr.interface_id = 'S';
	io_hdr.cmd_len = sizeof (turCmdBlk);
	io_hdr.mx_sb_len = sizeof (sense_buffer);
	io_hdr.dxfer_direction = SG_DXFER_NONE;
	io_hdr.cmdp = turCmdBlk;
	io_hdr.sbp = sense_buffer;
	io_hdr.timeout = timeout * 1000;
	io_hdr.pack_id = 0;
	if (ioctl(fd, SG_IO, &io_hdr) < 0) {
		TUR_MSG(msg, MSG_TUR_DOWN);
		return PATH_DOWN;
	}
	if ((io_hdr.status & 0x7e) == 0x18) {
		/*
		 * SCSI-3 arrays might return
		 * reservation conflict on TUR
		 */
		TUR_MSG(msg, MSG_TUR_UP);
		return PATH_UP;
	}
	if (io_hdr.info & SG_INFO_OK_MASK) {
		int key = 0, asc, ascq;

		switch (io_hdr.host_status) {
		case DID_OK:
		case DID_NO_CONNECT:
		case DID_BAD_TARGET:
		case DID_ABORT:
		case DID_TRANSPORT_FAILFAST:
			break;
		default:
			/* Driver error, retry */
			if (--retry_tur)
				goto retry;
			break;
		}
		if (io_hdr.sb_len_wr > 3) {
			if (io_hdr.sbp[0] == 0x72 || io_hdr.sbp[0] == 0x73) {
				key = io_hdr.sbp[1] & 0x0f;
				asc = io_hdr.sbp[2];
				ascq = io_hdr.sbp[3];
			} else if (io_hdr.sb_len_wr > 13 &&
				   ((io_hdr.sbp[0] & 0x7f) == 0x70 ||
				    (io_hdr.sbp[0] & 0x7f) == 0x71)) {
				key = io_hdr.sbp[2] & 0x0f;
				asc = io_hdr.sbp[12];
				ascq = io_hdr.sbp[13];
			}
		}
		if (key == 0x6) {
			/* Unit Attention, retry */
			if (--retry_tur)
				goto retry;
		}
		else if (key == 0x2) {
			/* Not Ready */
			/* Note: Other ALUA states are either UP or DOWN */
			if( asc == 0x04 && ascq == 0x0b){
				/*
				 * LOGICAL UNIT NOT ACCESSIBLE,
				 * TARGET PORT IN STANDBY STATE
				 */
				TUR_MSG(msg, MSG_TUR_GHOST);
				return PATH_GHOST;
			}
		}
		TUR_MSG(msg, MSG_TUR_DOWN);
		return PATH_DOWN;
	}
	TUR_MSG(msg, MSG_TUR_UP);
	return PATH_UP;
}
