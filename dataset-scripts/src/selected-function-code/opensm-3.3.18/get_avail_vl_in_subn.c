static uint8_t get_avail_vl_in_subn(osm_ucast_mgr_t * p_mgr)
{
	uint32_t i = 0;
	uint8_t vls_avail = 0xFF, port_vls_avail = 0;
	cl_qmap_t *sw_tbl = &p_mgr->p_subn->sw_guid_tbl;
	cl_map_item_t *item = NULL;
	osm_switch_t *sw = NULL;

	/* traverse all switches to get the number of available virtual lanes in the subnet */
	for (item = cl_qmap_head(sw_tbl); item != cl_qmap_end(sw_tbl);
	     item = cl_qmap_next(item)) {
		sw = (osm_switch_t *) item;

		/* ignore management port 0 */
		for (i = 1; i < osm_node_get_num_physp(sw->p_node); i++) {
			osm_physp_t *p_physp =
			    osm_node_get_physp_ptr(sw->p_node, i);

			if (p_physp && p_physp->p_remote_physp) {
				port_vls_avail =
				    ib_port_info_get_op_vls(&p_physp->
							    port_info);
				if (port_vls_avail
				    && port_vls_avail < vls_avail)
					vls_avail = port_vls_avail;
			}
		}
	}

	/* ib_port_info_get_op_vls gives values 1 ... 5 (s. IBAS 14.2.5.6) */
	vls_avail = 1 << (vls_avail - 1);

	/* set boundaries (s. IBAS 3.5.7) */
	if (vls_avail > 15)
		vls_avail = 15;
	if (vls_avail < 1)
		vls_avail = 1;

	return vls_avail;
}