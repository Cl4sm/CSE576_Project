boolean_t osm_mgrp_remove_port(osm_subn_t * subn, osm_log_t * log, osm_mgrp_t * mgrp,
			  osm_mcm_alias_guid_t * mcm_alias_guid,
			  ib_member_rec_t *mcmr)
{
	uint8_t join_state = mcmr->scope_state & 0xf;
	uint8_t port_join_state, new_join_state;
	boolean_t mgrp_deleted = FALSE;

	/*
	 * according to the same o15-0.1.14 we get the stored
	 * JoinState and the request JoinState and they must be
	 * opposite to leave - otherwise just update it
	 */
	port_join_state = mcm_alias_guid->scope_state & 0x0F;
	new_join_state = port_join_state & ~join_state;

	if (OSM_LOG_IS_ACTIVE_V2(log, OSM_LOG_VERBOSE)) {
		char gid_str[INET6_ADDRSTRLEN];
		OSM_LOG(log, OSM_LOG_VERBOSE,
			"GUID 0x%" PRIx64 " Port 0x%" PRIx64
			" leaving MC group %s (mlid 0x%x)\n",
			cl_ntoh64(mcm_alias_guid->alias_guid),
			cl_ntoh64(mcm_alias_guid->p_base_mcm_port->port->guid),
			inet_ntop(AF_INET6, mgrp->mcmember_rec.mgid.raw,
				  gid_str, sizeof(gid_str)),
			cl_ntoh16(mgrp->mlid));
	}

	if (new_join_state & IB_JOIN_STATE_FULL ||
	    (new_join_state &&
	     (mgrp->full_members > (port_join_state & IB_JOIN_STATE_FULL) ? 1 : 0))) {
		mcm_alias_guid->scope_state =
		    new_join_state | (mcm_alias_guid->scope_state & 0xf0);
		OSM_LOG(log, OSM_LOG_DEBUG,
			"updating GUID 0x%" PRIx64 " port 0x%" PRIx64
			" JoinState 0x%x -> 0x%x\n",
			cl_ntoh64(mcm_alias_guid->alias_guid),
			cl_ntoh64(mcm_alias_guid->p_base_mcm_port->port->guid),
			port_join_state, new_join_state);
		mcmr->scope_state = mcm_alias_guid->scope_state;
	} else {
		mcmr->scope_state = mcm_alias_guid->scope_state & 0xf0;
		OSM_LOG(log, OSM_LOG_DEBUG, "removing alias GUID 0x%" PRIx64 "\n",
			cl_ntoh64(mcm_alias_guid->alias_guid));
		cl_qmap_remove_item(&mgrp->mcm_alias_port_tbl,
				    &mcm_alias_guid->map_item);
		if (is_qmap_empty_for_port(&mgrp->mcm_alias_port_tbl,
					   mcm_alias_guid->p_base_mcm_port->port)) { /* last alias in mcast group for this port */
			OSM_LOG(log, OSM_LOG_DEBUG, "removing port 0x%" PRIx64 "\n",
				cl_ntoh64(mcm_alias_guid->p_base_mcm_port->port->guid));
			cl_qmap_remove_item(&mgrp->mcm_port_tbl,
					    &mcm_alias_guid->p_base_mcm_port->map_item);
			cl_qlist_remove_item(&mcm_alias_guid->p_base_mcm_port->port->mcm_list,
					     &mcm_alias_guid->p_base_mcm_port->list_item);
			if (is_qmap_empty_for_mcm_port(&mgrp->mcm_alias_port_tbl,
						       mcm_alias_guid->p_base_mcm_port)) /* last alias in mcast group for this mcm port */
				osm_mcm_port_delete(mcm_alias_guid->p_base_mcm_port);
			osm_sm_reroute_mlid(&subn->p_osm->sm, mgrp->mlid);
		}
		osm_mcm_alias_guid_delete(&mcm_alias_guid);
	}

	/* no more full members so the group will be deleted after re-route
	   but only if it is not a well known group */
	if ((port_join_state & IB_JOIN_STATE_FULL) &&
	    !(new_join_state & IB_JOIN_STATE_FULL) &&
	    --mgrp->full_members == 0) {
		mgrp_send_notice(subn, log, mgrp, 67);
		osm_mgrp_cleanup(subn, mgrp);
		mgrp_deleted = TRUE;
	}

	subn->p_osm->sa.dirty = TRUE;

	return (mgrp_deleted);
}