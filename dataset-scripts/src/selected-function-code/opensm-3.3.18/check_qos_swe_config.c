static
void check_qos_swe_config(osm_qos_options_t *opt,
			  osm_qos_options_t *def, osm_log_t *log)
{
	const char *vlarb_str, *tstr;
	bool is_default;
	unsigned max_vls;

	max_vls = def->max_vls;
	if (opt->max_vls > 0)
		max_vls = opt->max_vls;

	if (max_vls > 0 && max_vls < 8)
		OSM_LOG(log, OSM_LOG_INFO,
			"Warning: full torus-2QoS functionality not available "
			"for configured %s_max_vls = %d\n",
			(opt->max_vls > 0 ? "qos_swe" : "qos"), opt->max_vls);

	vlarb_str = opt->vlarb_high;
	is_default = false;
	tstr = "qos_swe";
	if (!vlarb_str) {
		vlarb_str = def->vlarb_high;
		tstr = "qos";
	}
	if (!vlarb_str) {
		vlarb_str = OSM_DEFAULT_QOS_VLARB_HIGH;
		is_default = true;
	}
	check_vlarb_config(vlarb_str, is_default, tstr, "high", log);

	vlarb_str = opt->vlarb_low;
	is_default = false;
	tstr = "qos_swe";
	if (!vlarb_str) {
		vlarb_str = def->vlarb_low;
		tstr = "qos";
	}
	if (!vlarb_str) {
		vlarb_str = OSM_DEFAULT_QOS_VLARB_LOW;
		is_default = true;
	}
	check_vlarb_config(vlarb_str, is_default, tstr, "low", log);

	if (opt->sl2vl)
		OSM_LOG(log, OSM_LOG_INFO,
			"Warning: torus-2QoS must override configured "
			"qos_swe_sl2vl to generate deadlock-free routes\n");
}