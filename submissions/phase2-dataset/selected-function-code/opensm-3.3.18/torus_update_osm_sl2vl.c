static
void torus_update_osm_sl2vl(void *context, osm_physp_t *osm_phys_port,
			    uint8_t iport_num, uint8_t oport_num,
			    ib_slvl_table_t *osm_oport_sl2vl)
{
	osm_node_t *node = osm_physp_get_node_ptr(osm_phys_port);
	struct torus_context *ctx = context;
	struct t_switch *sw = NULL;
	int sl, vl;

	if (node->sw) {
		sw = node->sw->priv;
		if (sw && sw->osm_switch != node->sw) {
			osm_log_t *log = &ctx->osm->log;
			guid_t guid;

			guid = osm_node_get_node_guid(node);
			OSM_LOG(log, OSM_LOG_INFO,
				"Note: osm_switch (GUID 0x%04"PRIx64") "
				"not in torus fabric description\n",
				cl_ntoh64(guid));
			return;
		}
	}
	for (sl = 0; sl < 16; sl++) {
		vl = sl2vl_entry(ctx->torus, sw, iport_num, oport_num, sl);
		ib_slvl_table_set(osm_oport_sl2vl, sl, vl);
	}
}