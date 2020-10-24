static
int torus_build_lfts(void *context)
{
	int status = -1;
	struct torus_context *ctx = context;
	struct fabric *fabric;
	struct torus *torus;

	if (!ctx->osm->subn.opt.qos) {
		OSM_LOG(&ctx->osm->log, OSM_LOG_ERROR,
			"ERR 4E44: Routing engine list contains torus-2QoS. "
			"Enable QoS for correct operation "
			"(-Q or 'qos TRUE' in opensm.conf).\n");
		return status;
	}

	fabric = &ctx->fabric;
	teardown_fabric(fabric);

	torus = calloc(1, sizeof(*torus));
	if (!torus) {
		OSM_LOG(&ctx->osm->log, OSM_LOG_ERROR,
			"ERR 4E45: allocating torus: %s\n", strerror(errno));
		goto out;
	}
	torus->osm = ctx->osm;
	fabric->osm = ctx->osm;

	if (!parse_config(ctx->osm->subn.opt.torus_conf_file,
			  fabric, torus))
		goto out;

	if (!capture_fabric(fabric))
		goto out;

	OSM_LOG(&torus->osm->log, OSM_LOG_INFO,
		"Found fabric w/ %d links, %d switches, %d CA ports, "
		"minimum data VLs: endport %d, switchport %d\n",
		(int)fabric->link_cnt, (int)fabric->switch_cnt,
		(int)fabric->ca_cnt, (int)ctx->osm->subn.min_data_vls,
		(int)ctx->osm->subn.min_sw_data_vls);

	if (!verify_setup(torus, fabric))
		goto out;

	OSM_LOG(&torus->osm->log, OSM_LOG_INFO,
		"Looking for %d x %d x %d %s\n",
		(int)torus->x_sz, (int)torus->y_sz, (int)torus->z_sz,
		(ALL_MESH(torus->flags) ? "mesh" : "torus"));

	if (!build_torus(fabric, torus)) {
		OSM_LOG(&torus->osm->log, OSM_LOG_ERROR, "ERR 4E57: "
			"build_torus finished with errors\n");
		goto out;
	}

	OSM_LOG(&torus->osm->log, OSM_LOG_INFO,
		"Built %d x %d x %d %s w/ %d links, %d switches, %d CA ports\n",
		(int)torus->x_sz, (int)torus->y_sz, (int)torus->z_sz,
		(ALL_MESH(torus->flags) ? "mesh" : "torus"),
		(int)torus->link_cnt, (int)torus->switch_cnt,
		(int)torus->ca_cnt);

	diagnose_fabric(fabric);
	/*
	 * Since we found some sort of torus fabric, report on any topology
	 * changes vs. the last torus we found.
	 */
	if (torus->flags & NOTIFY_CHANGES)
		report_torus_changes(torus, ctx->torus);

	if (routable_torus(torus, fabric))
		status = route_torus(torus);

out:
	if (status) {		/* bad torus!! */
		if (torus)
			teardown_torus(torus);
	} else {
		osm_subn_opt_t *opt = &torus->osm->subn.opt;
		osm_log_t *log = &torus->osm->log;

		if (ctx->torus)
			teardown_torus(ctx->torus);
		ctx->torus = torus;

		check_qos_swe_config(&opt->qos_swe_options, &opt->qos_options,
				     log);

		check_qos_ep_config(&opt->qos_ca_options,
				    &opt->qos_options, "qos_ca", log);
		check_qos_ep_config(&opt->qos_sw0_options,
				    &opt->qos_options, "qos_sw0", log);
		check_qos_ep_config(&opt->qos_rtr_options,
				    &opt->qos_options, "qos_rtr", log);
	}
	teardown_fabric(fabric);
	return status;
}