const char *ib_get_mad_status_str(IN const ib_mad_t * const p_mad)
{
	static char line[512];
	uint32_t offset = 0;
	ib_net16_t status;
	boolean_t first = TRUE;

	line[offset] = '\0';

	status = (ib_net16_t) (p_mad->status & IB_SMP_STATUS_MASK);

	if (status == 0) {
		strcat(&line[offset], "IB_SUCCESS");
		return (line);
	}

	if (status & IB_MAD_STATUS_BUSY) {
		strcat(&line[offset], ib_mad_status_str_busy);
		offset += sizeof(ib_mad_status_str_busy);
	}
	if (status & IB_MAD_STATUS_REDIRECT) {
		if (!first) {
			strcat(&line[offset], generic_or_str);
			offset += sizeof(generic_or_str) - 1;
		}
		first = FALSE;
		strcat(&line[offset], ib_mad_status_str_redirect);
		offset += sizeof(ib_mad_status_str_redirect) - 1;
	}
	if ((status & IB_MAD_STATUS_INVALID_FIELD) ==
	    IB_MAD_STATUS_UNSUP_CLASS_VER) {
		if (!first) {
			strcat(&line[offset], generic_or_str);
			offset += sizeof(generic_or_str) - 1;
		}
		first = FALSE;
		strcat(&line[offset], ib_mad_status_str_unsup_class_ver);
		offset += sizeof(ib_mad_status_str_unsup_class_ver) - 1;
	}
	if ((status & IB_MAD_STATUS_INVALID_FIELD) ==
	    IB_MAD_STATUS_UNSUP_METHOD) {
		if (!first) {
			strcat(&line[offset], generic_or_str);
			offset += sizeof(generic_or_str) - 1;
		}
		first = FALSE;
		strcat(&line[offset], ib_mad_status_str_unsup_method);
		offset += sizeof(ib_mad_status_str_unsup_method) - 1;
	}
	if ((status & IB_MAD_STATUS_INVALID_FIELD) ==
	    IB_MAD_STATUS_UNSUP_METHOD_ATTR) {
		if (!first) {
			strcat(&line[offset], generic_or_str);
			offset += sizeof(generic_or_str) - 1;
		}
		first = FALSE;
		strcat(&line[offset], ib_mad_status_str_unsup_method_attr);
		offset += sizeof(ib_mad_status_str_unsup_method_attr) - 1;
	}
	if ((status & IB_MAD_STATUS_INVALID_FIELD) ==
	    IB_MAD_STATUS_INVALID_FIELD) {
		if (!first) {
			strcat(&line[offset], generic_or_str);
			offset += sizeof(generic_or_str) - 1;
		}
		first = FALSE;
		strcat(&line[offset], ib_mad_status_str_invalid_field);
		offset += sizeof(ib_mad_status_str_invalid_field) - 1;
	}
	if ((status & IB_MAD_STATUS_CLASS_MASK) ==
	    IB_SA_MAD_STATUS_NO_RESOURCES) {
		if (!first) {
			strcat(&line[offset], generic_or_str);
			offset += sizeof(generic_or_str) - 1;
		}
		first = FALSE;
		strcat(&line[offset], ib_mad_status_str_no_resources);
		offset += sizeof(ib_mad_status_str_no_resources) - 1;
	}
	if ((status & IB_MAD_STATUS_CLASS_MASK) == IB_SA_MAD_STATUS_REQ_INVALID) {
		if (!first) {
			strcat(&line[offset], generic_or_str);
			offset += sizeof(generic_or_str) - 1;
		}
		first = FALSE;
		strcat(&line[offset], ib_mad_status_str_req_invalid);
		offset += sizeof(ib_mad_status_str_req_invalid) - 1;
	}
	if ((status & IB_MAD_STATUS_CLASS_MASK) == IB_SA_MAD_STATUS_NO_RECORDS) {
		if (!first) {
			strcat(&line[offset], generic_or_str);
			offset += sizeof(generic_or_str) - 1;
		}
		first = FALSE;
		strcat(&line[offset], ib_mad_status_str_no_records);
		offset += sizeof(ib_mad_status_str_no_records) - 1;
	}
	if ((status & IB_MAD_STATUS_CLASS_MASK) ==
	    IB_SA_MAD_STATUS_TOO_MANY_RECORDS) {
		if (!first) {
			strcat(&line[offset], generic_or_str);
			offset += sizeof(generic_or_str) - 1;
		}
		first = FALSE;
		strcat(&line[offset], ib_mad_status_str_too_many_records);
		offset += sizeof(ib_mad_status_str_too_many_records) - 1;
	}
	if ((status & IB_MAD_STATUS_CLASS_MASK) == IB_SA_MAD_STATUS_INVALID_GID) {
		if (!first) {
			strcat(&line[offset], generic_or_str);
			offset += sizeof(generic_or_str) - 1;
		}
		first = FALSE;
		strcat(&line[offset], ib_mad_status_str_invalid_gid);
		offset += sizeof(ib_mad_status_str_invalid_gid) - 1;
	}
	if ((status & IB_MAD_STATUS_CLASS_MASK) == IB_SA_MAD_STATUS_INSUF_COMPS) {
		if (!first) {
			strcat(&line[offset], generic_or_str);
			offset += sizeof(generic_or_str) - 1;
		}
		first = FALSE;
		strcat(&line[offset], ib_mad_status_str_insuf_comps);
		offset += sizeof(ib_mad_status_str_insuf_comps) - 1;
	}

	return (line);
}