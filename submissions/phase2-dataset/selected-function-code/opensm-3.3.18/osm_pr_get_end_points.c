ib_net16_t osm_pr_get_end_points(IN osm_sa_t * sa,
					IN const ib_sa_mad_t *sa_mad,
					OUT const osm_alias_guid_t ** pp_src_alias_guid,
					OUT const osm_alias_guid_t ** pp_dest_alias_guid,
					OUT const osm_port_t ** pp_src_port,
					OUT const osm_port_t ** pp_dest_port,
					OUT const ib_gid_t ** pp_sgid,
					OUT const ib_gid_t ** pp_dgid)
{
	const ib_path_rec_t *p_pr = ib_sa_mad_get_payload_ptr(sa_mad);
	ib_net64_t comp_mask = sa_mad->comp_mask;
	ib_net64_t dest_guid;
	ib_net16_t sa_status = IB_SA_MAD_STATUS_SUCCESS;

	OSM_LOG_ENTER(sa->p_log);

	/*
	   Determine what fields are valid and then get a pointer
	   to the source and destination port objects, if possible.
	 */

	/*
	   Check a few easy disqualifying cases up front before getting
	   into the endpoints.
	 */

	*pp_src_alias_guid = NULL;
	*pp_src_port = NULL;
	if (comp_mask & IB_PR_COMPMASK_SGID) {
		if (!ib_gid_is_link_local(&p_pr->sgid)) {
			if (ib_gid_get_subnet_prefix(&p_pr->sgid) !=
			    sa->p_subn->opt.subnet_prefix) {
				/*
				   This 'error' is the client's fault (bad gid)
				   so don't enter it as an error in our own log.
				   Return an error response to the client.
				 */
				OSM_LOG(sa->p_log, OSM_LOG_VERBOSE,
					"Non local SGID subnet prefix 0x%016"
					PRIx64 "\n",
					cl_ntoh64(p_pr->sgid.unicast.prefix));
				sa_status = IB_SA_MAD_STATUS_INVALID_GID;
				goto Exit;
			}
		}

		*pp_src_alias_guid = osm_get_alias_guid_by_guid(sa->p_subn,
								p_pr->sgid.unicast.interface_id);
		if (!*pp_src_alias_guid) {
			/*
			   This 'error' is the client's fault (bad gid) so
			   don't enter it as an error in our own log.
			   Return an error response to the client.
			 */
			OSM_LOG(sa->p_log, OSM_LOG_VERBOSE,
				"No source port with GUID 0x%016" PRIx64 "\n",
				cl_ntoh64(p_pr->sgid.unicast.interface_id));
			sa_status = IB_SA_MAD_STATUS_INVALID_GID;
			goto Exit;
		}
		if (pp_sgid)
			*pp_sgid = &p_pr->sgid;
	} else if (comp_mask & IB_PR_COMPMASK_SLID) {
		*pp_src_port = osm_get_port_by_lid(sa->p_subn, p_pr->slid);
		if (!*pp_src_port) {
			/*
			   This 'error' is the client's fault (bad lid) so
			   don't enter it as an error in our own log.
			   Return an error response to the client.
			 */
			OSM_LOG(sa->p_log, OSM_LOG_VERBOSE, "No source port "
				"with LID %u\n", cl_ntoh16(p_pr->slid));
			sa_status = IB_SA_MAD_STATUS_NO_RECORDS;
			goto Exit;
		}
	}

	*pp_dest_alias_guid = NULL;
	*pp_dest_port = NULL;
	if (comp_mask & IB_PR_COMPMASK_DGID) {
		if (!ib_gid_is_link_local(&p_pr->dgid) &&
		    !ib_gid_is_multicast(&p_pr->dgid) &&
		    ib_gid_get_subnet_prefix(&p_pr->dgid) !=
		    sa->p_subn->opt.subnet_prefix) {
			dest_guid = find_router(sa, p_pr->dgid.unicast.prefix);
			if (!dest_guid) {
				char gid_str[INET6_ADDRSTRLEN];
				OSM_LOG(sa->p_log, OSM_LOG_VERBOSE,
					"Off subnet DGID %s, but router not "
					"found\n",
					inet_ntop(AF_INET6, p_pr->dgid.raw,
						  gid_str, sizeof(gid_str)));
				sa_status = IB_SA_MAD_STATUS_INVALID_GID;
				goto Exit;
			}
			if (pp_dgid)
				*pp_dgid = &p_pr->dgid;
		} else
			dest_guid = p_pr->dgid.unicast.interface_id;

		*pp_dest_alias_guid = osm_get_alias_guid_by_guid(sa->p_subn,
								 dest_guid);
		if (!*pp_dest_alias_guid) {
			/*
			   This 'error' is the client's fault (bad gid) so
			   don't enter it as an error in our own log.
			   Return an error response to the client.
			 */
			OSM_LOG(sa->p_log, OSM_LOG_VERBOSE,
				"No dest port with GUID 0x%016" PRIx64 "\n",
				cl_ntoh64(dest_guid));
			sa_status = IB_SA_MAD_STATUS_INVALID_GID;
			goto Exit;
		}
	} else if (comp_mask & IB_PR_COMPMASK_DLID) {
		*pp_dest_port = osm_get_port_by_lid(sa->p_subn, p_pr->dlid);
		if (!*pp_dest_port) {
			/*
			   This 'error' is the client's fault (bad lid)
			   so don't enter it as an error in our own log.
			   Return an error response to the client.
			 */
			OSM_LOG(sa->p_log, OSM_LOG_VERBOSE, "No dest port "
				"with LID %u\n", cl_ntoh16(p_pr->dlid));
			sa_status = IB_SA_MAD_STATUS_NO_RECORDS;
			goto Exit;
		}
	}

Exit:
	OSM_LOG_EXIT(sa->p_log);
	return sa_status;
}