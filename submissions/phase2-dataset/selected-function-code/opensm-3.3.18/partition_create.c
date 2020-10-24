static int partition_create(unsigned lineno, struct part_conf *conf,
			    char *name, char *id, char *flag, char *flag_val)
{
	ib_net16_t pkey;

	if (!id && name && isdigit(*name)) {
		id = name;
		name = NULL;
	}

	if (id) {
		char *end;

		pkey = cl_hton16((uint16_t)strtoul(id, &end, 0));
		if (end == id || *end)
			return -1;
	} else
		pkey = 0;

	conf->p_prtn = osm_prtn_make_new(conf->p_log, conf->p_subn,
					 name, pkey);
	if (!conf->p_prtn)
		return -1;

	if (!conf->p_subn->opt.qos && conf->flags.sl != OSM_DEFAULT_SL) {
		OSM_LOG(conf->p_log, OSM_LOG_DEBUG, "Overriding SL %d"
			" to default SL %d on partition %s"
			" as QoS is not enabled.\n",
			conf->flags.sl, OSM_DEFAULT_SL, name);
		conf->flags.sl = OSM_DEFAULT_SL;
	}
	conf->p_prtn->sl = (uint8_t) conf->flags.sl;

	if (conf->is_ipoib) {
		struct precreate_mgroup broadcast_mgroup;
		memset(&broadcast_mgroup, 0, sizeof(broadcast_mgroup));
		broadcast_mgroup.mgid = osm_ipoib_broadcast_mgid;
		pkey = CL_HTON16(0x8000) | conf->p_prtn->pkey;
		memcpy(&broadcast_mgroup.mgid.raw[4], &pkey , sizeof(pkey));
		broadcast_mgroup.flags.mtu = conf->flags.mtu;
		broadcast_mgroup.flags.rate = conf->flags.rate;
		broadcast_mgroup.flags.sl = conf->flags.sl;
		broadcast_mgroup.flags.Q_Key = conf->flags.Q_Key ?
						conf->flags.Q_Key :
						OSM_IPOIB_BROADCAST_MGRP_QKEY;
		broadcast_mgroup.flags.TClass = conf->flags.TClass;
		broadcast_mgroup.flags.FlowLabel = conf->flags.FlowLabel;
		__create_mgrp(conf, &broadcast_mgroup);
	}

	return 0;
}