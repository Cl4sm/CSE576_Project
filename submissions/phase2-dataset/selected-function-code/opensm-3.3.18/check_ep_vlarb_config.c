static
void check_ep_vlarb_config(const char *vlarb_str,
			   bool is_default, bool is_specific,
			   const char *str, const char *pri, osm_log_t *log)
{
	unsigned i, total_weight[IB_MAX_NUM_VLS] = {0,};
	int val = 0;

	sum_vlarb_weights(vlarb_str, total_weight);
	for (i = 2; i < 8; i++) {
		val += total_weight[i];
	}
	if (!val)
		return;

	if (is_specific)
		OSM_LOG(log, OSM_LOG_INFO,
			"Warning: torus-2QoS recommends 0 VLarb weights"
			" for VLs 2-7 on endpoint links; not true for "
			" configured %s_vlarb_%s\n", str, pri);
	else
		OSM_LOG(log, OSM_LOG_INFO,
			"Warning: torus-2QoS recommends 0 VLarb weights "
			"for VLs 2-7 on endpoint links; not true for %s "
			"qos_vlarb_%s values used for %s_vlarb_%s\n",
			(is_default ? "default" : "configured"), pri, str, pri);
}