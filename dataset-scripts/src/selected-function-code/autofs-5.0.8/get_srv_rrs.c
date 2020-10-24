int get_srv_rrs(unsigned int logopt,
		char *name, struct srv_rr **dcs, unsigned int *dcs_count)
{
	struct srv_rr *srvs;
	unsigned int srv_num;
	HEADER *header;
	u_char *packet;
	u_char *start;
	u_char *end;
	unsigned int count;
	int i, len;
	char ebuf[MAX_ERR_BUF];

	len = do_srv_query(logopt, name, &packet);
	if (len < 0)
		return 0;

	header = (HEADER *) packet;
	start = packet + sizeof(HEADER);
	end = packet + len;

	srvs = NULL;
	srv_num = 0;

	/* Skip over question */
	len = get_name_len(packet, start, end);
	if (len < 0) {
		error(logopt, "failed to get name length");
		goto error_out;
	}

	start += len + QFIXEDSZ;

	count = ntohs(header->ancount);

	debug(logopt, "%d records returned in the answer section", count);

	if (count <= 0) {
		error(logopt, "no records found in answers section");
		goto error_out;
	}

	srvs = malloc(sizeof(struct srv_rr) * count);
	if (!srvs) {
		char *estr = strerror_r(errno, ebuf, MAX_ERR_BUF);
		error(logopt, "malloc: %s", estr);
		goto error_out;
	}
	memset(srvs, 0, sizeof(struct srv_rr) * count);

	srv_num = 0;
	for (i = 0; i < count && (start < end); i++) {
		unsigned int data_offset;
		struct srv_rr srv;
		struct srv_rr *psrv;
		struct rr rr;

		memset(&rr, 0, sizeof(struct rr));

		data_offset = get_data_offset(packet, start, end, &rr);
		if (data_offset <= 0) {
			error(logopt, "failed to get start of data");
			goto error_out;
		}
		start += data_offset;

		if (rr.type != T_SRV)
			continue;

		psrv = parse_srv_rr(logopt, packet, start, end, &rr, &srv);
		if (psrv) {
			memcpy(&srvs[srv_num], psrv, sizeof(struct srv_rr));
			srv_num++;
		}

		start += rr.len;
	}
	free(packet);

	if (!srv_num) {
		error(logopt, "no srv resource records found");
		goto error_srvs;
	}

	qsort(srvs, srv_num, sizeof(struct srv_rr),
		(int (*)(const void *, const void *)) cmp);

	*dcs = srvs;
	*dcs_count = srv_num;

	return 1;

error_out:
	free(packet);
error_srvs:
	if (srvs)
		free_srv_rrs(srvs, srv_num);
	return 0;
}