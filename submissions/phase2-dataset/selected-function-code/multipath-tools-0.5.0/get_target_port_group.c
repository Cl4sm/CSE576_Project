get_target_port_group(int fd)
{
	unsigned char		*buf;
	struct vpd83_data *	vpd83;
	struct vpd83_dscr *	dscr;
	int			rc;
	int			buflen, scsi_buflen;

	buflen = 128; /* Lets start from 128 */
	buf = (unsigned char *)malloc(buflen);
	if (!buf) {
		PRINT_DEBUG("malloc failed: could not allocate"
			     "%u bytes\n", buflen);
		return -RTPG_RTPG_FAILED;
	}

	memset(buf, 0, buflen);
	rc = do_inquiry(fd, 1, 0x83, buf, buflen);
	if (rc < 0)
		goto out;

	scsi_buflen = (buf[2] << 8 | buf[3]) + 4;
	if (buflen < scsi_buflen) {
		free(buf);
		buf = (unsigned char *)malloc(scsi_buflen);
		if (!buf) {
			PRINT_DEBUG("malloc failed: could not allocate"
				     "%u bytes\n", scsi_buflen);
			return -RTPG_RTPG_FAILED;
		}
		buflen = scsi_buflen;
		memset(buf, 0, buflen);
		rc = do_inquiry(fd, 1, 0x83, buf, buflen);
		if (rc < 0)
			goto out;
	}

	vpd83 = (struct vpd83_data *) buf;
	rc = -RTPG_NO_TPG_IDENTIFIER;
	FOR_EACH_VPD83_DSCR(vpd83, dscr) {
		if (vpd83_dscr_istype(dscr, IDTYPE_TARGET_PORT_GROUP)) {
			struct vpd83_tpg_dscr *p;
			if (rc != -RTPG_NO_TPG_IDENTIFIER) {
				PRINT_DEBUG("get_target_port_group: more "
					    "than one TPG identifier found!\n");
				continue;
			}
			p  = (struct vpd83_tpg_dscr *)dscr->data;
			rc = get_uint16(p->tpg);
		}
	}

	if (rc == -RTPG_NO_TPG_IDENTIFIER) {
		PRINT_DEBUG("get_target_port_group: "
			    "no TPG identifier found!\n");
	}
out:
	free(buf);
	return rc;
}
