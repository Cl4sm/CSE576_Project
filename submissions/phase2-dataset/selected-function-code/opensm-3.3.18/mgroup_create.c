static int mgroup_create(char *p, char *mgid, unsigned lineno, struct part_conf *conf)
{
	int ret = 0;
	struct precreate_mgroup mgroup;

	memset(&mgroup, 0, sizeof(mgroup));

	if (inet_pton(AF_INET6, mgid, &mgroup.mgid) != 1
	    || mgroup.mgid.raw[0] != 0xff) {
		OSM_LOG(conf->p_log, OSM_LOG_ERROR,
			"PARSE ERROR partition conf file line %d: "
			"mgid \"%s\": gid is not multicast\n", lineno, mgid);
		return 0;
	}

	/* inherit partition flags */
	mgroup.flags.mtu = conf->flags.mtu;
	mgroup.flags.rate = conf->flags.rate;
	mgroup.flags.sl = conf->flags.sl;
	mgroup.flags.Q_Key = conf->flags.Q_Key;
	mgroup.flags.FlowLabel = conf->flags.FlowLabel;
	mgroup.flags.scope_mask = conf->flags.scope_mask;

	/* override with user specified flags */
	ret = parse_mgroup_flags(conf->p_log, &mgroup, p, lineno);

	/* check/verify special IP group parameters */
	if (mgid_is_ip(&mgroup.mgid)) {
		ib_net16_t pkey = conf->p_prtn->pkey | cl_hton16(0x8000);

		if (!ip_mgroup_pkey_ok(conf, &mgroup)
		    || !ip_mgroup_rate_ok(conf, &mgroup)
		    || !ip_mgroup_mtu_ok(conf, &mgroup))
			goto error;

		/* set special IP settings */
		memcpy(&mgroup.mgid.raw[4], &pkey, sizeof(pkey));

		if (mgroup.flags.Q_Key == 0)
			mgroup.flags.Q_Key = OSM_IPOIB_BROADCAST_MGRP_QKEY;
	}

	/* don't create multiple copies of the group */
	if (osm_get_mgrp_by_mgid(conf->p_subn, &mgroup.mgid))
		goto error;

	/* create the group */
	__create_mgrp(conf, &mgroup);

error:
	return ret;
}