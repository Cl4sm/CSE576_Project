static void cache_cleanup_switches(osm_ucast_mgr_t * p_mgr)
{
	cache_switch_t *p_sw;
	cache_switch_t *p_next_sw;
	unsigned port_num;
	boolean_t found_port;

	if (!p_mgr->cache_valid)
		return;

	p_next_sw = (cache_switch_t *) cl_qmap_head(&p_mgr->cache_sw_tbl);
	while (p_next_sw !=
	       (cache_switch_t *) cl_qmap_end(&p_mgr->cache_sw_tbl)) {
		p_sw = p_next_sw;
		p_next_sw = (cache_switch_t *) cl_qmap_next(&p_sw->map_item);

		found_port = FALSE;
		for (port_num = 1; port_num < p_sw->num_ports; port_num++)
			if (p_sw->ports[port_num].remote_lid_ho)
				found_port = TRUE;

		if (!found_port) {
			cl_qmap_remove_item(&p_mgr->cache_sw_tbl,
					    &p_sw->map_item);
			cache_sw_destroy(p_sw);
		}
	}
}