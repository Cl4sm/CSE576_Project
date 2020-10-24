int hds_modular_prio (const char *dev, int fd)
{
	int k;
	char vendor[9];
	char product[32];
	char serial[32];
	char ldev[32];
	char ctrl[32];
	char port[32];
	unsigned char inqCmdBlk[INQ_CMD_LEN] = { INQ_CMD_CODE, 0, 0, 0, INQ_REPLY_LEN, 0 };
	unsigned char inqBuff[INQ_REPLY_LEN];
	unsigned char *inqBuffp = inqBuff;
	unsigned char sense_buffer[32];
	sg_io_hdr_t io_hdr;

	if ((ioctl (fd, SG_GET_VERSION_NUM, &k) < 0) || (k < 30000)) {
		pp_hds_log(0, "can't use SG ioctl interface");
		return -1;
	}

	memset (&io_hdr, 0, sizeof (sg_io_hdr_t));
	memset (inqBuff, 0, INQ_REPLY_LEN);
	io_hdr.interface_id = 'S';
	io_hdr.cmd_len = sizeof (inqCmdBlk);
	io_hdr.mx_sb_len = sizeof (sense_buffer);
	io_hdr.dxfer_direction = SG_DXFER_FROM_DEV;
	io_hdr.dxfer_len = INQ_REPLY_LEN;
	io_hdr.dxferp = inqBuff;
	io_hdr.cmdp = inqCmdBlk;
	io_hdr.sbp = sense_buffer;
	io_hdr.timeout = 2000;	/* TimeOut = 2 seconds */

	if (ioctl (fd, SG_IO, &io_hdr) < 0) {
		pp_hds_log(0, "SG_IO error");
		return -1;
	}
	if ((io_hdr.info & SG_INFO_OK_MASK) != SG_INFO_OK) {
		pp_hds_log(0, "SCSI error");
		return -1;
	}

	snprintf (vendor, 9, "%.8s", inqBuffp + 8);
	snprintf (product, 17, "%.16s", inqBuffp + 16);
	snprintf (serial, 5, "%.4s", inqBuffp + 40);
	snprintf (ldev, 5, "%.4s", inqBuffp + 44);
	snprintf (ctrl, 2, "%.1s", inqBuffp + 49);
	snprintf (port, 2, "%.1s", inqBuffp + 50);

	pp_hds_log(4, "VENDOR:  %s", vendor);
	pp_hds_log(4, "PRODUCT: %s", product);
	pp_hds_log(4, "SERIAL:  0x%s", serial);
	pp_hds_log(4, "LDEV:    0x%s", ldev);
	pp_hds_log(4, "CTRL:    %s", ctrl);
	pp_hds_log(4, "PORT:    %s", port);

	switch (ctrl[0]) {
	case '0': case '2': case '4': case '6': case '8':
		switch (ldev[3]) {
		case '0': case '2': case '4': case '6': case '8': case 'A': case 'C': case 'E':
			pp_hds_log(4, "CTRL EVEN, LDEV EVEN, PRIO 1");
			return 1;
			break;
		case '1': case '3': case '5': case '7': case '9': case 'B': case 'D': case 'F':
			pp_hds_log(4, "CTRL EVEN, LDEV ODD, PRIO 0");
			return 0;
			break;
		}
	case '1': case '3': case '5': case '7': case '9':
		switch (ldev[3]) {
		case '0': case '2': case '4': case '6': case '8': case 'A': case 'C': case 'E':
			pp_hds_log(4, "CTRL ODD, LDEV EVEN, PRIO 0");
			return 0;
			break;
		case '1': case '3': case '5': case '7': case '9': case 'B': case 'D': case 'F':
			pp_hds_log(4, "CTRL ODD, LDEV ODD, PRIO 1");
			return 1;
			break;
		}
	}
	return -1;
}
