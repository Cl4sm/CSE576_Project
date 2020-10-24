static int get_proxy(const char *dev, int fd)
{
	unsigned char results[256];
	unsigned char sb[128];
	unsigned char cdb[INQUIRY_CMDLEN] = {INQUIRY_CMD, 1, 0xc1, 0,
						   sizeof(sb), 0};
	struct sg_io_hdr io_hdr;
	int ret = -1;

	memset(&results, 0, sizeof (results));
	memset(&io_hdr, 0, sizeof (struct sg_io_hdr));
	io_hdr.interface_id = 'S';
	io_hdr.cmd_len = sizeof (cdb);
	io_hdr.mx_sb_len = sizeof (sb);
	io_hdr.dxfer_direction = SG_DXFER_FROM_DEV;
	io_hdr.dxfer_len = sizeof (results);
	io_hdr.dxferp = results;
	io_hdr.cmdp = cdb;
	io_hdr.sbp = sb;
	io_hdr.timeout = SG_TIMEOUT;
	io_hdr.pack_id = 0;
	if (ioctl(fd, SG_IO, &io_hdr) < 0) {
		pp_ontap_log(0, "ioctl sending inquiry command failed, "
			"errno=%d", errno);
		dump_cdb(cdb, sizeof(cdb));
		goto out;
	}
	if (io_hdr.info & SG_INFO_OK_MASK) {
		pp_ontap_log(0, "SCSI error");
		dump_cdb(cdb, sizeof(cdb));
		process_sg_error(&io_hdr);
		goto out;
	}

	if (results[1] != 0xc1 || results[8] != 0x0a ||
	    results[9] != 0x98 || results[10] != 0x0a ||
	    results[11] != 0x0 || results[12] != 0xc1 ||
	    results[13] != 0x0) {
		pp_ontap_log(0,"proxy info page in unknown format - ");
		pp_ontap_log(0,"results[8-13]=0x%02x 0x%02x 0x%02x 0x%02x "
			"0x%02x 0x%02x",
			results[8], results[9], results[10],
			results[11], results[12], results[13]);
		dump_cdb(cdb, sizeof(cdb));
		goto out;
	}
	ret = (results[19] & 0x02) >> 1;

 out:
	return(ret);
}
