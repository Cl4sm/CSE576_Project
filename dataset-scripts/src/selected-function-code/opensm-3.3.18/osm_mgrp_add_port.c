osm_mcm_port_t *osm_mgrp_add_port(IN osm_subn_t * subn, osm_log_t * log,
				  IN osm_mgrp_t * mgrp, osm_port_t *port,
				  IN ib_member_rec_t *mcmr, IN boolean_t proxy)
{
	osm_mcm_port_t *mcm_port;
	osm_mcm_alias_guid_t *p_mcm_alias_guid, *p_mcm_alias_guid_check;
	cl_map_item_t *prev_item;
	uint8_t prev_join_state = 0, join_state = mcmr->scope_state;
	uint8_t prev_scope;

	if (OSM_LOG_IS_ACTIVE_V2(log, OSM_LOG_VERBOSE)) {
		char gid_str[INET6_ADDRSTRLEN];
		OSM_LOG(log, OSM_LOG_VERBOSE, "GUID 0x%016" PRIx64
			" Port 0x%016" PRIx64 " joining "
			"MC group %s (mlid 0x%x)\n",
			cl_ntoh64(mcmr->port_gid.unicast.interface_id),
			cl_ntoh64(port->guid),
			inet_ntop(AF_INET6, mgrp->mcmember_rec.mgid.raw,
				  gid_str, sizeof(gid_str)),
			cl_ntoh16(mgrp->mlid));
	}

	mcm_port = osm_mcm_port_new(port, mgrp);
	if (!mcm_port)
		return NULL;

	p_mcm_alias_guid = osm_mcm_alias_guid_new(mcm_port, mcmr, proxy);
	if (!p_mcm_alias_guid) {
		osm_mcm_port_delete(mcm_port);
		return NULL;
	}

	/*
	   prev_item = cl_qmap_insert(...)
	   Pointer to the item in the map with the specified key.  If insertion
	   was successful, this is the pointer to the item.  If an item with the
	   specified key already exists in the map, the pointer to that item is
	   returned.
	 */
	prev_item = cl_qmap_insert(&mgrp->mcm_port_tbl, port->guid,
				   &mcm_port->map_item);

	if (prev_item != &mcm_port->map_item) {	/* mcm port already exists */
		osm_mcm_port_delete(mcm_port);
		mcm_port = (osm_mcm_port_t *) prev_item;

		p_mcm_alias_guid->p_base_mcm_port = (osm_mcm_port_t *) prev_item;
		p_mcm_alias_guid_check = insert_alias_guid(mgrp, p_mcm_alias_guid);
		if (p_mcm_alias_guid_check) {	/* alias GUID already exists */
			p_mcm_alias_guid = p_mcm_alias_guid_check;
			ib_member_get_scope_state(p_mcm_alias_guid->scope_state,
						  &prev_scope, &prev_join_state);
			p_mcm_alias_guid->scope_state =
			    ib_member_set_scope_state(prev_scope,
						      prev_join_state | join_state);
		}
	} else {
		insert_alias_guid(mgrp, p_mcm_alias_guid);
		cl_qlist_insert_tail(&port->mcm_list, &mcm_port->list_item);
		osm_sm_reroute_mlid(&subn->p_osm->sm, mgrp->mlid);
	}

	/* o15.0.1.11: copy the join state */
	mcmr->scope_state = p_mcm_alias_guid->scope_state;

	if ((join_state & IB_JOIN_STATE_FULL) &&
	    !(prev_join_state & IB_JOIN_STATE_FULL) &&
	    ++mgrp->full_members == 1)
		mgrp_send_notice(subn, log, mgrp, 66);

	subn->p_osm->sa.dirty = TRUE;
	return mcm_port;
}