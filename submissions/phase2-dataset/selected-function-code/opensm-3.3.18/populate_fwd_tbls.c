static void populate_fwd_tbls(lash_t * p_lash)
{
	osm_log_t *p_log = &p_lash->p_osm->log;
	osm_subn_t *p_subn = &p_lash->p_osm->subn;
	osm_switch_t *p_sw, *p_next_sw, *p_dst_sw;
	osm_port_t *port;
	uint16_t max_lid_ho, lid;

	OSM_LOG_ENTER(p_log);

	p_next_sw = (osm_switch_t *) cl_qmap_head(&p_subn->sw_guid_tbl);

	/* Go through each switch individually */
	while (p_next_sw != (osm_switch_t *) cl_qmap_end(&p_subn->sw_guid_tbl)) {
		uint64_t current_guid;
		switch_t *sw;
		p_sw = p_next_sw;
		p_next_sw = (osm_switch_t *) cl_qmap_next(&p_sw->map_item);

		max_lid_ho = p_sw->max_lid_ho;
		current_guid = p_sw->p_node->node_info.port_guid;
		sw = p_sw->priv;

		memset(p_sw->new_lft, OSM_NO_PATH, p_sw->lft_size);

		for (lid = 1; lid <= max_lid_ho; lid++) {
			port = osm_get_port_by_lid_ho(p_subn, lid);
			if (!port)
				continue;

			p_dst_sw = get_osm_switch_from_port(port);
			if (p_dst_sw == p_sw) {
				uint8_t egress_port = port->p_node->sw ? 0 :
					port->p_physp->p_remote_physp->port_num;
				p_sw->new_lft[lid] = egress_port;
				OSM_LOG(p_log, OSM_LOG_VERBOSE,
					"LASH fwd MY SRC SRC GUID 0x%016" PRIx64
					" src lash id (%d), src lid no (%u) src lash port (%d) "
					"DST GUID 0x%016" PRIx64
					" src lash id (%d), src lash port (%d)\n",
					cl_ntoh64(current_guid), -1, lid,
					egress_port, cl_ntoh64(current_guid),
					-1, egress_port);
			} else if (p_dst_sw) {
				unsigned dst_lash_switch_id =
				    get_lash_id(p_dst_sw);
				uint8_t lash_egress_port =
				    (uint8_t) sw->
				    routing_table[dst_lash_switch_id].out_link;
				uint8_t physical_egress_port =
					get_next_port(sw, lash_egress_port);

				p_sw->new_lft[lid] = physical_egress_port;
				OSM_LOG(p_log, OSM_LOG_VERBOSE,
					"LASH fwd SRC GUID 0x%016" PRIx64
					" src lash id (%d), "
					"src lid no (%u) src lash port (%d) "
					"DST GUID 0x%016" PRIx64
					" src lash id (%d), src lash port (%d)\n",
					cl_ntoh64(current_guid), sw->id, lid,
					lash_egress_port,
					cl_ntoh64(p_dst_sw->p_node->node_info.
						  port_guid),
					dst_lash_switch_id,
					physical_egress_port);
			}
		}		/* for */
	}
	OSM_LOG_EXIT(p_log);
}