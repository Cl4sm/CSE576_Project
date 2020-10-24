static int
do_tur (int fd, unsigned int timeout)
{
	unsigned char turCmdBlk[TUR_CMD_LEN] = { 0x00, 0, 0, 0, 0, 0 };
	struct sg_io_hdr io_hdr;
	unsigned char sense_buffer[32];

	memset(&io_hdr, 0, sizeof (struct sg_io_hdr));
	io_hdr.interface_id = 'S';
	io_hdr.cmd_len = sizeof (turCmdBlk);
	io_hdr.mx_sb_len = sizeof (sense_buffer);
	io_hdr.dxfer_direction = SG_DXFER_NONE;
	io_hdr.cmdp = turCmdBlk;
	io_hdr.sbp = sense_buffer;
	io_hdr.timeout = timeout * 1000;
	io_hdr.pack_id = 0;

	if (ioctl(fd, SG_IO, &io_hdr) < 0)
		return 1;

	if (io_hdr.info & SG_INFO_OK_MASK)
		return 1;

	return 0;
}
