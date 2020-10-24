static int parse_part_conf(struct part_conf *conf, char *str, int lineno)
{
	int ret, len = 0;
	char *name, *id, *flag, *flval;
	char *q, *p;

	p = str;
	if (*p == '\t' || *p == '\0' || *p == '\n')
		p++;

	len += (int)(p - str);
	str = p;

	if (conf->p_prtn)
		goto skip_header;

	q = strchr(p, ':');
	if (!q) {
		OSM_LOG(conf->p_log, OSM_LOG_ERROR, "PARSE ERROR: line %d: "
			"no partition definition found\n", lineno);
		fprintf(stderr, "\nPARSE ERROR: line %d: "
			"no partition definition found\n", lineno);
		return -1;
	}

	*q++ = '\0';
	str = q;

	name = id = flag = flval = NULL;

	q = strchr(p, ',');
	if (q)
		*q = '\0';

	ret = parse_name_token(p, &name, &id);
	p += ret;
	len += ret;

	while (q) {
		flag = flval = NULL;
		q = strchr(p, ',');
		if (q)
			*q++ = '\0';
		ret = parse_name_token(p, &flag, &flval);
		if (!flag) {
			OSM_LOG(conf->p_log, OSM_LOG_ERROR,
				"PARSE ERROR: line %d: "
				"bad partition flags\n", lineno);
			fprintf(stderr, "\nPARSE ERROR: line %d: "
				"bad partition flags\n", lineno);
			return -1;
		}
		p += ret;
		len += ret;
		partition_add_flag(lineno, conf, flag, flval);
	}

	if (p != str || (partition_create(lineno, conf,
					  name, id, flag, flval) < 0)) {
		OSM_LOG(conf->p_log, OSM_LOG_ERROR, "PARSE ERROR: line %d: "
			"bad partition definition\n", lineno);
		fprintf(stderr, "\nPARSE ERROR: line %d: "
			"bad partition definition\n", lineno);
		return -1;
	}

skip_header:
	do {
		name = flag = NULL;
		q = strchr(p, ',');
		if (q)
			*q++ = '\0';
		ret = parse_name_token(p, &name, &flag);
		len += ret;

		if (strcmp(name, "mgid") == 0) {
			/* parse an mgid line if specified. */
			len += mgroup_create(p+ret, flag, lineno, conf);
			goto done; /* We're done: this consumes the line */
		}
		if (partition_add_port(lineno, conf, name, flag) < 0) {
			OSM_LOG(conf->p_log, OSM_LOG_ERROR,
				"PARSE ERROR: line %d: "
				"bad PortGUID\n", lineno);
			fprintf(stderr, "PARSE ERROR: line %d: "
				"bad PortGUID\n", lineno);
			return -1;
		}
		p += ret;
	} while (q);

done:
	return len;
}