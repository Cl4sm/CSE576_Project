static int ontap_prio(const char *dev, int fd)
{
	unsigned char results[RESULTS_MAX];
	int results_size=RESULTS_MAX;
	int rc;
	int is_proxy;
	int is_iscsi_software;
	int is_iscsi_hardware;
	int tot_len;

	is_iscsi_software = is_iscsi_hardware = is_proxy = 0;

	memset(&results, 0, sizeof (results));
	rc = send_gva(dev, fd, 0x41, results, &results_size);
	if (rc >= 0) {
		tot_len = results[0] << 24 | results[1] << 16 |
			  results[2] << 8 | results[3];
		if (tot_len <= 8) {
			goto try_fcp_proxy;
		}
		if (results[8] != 0x41) {
			pp_ontap_log(0, "GVA page 0x41 error - "
				"results[8] = 0x%x", results[8]);
			goto try_fcp_proxy;
		}
		if ((strncmp((char *)&results[12], "ism_sw", 6) == 0) ||
		    (strncmp((char *)&results[12], "iswt", 4) == 0)) {
			is_iscsi_software = 1;
			goto prio_select;
		}
		else if (strncmp((char *)&results[12], "ism_sn", 6) == 0) {
			is_iscsi_hardware = 1;
			goto prio_select;
		}
	} else {
		return 0;
	}

 try_fcp_proxy:
	rc = get_proxy(dev, fd);
	if (rc >= 0) {
		is_proxy = rc;
	}

 prio_select:
	if (is_iscsi_hardware) {
		return 3;
	} else if (is_iscsi_software) {
		return 2;
	} else {
		if (is_proxy) {
			return 1;
		} else {
			/* Either non-proxy, or couldn't get proxy info */
			return 4;
		}
	}
}
