int
do_inquiry(int fd, int evpd, unsigned int codepage, void *resp, int resplen)
{
	struct inquiry_command	cmd;
	struct sg_io_hdr	hdr;
	unsigned char		sense[SENSE_BUFF_LEN];

	memset(&cmd, 0, sizeof(cmd));
	cmd.op = OPERATION_CODE_INQUIRY;
	if (evpd) {
		inquiry_command_set_evpd(&cmd);
		cmd.page = codepage;
	}
	set_uint16(cmd.length, resplen);
	PRINT_HEX((unsigned char *) &cmd, sizeof(cmd));

	memset(&hdr, 0, sizeof(hdr));
	hdr.interface_id	= 'S';
	hdr.cmdp		= (unsigned char *) &cmd;
	hdr.cmd_len		= sizeof(cmd);
	hdr.dxfer_direction	= SG_DXFER_FROM_DEV;
	hdr.dxferp		= resp;
	hdr.dxfer_len		= resplen;
	hdr.sbp			= sense;
	hdr.mx_sb_len		= sizeof(sense);
	hdr.timeout		= DEF_TIMEOUT;

	if (ioctl(fd, SG_IO, &hdr) < 0) {
		PRINT_DEBUG("do_inquiry: IOCTL failed!\n");
		return -RTPG_INQUIRY_FAILED;
	}

	if (scsi_error(&hdr)) {
		PRINT_DEBUG("do_inquiry: SCSI error!\n");
		return -RTPG_INQUIRY_FAILED;
	}
	PRINT_HEX((unsigned char *) resp, resplen);

	return 0;
}
