void osm_subn_destroy(IN osm_subn_t * p_subn)
{
	int i;
	osm_node_t *p_node, *p_next_node;
	osm_assigned_guids_t *p_assigned_guids, *p_next_assigned_guids;
	osm_alias_guid_t *p_alias_guid, *p_next_alias_guid;
	osm_port_t *p_port, *p_next_port;
	osm_switch_t *p_sw, *p_next_sw;
	osm_remote_sm_t *p_rsm, *p_next_rsm;
	osm_prtn_t *p_prtn, *p_next_prtn;
	osm_infr_t *p_infr, *p_next_infr;
	osm_svcr_t *p_svcr, *p_next_svcr;

	/* it might be a good idea to de-allocate all known objects */
	p_next_node = (osm_node_t *) cl_qmap_head(&p_subn->node_guid_tbl);
	while (p_next_node !=
	       (osm_node_t *) cl_qmap_end(&p_subn->node_guid_tbl)) {
		p_node = p_next_node;
		p_next_node = (osm_node_t *) cl_qmap_next(&p_node->map_item);
		osm_node_delete(&p_node);
	}

	p_next_assigned_guids = (osm_assigned_guids_t *) cl_qmap_head(&p_subn->assigned_guids_tbl);
	while (p_next_assigned_guids !=
	       (osm_assigned_guids_t *) cl_qmap_end(&p_subn->assigned_guids_tbl)) {
		p_assigned_guids = p_next_assigned_guids;
		p_next_assigned_guids = (osm_assigned_guids_t *) cl_qmap_next(&p_assigned_guids->map_item);
		osm_assigned_guids_delete(&p_assigned_guids);
	}

	p_next_alias_guid = (osm_alias_guid_t *) cl_qmap_head(&p_subn->alias_port_guid_tbl);
	while (p_next_alias_guid !=
	       (osm_alias_guid_t *) cl_qmap_end(&p_subn->alias_port_guid_tbl)) {
		p_alias_guid = p_next_alias_guid;
		p_next_alias_guid = (osm_alias_guid_t *) cl_qmap_next(&p_alias_guid->map_item);
		osm_alias_guid_delete(&p_alias_guid);
	}

	while (cl_qlist_count(&p_subn->alias_guid_list))
		osm_guid_work_obj_delete((osm_guidinfo_work_obj_t *) cl_qlist_remove_head(&p_subn->alias_guid_list));

	p_next_port = (osm_port_t *) cl_qmap_head(&p_subn->port_guid_tbl);
	while (p_next_port !=
	       (osm_port_t *) cl_qmap_end(&p_subn->port_guid_tbl)) {
		p_port = p_next_port;
		p_next_port = (osm_port_t *) cl_qmap_next(&p_port->map_item);
		osm_port_delete(&p_port);
	}

	p_next_sw = (osm_switch_t *) cl_qmap_head(&p_subn->sw_guid_tbl);
	while (p_next_sw != (osm_switch_t *) cl_qmap_end(&p_subn->sw_guid_tbl)) {
		p_sw = p_next_sw;
		p_next_sw = (osm_switch_t *) cl_qmap_next(&p_sw->map_item);
		osm_switch_delete(&p_sw);
	}

	p_next_rsm = (osm_remote_sm_t *) cl_qmap_head(&p_subn->sm_guid_tbl);
	while (p_next_rsm !=
	       (osm_remote_sm_t *) cl_qmap_end(&p_subn->sm_guid_tbl)) {
		p_rsm = p_next_rsm;
		p_next_rsm = (osm_remote_sm_t *) cl_qmap_next(&p_rsm->map_item);
		free(p_rsm);
	}

	p_next_prtn = (osm_prtn_t *) cl_qmap_head(&p_subn->prtn_pkey_tbl);
	while (p_next_prtn !=
	       (osm_prtn_t *) cl_qmap_end(&p_subn->prtn_pkey_tbl)) {
		p_prtn = p_next_prtn;
		p_next_prtn = (osm_prtn_t *) cl_qmap_next(&p_prtn->map_item);
		osm_prtn_delete(p_subn, &p_prtn);
	}

	cl_fmap_remove_all(&p_subn->mgrp_mgid_tbl);

	for (i = 0; i <= p_subn->max_mcast_lid_ho - IB_LID_MCAST_START_HO;
	     i++)
		if (p_subn->mboxes[i])
			osm_mgrp_box_delete(p_subn->mboxes[i]);

	p_next_infr = (osm_infr_t *) cl_qlist_head(&p_subn->sa_infr_list);
	while (p_next_infr !=
	       (osm_infr_t *) cl_qlist_end(&p_subn->sa_infr_list)) {
		p_infr = p_next_infr;
		p_next_infr = (osm_infr_t *) cl_qlist_next(&p_infr->list_item);
		osm_infr_delete(p_infr);
	}

	p_next_svcr = (osm_svcr_t *) cl_qlist_head(&p_subn->sa_sr_list);
	while (p_next_svcr !=
	       (osm_svcr_t *) cl_qlist_end(&p_subn->sa_sr_list)) {
		p_svcr = p_next_svcr;
		p_next_svcr = (osm_svcr_t *) cl_qlist_next(&p_svcr->list_item);
		osm_svcr_delete(p_svcr);
	}

	cl_ptr_vector_destroy(&p_subn->port_lid_tbl);

	osm_qos_policy_destroy(p_subn->p_qos_policy);

	while (!cl_is_qlist_empty(&p_subn->prefix_routes_list)) {
		cl_list_item_t *item = cl_qlist_remove_head(&p_subn->prefix_routes_list);
		free(item);
	}

	subn_opt_destroy(&p_subn->opt);
	free(p_subn->opt.file_opts);
}