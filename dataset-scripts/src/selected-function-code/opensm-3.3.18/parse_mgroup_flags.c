static int parse_mgroup_flags(osm_log_t * p_log,
				struct precreate_mgroup *mgroup,
				char *p, unsigned lineno)
{
	int ret, len = 0;
	char *flag, *val, *q;
	do {
		flag = val = NULL;
		q = strchr(p, ',');
		if (q)
			*q++ = '\0';

		ret = parse_name_token(p, &flag, &val);

		if (!parse_group_flag(lineno, p_log, &mgroup->flags,
				     flag, val)) {
			OSM_LOG(p_log, OSM_LOG_VERBOSE,
				"PARSE WARN: line %d: "
				"unrecognized mgroup flag \'%s\'"
				" - ignored\n", lineno, flag);
		}
		p += ret;
		len += ret;
	} while (q);

	return (len);
}