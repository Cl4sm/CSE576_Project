static void drop_mgr_remove_port(osm_sm_t * sm, IN osm_port_t * p_port)
{
	ib_net64_t port_guid;
	osm_port_t *p_port_check;
	cl_qmap_t *p_alias_guid_tbl;
	cl_qmap_t *p_sm_guid_tbl;
	osm_mcm_port_t *mcm_port;
	cl_ptr_vector_t *p_port_lid_tbl;
	uint16_t min_lid_ho;
	uint16_t max_lid_ho;
	uint16_t lid_ho;
	osm_node_t *p_node;
	osm_remote_sm_t *p_sm;
	osm_alias_guid_t *p_alias_guid, *p_alias_guid_check;
	osm_guidinfo_work_obj_t *wobj;
	cl_list_item_t *item, *next_item;
	ib_gid_t port_gid;
	ib_mad_notice_attr_t notice;
	ib_api_status_t status;

	OSM_LOG_ENTER(sm->p_log);

	port_guid = osm_port_get_guid(p_port);
	OSM_LOG(sm->p_log, OSM_LOG_VERBOSE,
		"Unreachable port 0x%016" PRIx64 "\n", cl_ntoh64(port_guid));

	p_port_check =
	    (osm_port_t *) cl_qmap_get(&sm->p_subn->port_guid_tbl, port_guid);
	if (p_port_check != p_port) {
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 0101: "
			"Port 0x%016" PRIx64 " not in guid table\n",
			cl_ntoh64(port_guid));
		goto Exit;
	}

	/* issue a notice - trap 65 */
	/* details of the notice */
	notice.generic_type = 0x83;	/* is generic subn mgt type */
	ib_notice_set_prod_type_ho(&notice, 4);	/* A class manager generator */
	/* endport ceases to be reachable */
	notice.g_or_v.generic.trap_num = CL_HTON16(65);
	/* The sm_base_lid is saved in network order already. */
	notice.issuer_lid = sm->p_subn->sm_base_lid;
	/* following C14-72.1.2 and table 119 p725 */
	/* we need to provide the GID */
	port_gid.unicast.prefix = sm->p_subn->opt.subnet_prefix;
	port_gid.unicast.interface_id = port_guid;
	memcpy(&(notice.data_details.ntc_64_67.gid),
	       &(port_gid), sizeof(ib_gid_t));

	/* According to page 653 - the issuer gid in this case of trap
	   is the SM gid, since the SM is the initiator of this trap. */
	notice.issuer_gid.unicast.prefix = sm->p_subn->opt.subnet_prefix;
	notice.issuer_gid.unicast.interface_id = sm->p_subn->sm_port_guid;

	status = osm_report_notice(sm->p_log, sm->p_subn, &notice);
	if (status != IB_SUCCESS) {
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 0103: "
			"Error sending trap reports (%s)\n",
			ib_get_err_str(status));
	}

	next_item = cl_qlist_head(&sm->p_subn->alias_guid_list);
	while (next_item != cl_qlist_end(&sm->p_subn->alias_guid_list)) {
		item = next_item;
		next_item = cl_qlist_next(item);
		wobj = cl_item_obj(item, wobj, list_item);
		if (wobj->p_port == p_port) {
			cl_qlist_remove_item(&sm->p_subn->alias_guid_list,
					     &wobj->list_item);
			osm_guid_work_obj_delete(wobj);
		}
	}

	while (!cl_is_qlist_empty(&p_port->mcm_list)) {
		mcm_port = cl_item_obj(cl_qlist_head(&p_port->mcm_list),
				       mcm_port, list_item);
		osm_mgrp_delete_port(sm->p_subn, sm->p_log, mcm_port->mgrp,
				     p_port);
	}

	p_alias_guid_tbl = &sm->p_subn->alias_port_guid_tbl;
	p_alias_guid_check = (osm_alias_guid_t *) cl_qmap_head(p_alias_guid_tbl);
	while (p_alias_guid_check != (osm_alias_guid_t *) cl_qmap_end(p_alias_guid_tbl)) {
		if (p_alias_guid_check->p_base_port == p_port)
			p_alias_guid = p_alias_guid_check;
		else
			p_alias_guid = NULL;
		p_alias_guid_check = (osm_alias_guid_t *) cl_qmap_next(&p_alias_guid_check->map_item);
		if (p_alias_guid) {
			cl_qmap_remove_item(p_alias_guid_tbl,
					    &p_alias_guid->map_item);
			osm_alias_guid_delete(&p_alias_guid);
		}
	}

	cl_qmap_remove(&sm->p_subn->port_guid_tbl, port_guid);

	p_sm_guid_tbl = &sm->p_subn->sm_guid_tbl;
	p_sm = (osm_remote_sm_t *) cl_qmap_remove(p_sm_guid_tbl, port_guid);
	if (p_sm != (osm_remote_sm_t *) cl_qmap_end(p_sm_guid_tbl)) {
		/* need to remove this item */
		OSM_LOG(sm->p_log, OSM_LOG_VERBOSE,
			"Cleaned SM for port guid 0x%016" PRIx64 "\n",
			cl_ntoh64(port_guid));
		/* clean up the polling_sm_guid */
		if (sm->polling_sm_guid == p_sm->smi.guid)
			sm->polling_sm_guid = 0;
		free(p_sm);
	}

	drop_mgr_remove_router(sm, port_guid);

	osm_port_get_lid_range_ho(p_port, &min_lid_ho, &max_lid_ho);

	OSM_LOG(sm->p_log, OSM_LOG_VERBOSE,
		"Clearing abandoned LID range [%u,%u]\n",
		min_lid_ho, max_lid_ho);

	p_port_lid_tbl = &sm->p_subn->port_lid_tbl;
	for (lid_ho = min_lid_ho; lid_ho <= max_lid_ho; lid_ho++)
		cl_ptr_vector_set(p_port_lid_tbl, lid_ho, NULL);

	drop_mgr_clean_physp(sm, p_port->p_physp);

	/* Delete event forwarding subscriptions */
	if (sm->p_subn->opt.drop_event_subscriptions) {
		if (osm_infr_remove_subscriptions(sm->p_subn, sm->p_log, port_guid)
		    == CL_SUCCESS)
			OSM_LOG(sm->p_log, OSM_LOG_DEBUG,
			    "Removed event subscriptions for port 0x%016" PRIx64 "\n",
			    cl_ntoh64(port_guid));
	}

	/* initialize the p_node - may need to get node_desc later */
	p_node = p_port->p_node;

	osm_port_delete(&p_port);

	OSM_LOG(sm->p_log, OSM_LOG_INFO,
		"Removed port with GUID:0x%016" PRIx64
		" LID range [%u, %u] of node:%s\n",
		cl_ntoh64(port_gid.unicast.interface_id),
		min_lid_ho, max_lid_ho,
		p_node ? p_node->print_desc : "UNKNOWN");

Exit:
	OSM_LOG_EXIT(sm->p_log);
}