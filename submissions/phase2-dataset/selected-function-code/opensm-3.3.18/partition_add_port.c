static int partition_add_port(unsigned lineno, struct part_conf *conf,
			      char *name, char *flag)
{
	osm_prtn_t *p = conf->p_prtn;
	ib_net64_t guid;
	membership_t membership = conf->membership;

	if (!name || !*name || !strncmp(name, "NONE", strlen(name)))
		return 0;

	if (flag) {
		/* reset default membership to limited */
		membership = LIMITED;
		if (!strncmp(flag, "full", strlen(flag)))
			membership = FULL;
		else if (!strncmp(flag, "both", strlen(flag)))
			membership = BOTH;
		else if (strncmp(flag, "limited", strlen(flag))) {
			OSM_LOG(conf->p_log, OSM_LOG_VERBOSE,
				"PARSE WARN: line %d: "
				"unrecognized port flag \'%s\'."
				" Assume \'limited\'\n", lineno, flag);
		}
	}

	if (!strncmp(name, "ALL", strlen(name)))
		return partition_add_all(conf, p, 0, membership);
	else if (!strncmp(name, "ALL_CAS", strlen(name)))
		return partition_add_all(conf, p, IB_NODE_TYPE_CA, membership);
	else if (!strncmp(name, "ALL_SWITCHES", strlen(name)))
		return partition_add_all(conf, p, IB_NODE_TYPE_SWITCH,
					 membership);
	else if (!strncmp(name, "ALL_ROUTERS", strlen(name)))
		return partition_add_all(conf, p, IB_NODE_TYPE_ROUTER,
					 membership);
	else if (!strncmp(name, "SELF", strlen(name))) {
		guid = cl_ntoh64(conf->p_subn->sm_port_guid);
	} else {
		char *end;
		guid = strtoull(name, &end, 0);
		if (!guid || *end)
			return -1;
	}

	if (membership != LIMITED &&
	    osm_prtn_add_port(conf->p_log, conf->p_subn, p,
			      cl_hton64(guid), TRUE) != IB_SUCCESS)
		return -1;
	if ((membership == LIMITED ||
	    (membership == BOTH && conf->p_subn->opt.allow_both_pkeys)) &&
	    osm_prtn_add_port(conf->p_log, conf->p_subn, p,
			      cl_hton64(guid), FALSE) != IB_SUCCESS)
		return -1;
	return 0;
}