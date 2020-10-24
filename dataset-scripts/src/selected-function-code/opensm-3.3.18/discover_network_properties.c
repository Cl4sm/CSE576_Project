static int discover_network_properties(lash_t * p_lash)
{
	int i, id = 0;
	uint8_t vl_min;
	osm_subn_t *p_subn = &p_lash->p_osm->subn;
	osm_switch_t *p_next_sw, *p_sw;
	osm_log_t *p_log = &p_lash->p_osm->log;

	p_lash->num_switches = cl_qmap_count(&p_subn->sw_guid_tbl);

	p_lash->switches = calloc(p_lash->num_switches, sizeof(switch_t *));
	if (!p_lash->switches)
		return -1;

	vl_min = 5;		/* set to a high value */

	p_next_sw = (osm_switch_t *) cl_qmap_head(&p_subn->sw_guid_tbl);
	while (p_next_sw != (osm_switch_t *) cl_qmap_end(&p_subn->sw_guid_tbl)) {
		uint16_t port_count;
		p_sw = p_next_sw;
		p_next_sw = (osm_switch_t *) cl_qmap_next(&p_sw->map_item);

		p_lash->switches[id] = switch_create(p_lash, id, p_sw);
		if (!p_lash->switches[id])
			return -1;
		id++;

		port_count = osm_node_get_num_physp(p_sw->p_node);

		/* Note, ignoring port 0. management port */
		for (i = 1; i < port_count; i++) {
			osm_physp_t *p_current_physp =
			    osm_node_get_physp_ptr(p_sw->p_node, i);

			if (p_current_physp
			    && p_current_physp->p_remote_physp) {

				ib_port_info_t *p_port_info =
				    &p_current_physp->port_info;
				uint8_t port_vl_min =
				    ib_port_info_get_op_vls(p_port_info);
				if (port_vl_min && port_vl_min < vl_min)
					vl_min = port_vl_min;
			}
		}		/* for */
	}			/* while */

	vl_min = 1 << (vl_min - 1);
	if (vl_min > 15)
		vl_min = 15;

	if (p_lash->p_osm->subn.opt.lash_start_vl >= vl_min) {
		OSM_LOG(p_log, OSM_LOG_ERROR, "ERR 4D03: "
			"Start VL(%d) too high for min operational vl(%d)\n",
			p_lash->p_osm->subn.opt.lash_start_vl, vl_min);
		return -1;
	}

	p_lash->vl_min = vl_min - p_lash->p_osm->subn.opt.lash_start_vl;

	OSM_LOG(p_log, OSM_LOG_INFO,
		"min operational vl(%d) start vl(%d) max_switches(%d)\n",
		p_lash->vl_min, p_lash->p_osm->subn.opt.lash_start_vl,
		p_lash->num_switches);
	return 0;
}