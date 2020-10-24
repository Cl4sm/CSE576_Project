static boolean_t validate_modify(IN osm_sa_t * sa, IN osm_mgrp_t * p_mgrp,
				 IN osm_mad_addr_t * p_mad_addr,
				 IN ib_member_rec_t * p_recvd_mcmember_rec,
				 OUT osm_mcm_alias_guid_t ** pp_mcm_alias_guid)
{
	ib_net64_t portguid;
	ib_gid_t request_gid;
	osm_physp_t *p_request_physp;
	ib_api_status_t res;

	portguid = p_recvd_mcmember_rec->port_gid.unicast.interface_id;

	*pp_mcm_alias_guid = osm_mgrp_get_mcm_alias_guid(p_mgrp, portguid);

	/* o15-0.2.1: If this is a new port being added - nothing to check */
	if (!*pp_mcm_alias_guid) {
		OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
			"This is a new port in the MC group\n");
		return TRUE;
	}

	/* We validate the request according the the proxy_join.
	   Check if the proxy_join is set or not */
	if ((*pp_mcm_alias_guid)->proxy_join == FALSE) {
		/* The proxy_join is not set. Modifying can by done only
		   if the requester GID == PortGID */
		res = osm_get_gid_by_mad_addr(sa->p_log, sa->p_subn, p_mad_addr,
					      &request_gid);
		if (res != IB_SUCCESS) {
			OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
				"Could not find port for requested address\n");
			return FALSE;
		}

		if ((*pp_mcm_alias_guid)->p_base_mcm_port->port->guid !=
		    request_gid.unicast.interface_id ||
		    (*pp_mcm_alias_guid)->port_gid.unicast.prefix !=
		    request_gid.unicast.prefix) {
			ib_gid_t base_port_gid;
			char gid_str[INET6_ADDRSTRLEN];
			char gid_str2[INET6_ADDRSTRLEN];

			base_port_gid.unicast.prefix = (*pp_mcm_alias_guid)->port_gid.unicast.prefix;
			base_port_gid.unicast.interface_id = (*pp_mcm_alias_guid)->p_base_mcm_port->port->guid;
			OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
				"No ProxyJoin but different ports: stored:"
				"%s request:%s\n",
				inet_ntop(AF_INET6, base_port_gid.raw, gid_str,
					  sizeof gid_str),
				inet_ntop(AF_INET6, request_gid.raw, gid_str2,
					  sizeof gid_str2));
			return FALSE;
		}
	} else {
		/* The proxy_join is set. Modification allowed only if the
		   requester is part of the partition for this MCMemberRecord */
		p_request_physp = osm_get_physp_by_mad_addr(sa->p_log,
							    sa->p_subn,
							    p_mad_addr);
		if (p_request_physp == NULL)
			return FALSE;

		if (!osm_physp_has_pkey(sa->p_log, p_mgrp->mcmember_rec.pkey,
					p_request_physp)) {
			/* the request port is not part of the partition for this mgrp */
			OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
				"Requesting port 0x%016" PRIx64 " has no P_Key 0x%04x\n",
				cl_ntoh64(p_request_physp->port_guid),
				cl_ntoh16(p_mgrp->mcmember_rec.pkey));
			return FALSE;
		}
	}
	return TRUE;
}