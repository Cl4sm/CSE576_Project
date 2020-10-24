int
get_asymmetric_access_state(int fd, unsigned int tpg)
{
	unsigned char		*buf;
	struct rtpg_data *	tpgd;
	struct rtpg_tpg_dscr *	dscr;
	int			rc;
	int			buflen;
	uint32_t		scsi_buflen;

	buflen = 128; /* Initial value from old code */
	buf = (unsigned char *)malloc(buflen);
	if (!buf) {
		PRINT_DEBUG ("malloc failed: could not allocate"
			"%u bytes\n", buflen);
		return -RTPG_RTPG_FAILED;
	}
	memset(buf, 0, buflen);
	rc = do_rtpg(fd, buf, buflen);
	if (rc < 0)
		goto out;
	scsi_buflen = (buf[0] << 24 | buf[1] << 16 | buf[2] << 8 | buf[3]) + 4;
	if (buflen < scsi_buflen) {
		free(buf);
		buf = (unsigned char *)malloc(scsi_buflen);
		if (!buf) {
			PRINT_DEBUG ("malloc failed: could not allocate"
				"%u bytes\n", scsi_buflen);
			return -RTPG_RTPG_FAILED;
		}
		buflen = scsi_buflen;
		memset(buf, 0, buflen);
		rc = do_rtpg(fd, buf, buflen);
		if (rc < 0)
			goto out;
	}

	tpgd = (struct rtpg_data *) buf;
	rc   = -RTPG_TPG_NOT_FOUND;
	RTPG_FOR_EACH_PORT_GROUP(tpgd, dscr) {
		if (get_uint16(dscr->tpg) == tpg) {
			if (rc != -RTPG_TPG_NOT_FOUND) {
				PRINT_DEBUG("get_asymmetric_access_state: "
					"more than one entry with same port "
					"group.\n");
			} else {
				PRINT_DEBUG("pref=%i\n", dscr->b0);
				rc = rtpg_tpg_dscr_get_aas(dscr);
			}
		}
	}
out:
	free(buf);
	return rc;
}
