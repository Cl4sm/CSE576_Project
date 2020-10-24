do_rtpg(int fd, void* resp, long resplen)
{
	struct rtpg_command	cmd;
	struct sg_io_hdr	hdr;
	unsigned char		sense[SENSE_BUFF_LEN];

	memset(&cmd, 0, sizeof(cmd));
	cmd.op			= OPERATION_CODE_RTPG;
	rtpg_command_set_service_action(&cmd);
	set_uint32(cmd.length, resplen);
	PRINT_HEX((unsigned char *) &cmd, sizeof(cmd));

	memset(&hdr, 0, sizeof(hdr));
	hdr.interface_id	= 'S';
	hdr.cmdp		= (unsigned char *) &cmd;
	hdr.cmd_len		= sizeof(cmd);
	hdr.dxfer_direction	= SG_DXFER_FROM_DEV;
	hdr.dxferp		= resp;
	hdr.dxfer_len		= resplen;
	hdr.mx_sb_len		= sizeof(sense);
	hdr.sbp			= sense;
	hdr.timeout		= DEF_TIMEOUT;

	if (ioctl(fd, SG_IO, &hdr) < 0)
		return -RTPG_RTPG_FAILED;

	if (scsi_error(&hdr)) {
		PRINT_DEBUG("do_rtpg: SCSI error!\n");
		return -RTPG_RTPG_FAILED;
	}
	PRINT_HEX(resp, resplen);

	return 0;
}
