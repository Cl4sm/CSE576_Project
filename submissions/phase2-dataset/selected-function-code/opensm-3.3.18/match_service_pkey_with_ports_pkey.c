static boolean_t
match_service_pkey_with_ports_pkey(IN osm_sa_t * sa,
				   IN const osm_madw_t * p_madw,
				   ib_service_record_t * p_service_rec,
				   ib_net64_t const comp_mask)
{
	boolean_t valid = TRUE;
	osm_physp_t *p_req_physp;
	ib_net64_t service_guid;
	osm_port_t *service_port;

	/* update the requester physical port */
	p_req_physp = osm_get_physp_by_mad_addr(sa->p_log, sa->p_subn,
						osm_madw_get_mad_addr_ptr
						(p_madw));
	if (p_req_physp == NULL) {
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 2404: "
			"Cannot find requester physical port\n");
		valid = FALSE;
		goto Exit;
	}
	OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
		"Requester port GUID 0x%" PRIx64 "\n",
		cl_ntoh64(osm_physp_get_port_guid(p_req_physp)));

	if ((comp_mask & IB_SR_COMPMASK_SPKEY) == IB_SR_COMPMASK_SPKEY) {
		/* We have a ServiceP_Key - check matching on requester port,
		   and ServiceGid port (if such exists) */
		/* Make sure it matches the p_req_physp */
		if (!osm_physp_has_pkey
		    (sa->p_log, p_service_rec->service_pkey, p_req_physp)) {
			valid = FALSE;
			goto Exit;
		}

		/* If unicast, make sure it matches the port of the ServiceGid */
		if (comp_mask & IB_SR_COMPMASK_SGID &&
		    !ib_gid_is_multicast(&p_service_rec->service_gid)) {
			service_guid =
			    p_service_rec->service_gid.unicast.interface_id;
			service_port =
			    osm_get_port_by_alias_guid(sa->p_subn, service_guid);
			if (!service_port) {
				OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 2405: "
					"No port object for port 0x%016" PRIx64
					"\n", cl_ntoh64(service_guid));
				valid = FALSE;
				goto Exit;
			}
			/* check on the table of the default physical port of the service port */
			if (!osm_physp_has_pkey(sa->p_log,
						p_service_rec->service_pkey,
						service_port->p_physp)) {
				valid = FALSE;
				goto Exit;
			}
		}
	}

Exit:
	return valid;
}