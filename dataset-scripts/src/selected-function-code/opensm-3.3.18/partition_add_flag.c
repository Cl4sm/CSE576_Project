static int partition_add_flag(unsigned lineno, struct part_conf *conf,
			      char *flag, char *val)
{
	int len = strlen(flag);

	/* ipoib gc group flags are processed here. */
	if (parse_group_flag(lineno, conf->p_log, &conf->flags, flag, val))
		return 0;

	/* partition flags go here. */
	if (!strncmp(flag, "ipoib", len)) {
		conf->is_ipoib = 1;
	} else if (!strncmp(flag, "defmember", len)) {
		if (!val || (strncmp(val, "limited", strlen(val))
			     && strncmp(val, "both", strlen(val))
			     && strncmp(val, "full", strlen(val))))
			OSM_LOG(conf->p_log, OSM_LOG_VERBOSE,
				"PARSE WARN: line %d: "
				"flag \'defmember\' requires valid value (limited or full or both)"
				" - skipped\n", lineno);
		else {
			if (!strncmp(val, "full", strlen(val)))
				conf->membership = FULL;
			else if (!strncmp(val, "both", strlen(val)))
				conf->membership = BOTH;
			else
				conf->membership = LIMITED;
		}
	} else {
		OSM_LOG(conf->p_log, OSM_LOG_VERBOSE,
			"PARSE WARN: line %d: "
			"unrecognized partition flag \'%s\'"
			" - ignored\n", lineno, flag);
	}
	return 0;
}