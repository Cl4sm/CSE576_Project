scsi_error(struct sg_io_hdr *hdr)
{
	/* Treat SG_ERR here to get rid of sg_err.[ch] */
	hdr->status &= 0x7e;

	if (
		(hdr->status == 0)        &&
		(hdr->host_status == 0)   &&
		(hdr->driver_status == 0)
	) {
		return 0;
	}

	if (
		(hdr->status == SCSI_CHECK_CONDITION)    ||
		(hdr->status == SCSI_COMMAND_TERMINATED) ||
		((hdr->driver_status & 0xf) == SG_ERR_DRIVER_SENSE)
	) {
		if (hdr->sbp && (hdr->sb_len_wr > 2)) {
			int		sense_key;
			unsigned char *	sense_buffer = hdr->sbp;

			if (sense_buffer[0] & 0x2)
				sense_key = sense_buffer[1] & 0xf;
			else
				sense_key = sense_buffer[2] & 0xf;

			if (sense_key == RECOVERED_ERROR)
				return 0;
		}
	}

	return 1;
}
