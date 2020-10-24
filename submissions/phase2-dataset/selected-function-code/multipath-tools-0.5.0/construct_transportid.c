int
construct_transportid(const char * lcp, struct transportid transid[], int num_transportids)
{
	int k = 0;
	int j, n, b, c, len, alen;
	const char * ecp;
	const char * isip;

	if ((0 == memcmp("fcp,", lcp, 4)) ||
			(0 == memcmp("FCP,", lcp, 4))) {
		lcp += 4;
		k = strspn(lcp, "0123456789aAbBcCdDeEfF");

		len = strlen(lcp);
		if (len != k) {
			fprintf(stderr, "badly formed symbolic FCP TransportID: %s\n",
					lcp);
			return 1;
		}
		transid[num_transportids].format_code = MPATH_PROTOCOL_ID_FC;
		transid[num_transportids].protocol_id = MPATH_WWUI_DEVICE_NAME;
		for (k = 0, j = 0, b = 0; k < 16; ++k) {
			c = lcp[k];
			if (isdigit(c))
				n = c - 0x30;
			else if (isupper(c))
				n = c - 0x37;
			else
				n = c - 0x57;
			if (k & 1) {
				transid[num_transportids].n_port_name[j] = b | n;
				++j;
			} else
				b = n << 4;
		}
		goto my_cont_b;
	}
	if ((0 == memcmp("sas,", lcp, 4)) || (0 == memcmp("SAS,", lcp, 4))) {
		lcp += 4;
		k = strspn(lcp, "0123456789aAbBcCdDeEfF");
		len =strlen(lcp);
		if (len != k) {
			fprintf(stderr, "badly formed symbolic SAS TransportID: %s\n",
					lcp);
			return 1;
		}
		transid[num_transportids].format_code = MPATH_PROTOCOL_ID_SAS;
		transid[num_transportids].protocol_id = MPATH_WWUI_DEVICE_NAME;
		memcpy(&transid[num_transportids].sas_address, lcp, 8);

		goto my_cont_b;
	}
	if (0 == memcmp("iqn.", lcp, 4)) {
		ecp = strpbrk(lcp, " \t");
		isip = strstr(lcp, ",i,0x");
		if (ecp && (isip > ecp))
			isip = NULL;
		len = ecp ? (ecp - lcp) : (int)strlen(lcp);
		transid[num_transportids].format_code = (isip ? MPATH_WWUI_PORT_IDENTIFIER:MPATH_WWUI_DEVICE_NAME);
		transid[num_transportids].protocol_id = MPATH_PROTOCOL_ID_ISCSI;
		alen = len + 1; /* at least one trailing null */
		if (alen < 20)
			alen = 20;
		else if (0 != (alen % 4))
			alen = ((alen / 4) + 1) * 4;
		if (alen > 241) { /* sam5r02.pdf A.2 (Annex) */
			fprintf(stderr, "iSCSI name too long, alen=%d\n", alen);
			return 0;
		}
		transid[num_transportids].iscsi_name[1] = alen & 0xff;
		memcpy(&transid[num_transportids].iscsi_name[2], lcp, len);
		goto my_cont_b;
	}
my_cont_b:
	if (k >= MPATH_MAX_PARAM_LEN) {
		fprintf(stderr, "build_transportid: array length exceeded\n");
		return 1;
	}
	return 0;
}
