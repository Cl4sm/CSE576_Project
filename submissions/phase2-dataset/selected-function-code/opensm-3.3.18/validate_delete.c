static boolean_t validate_delete(IN osm_sa_t * sa, IN osm_mgrp_t * p_mgrp,
				 IN osm_mad_addr_t * p_mad_addr,
				 IN ib_member_rec_t * p_recvd_mcmember_rec,
				 OUT osm_mcm_alias_guid_t ** pp_mcm_alias_guid)
{
	ib_net64_t portguid;

	portguid = p_recvd_mcmember_rec->port_gid.unicast.interface_id;

	*pp_mcm_alias_guid = osm_mgrp_get_mcm_alias_guid(p_mgrp, portguid);

	/* 1 */
	if (!*pp_mcm_alias_guid) {
		OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
			"Failed to find the port in the MC group\n");
		return FALSE;
	}

	/* 2 */
	if (!(p_recvd_mcmember_rec->scope_state & 0x0F &
	      (*pp_mcm_alias_guid)->scope_state)) {
		OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
			"Could not find any matching bits in the stored "
			"and requested JoinStates\n");
		return FALSE;
	}

	/* 3 */
	if (((p_recvd_mcmember_rec->scope_state & 0x0F) |
	     (0x0F & (*pp_mcm_alias_guid)->scope_state)) !=
	    (0x0F & (*pp_mcm_alias_guid)->scope_state)) {
		OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
			"Some bits in the request JoinState (0x%X) are not "
			"set in the stored port (0x%X)\n",
			(p_recvd_mcmember_rec->scope_state & 0x0F),
			(0x0F & (*pp_mcm_alias_guid)->scope_state));
		return FALSE;
	}

	/* 4 */
	/* Validate according the the proxy_join (o15-0.1.2) */
	if (validate_modify(sa, p_mgrp, p_mad_addr, p_recvd_mcmember_rec,
			    pp_mcm_alias_guid) == FALSE) {
		OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
			"proxy_join validation failure\n");
		return FALSE;
	}
	return TRUE;
}