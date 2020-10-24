static void osm_lash_process_switch(lash_t * p_lash, osm_switch_t * p_sw)
{
	osm_log_t *p_log = &p_lash->p_osm->log;
	int i, port_count;
	osm_physp_t *p_current_physp, *p_remote_physp;
	unsigned switch_a_lash_id, switch_b_lash_id;

	OSM_LOG_ENTER(p_log);

	switch_a_lash_id = get_lash_id(p_sw);
	port_count = osm_node_get_num_physp(p_sw->p_node);

	/* starting at port 1, ignoring management port on switch */
	for (i = 1; i < port_count; i++) {

		p_current_physp = osm_node_get_physp_ptr(p_sw->p_node, i);
		if (p_current_physp) {
			p_remote_physp = p_current_physp->p_remote_physp;
			if (p_remote_physp && p_remote_physp->p_node->sw) {
				int physical_port_a_num =
				    osm_physp_get_port_num(p_current_physp);
				int physical_port_b_num =
				    osm_physp_get_port_num(p_remote_physp);
				switch_b_lash_id =
				    get_lash_id(p_remote_physp->p_node->sw);

				connect_switches(p_lash, switch_a_lash_id,
						 switch_b_lash_id,
						 physical_port_a_num);
				OSM_LOG(p_log, OSM_LOG_VERBOSE,
					"LASH SUCCESS connected G 0x%016" PRIx64
					" , lash_id(%u), P(%u) " " to G 0x%016"
					PRIx64 " , lash_id(%u) , P(%u)\n",
					cl_ntoh64(osm_physp_get_port_guid
						  (p_current_physp)),
					switch_a_lash_id, physical_port_a_num,
					cl_ntoh64(osm_physp_get_port_guid
						  (p_remote_physp)),
					switch_b_lash_id, physical_port_b_num);
			}
		}
	}

	OSM_LOG_EXIT(p_log);
}