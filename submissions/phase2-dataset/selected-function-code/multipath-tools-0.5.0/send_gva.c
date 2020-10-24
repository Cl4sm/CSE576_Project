		    unsigned char *results, int *results_size)
{
	unsigned char sb[128];
	unsigned char cdb[10] = {0xc0, 0, 0x1, 0xa, 0x98, 0xa,
				 pg, sizeof(sb), 0, 0};
	struct sg_io_hdr io_hdr;
	int ret = -1;

	memset(&io_hdr, 0, sizeof (struct sg_io_hdr));
	memset(results, 0, *results_size);
	io_hdr.interface_id = 'S';
	io_hdr.cmd_len = sizeof (cdb);
	io_hdr.mx_sb_len = sizeof (sb);
	io_hdr.dxfer_direction = SG_DXFER_FROM_DEV;
	io_hdr.dxfer_len = *results_size;
	io_hdr.dxferp = results;
	io_hdr.cmdp = cdb;
	io_hdr.sbp = sb;
	io_hdr.timeout = SG_TIMEOUT;
	io_hdr.pack_id = 0;
	if (ioctl(fd, SG_IO, &io_hdr) < 0) {
		pp_ontap_log(0, "SG_IO ioctl failed, errno=%d", errno);
		dump_cdb(cdb, sizeof(cdb));
		goto out;
	}
	if (io_hdr.info & SG_INFO_OK_MASK) {
		pp_ontap_log(0, "SCSI error");
		dump_cdb(cdb, sizeof(cdb));
		process_sg_error(&io_hdr);
		goto out;
	}

	if (results[4] != 0x0a || results[5] != 0x98 ||
	    results[6] != 0x0a ||results[7] != 0x01) {
		dump_cdb(cdb, sizeof(cdb));
		pp_ontap_log(0, "GVA return wrong format ");
		pp_ontap_log(0, "results[4-7] = 0x%02x 0x%02x 0x%02x 0x%02x",
			results[4], results[5], results[6], results[7]);
		goto out;
	}
	ret = 0;
 out:
	return(ret);
}
