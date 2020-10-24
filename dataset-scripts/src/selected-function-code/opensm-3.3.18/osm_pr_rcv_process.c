void osm_pr_rcv_process(IN void *context, IN void *data)
{
	osm_sa_t *sa = context;
	osm_madw_t *p_madw = data;
	const ib_sa_mad_t *p_sa_mad = osm_madw_get_sa_mad_ptr(p_madw);
	ib_path_rec_t *p_pr = ib_sa_mad_get_payload_ptr(p_sa_mad);
	cl_qlist_t pr_list;
	const ib_gid_t *p_sgid = NULL, *p_dgid = NULL;
	const osm_alias_guid_t *p_src_alias_guid, *p_dest_alias_guid;
	const osm_port_t *p_src_port, *p_dest_port;
	osm_port_t *requester_port;
	uint8_t rate, mtu;

	OSM_LOG_ENTER(sa->p_log);

	CL_ASSERT(p_madw);

	CL_ASSERT(p_sa_mad->attr_id == IB_MAD_ATTR_PATH_RECORD);

	/* we only support SubnAdmGet and SubnAdmGetTable methods */
	if (p_sa_mad->method != IB_MAD_METHOD_GET &&
	    p_sa_mad->method != IB_MAD_METHOD_GETTABLE) {
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 1F17: "
			"Unsupported Method (%s) for PathRecord request\n",
			ib_get_sa_method_str(p_sa_mad->method));
		osm_sa_send_error(sa, p_madw, IB_MAD_STATUS_UNSUP_METHOD_ATTR);
		goto Exit;
	}

	/* Validate rate if supplied */
	if ((p_sa_mad->comp_mask & IB_PR_COMPMASK_RATESELEC) &&
	    (p_sa_mad->comp_mask & IB_PR_COMPMASK_RATE)) {
		rate = ib_path_rec_rate(p_pr);
		if (!ib_rate_is_valid(rate)) {
			osm_sa_send_error(sa, p_madw,
					  IB_SA_MAD_STATUS_REQ_INVALID);
			goto Exit;
		}
	}
	/* Validate MTU if supplied */
	if ((p_sa_mad->comp_mask & IB_PR_COMPMASK_MTUSELEC) &&
	    (p_sa_mad->comp_mask & IB_PR_COMPMASK_MTU)) {
		mtu = ib_path_rec_mtu(p_pr);
		if (!ib_mtu_is_valid(mtu)) {
			osm_sa_send_error(sa, p_madw,
					  IB_SA_MAD_STATUS_REQ_INVALID);
			goto Exit;
		}
	}

	/* Make sure either none or both ServiceID parameters are supplied */
	if ((p_sa_mad->comp_mask & IB_PR_COMPMASK_SERVICEID) != 0 &&
	    (p_sa_mad->comp_mask & IB_PR_COMPMASK_SERVICEID) !=
	     IB_PR_COMPMASK_SERVICEID) {
		osm_sa_send_error(sa, p_madw, IB_SA_MAD_STATUS_INSUF_COMPS);
		goto Exit;
	}

	cl_qlist_init(&pr_list);

	/*
	   Most SA functions (including this one) are read-only on the
	   subnet object, so we grab the lock non-exclusively.
	 */
	cl_plock_acquire(sa->p_lock);

	/* update the requester physical port */
	requester_port = osm_get_port_by_mad_addr(sa->p_log, sa->p_subn,
						  osm_madw_get_mad_addr_ptr
						  (p_madw));
	if (requester_port == NULL) {
		cl_plock_release(sa->p_lock);
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 1F16: "
			"Cannot find requester physical port\n");
		goto Exit;
	}

	if (OSM_LOG_IS_ACTIVE_V2(sa->p_log, OSM_LOG_DEBUG)) {
		OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
			"Requester port GUID 0x%" PRIx64 "\n",
			cl_ntoh64(osm_port_get_guid(requester_port)));
		osm_dump_path_record_v2(sa->p_log, p_pr, FILE_ID, OSM_LOG_DEBUG);
	}

	/* Handle multicast destinations separately */
	if ((p_sa_mad->comp_mask & IB_PR_COMPMASK_DGID) &&
	    ib_gid_is_multicast(&p_pr->dgid)) {
		pr_process_multicast(sa, p_sa_mad, &pr_list);
		goto Unlock;
	}

	OSM_LOG(sa->p_log, OSM_LOG_DEBUG, "Unicast destination requested\n");

	if (osm_pr_get_end_points(sa, p_sa_mad,
				  &p_src_alias_guid, &p_dest_alias_guid,
				  &p_src_port, &p_dest_port,
				  &p_sgid, &p_dgid) != IB_SA_MAD_STATUS_SUCCESS)
		goto Unlock;
	/*
	   What happens next depends on the type of endpoint information
	   that was specified....
	 */
	if (p_src_alias_guid) {
		if (p_dest_alias_guid)
			osm_pr_process_pair(sa, p_sa_mad, requester_port,
					    p_src_alias_guid, p_dest_alias_guid,
					    p_sgid, p_dgid, &pr_list);
		else if (!p_dest_port)
			osm_pr_process_half(sa, p_sa_mad, requester_port,
					    p_src_alias_guid, NULL, p_sgid,
					    p_dgid, &pr_list);
		else {
			/* Get all alias GUIDs for the dest port */
			p_dest_alias_guid = (osm_alias_guid_t *) cl_qmap_head(&sa->p_subn->alias_port_guid_tbl);
			while (p_dest_alias_guid !=
			       (osm_alias_guid_t *) cl_qmap_end(&sa->p_subn->alias_port_guid_tbl)) {
				if (osm_get_port_by_alias_guid(sa->p_subn, p_dest_alias_guid->alias_guid) ==
				    p_dest_port)
					osm_pr_process_pair(sa, p_sa_mad,
							    requester_port,
							    p_src_alias_guid,
							    p_dest_alias_guid,
							    p_sgid, p_dgid,
							    &pr_list);
				if (p_sa_mad->method == IB_MAD_METHOD_GET &&
				    cl_qlist_count(&pr_list) > 0)
					break;

				p_dest_alias_guid = (osm_alias_guid_t *) cl_qmap_next(&p_dest_alias_guid->map_item);
			}
		}
	} else {
		if (p_dest_alias_guid)
			osm_pr_process_half(sa, p_sa_mad, requester_port,
					    NULL, p_dest_alias_guid, p_sgid,
					    p_dgid, &pr_list);
		else if (!p_src_port && !p_dest_port)
			/*
			   Katie, bar the door!
			 */
			pr_rcv_process_world(sa, p_sa_mad, requester_port,
					     p_sgid, p_dgid, &pr_list);
		else if (p_src_port && !p_dest_port) {
			/* Get all alias GUIDs for the src port */
			p_src_alias_guid = (osm_alias_guid_t *) cl_qmap_head(&sa->p_subn->alias_port_guid_tbl);
			while (p_src_alias_guid !=
			       (osm_alias_guid_t *) cl_qmap_end(&sa->p_subn->alias_port_guid_tbl)) {
				if (osm_get_port_by_alias_guid(sa->p_subn,
							       p_src_alias_guid->alias_guid) ==
				    p_src_port)
					osm_pr_process_half(sa, p_sa_mad,
							    requester_port,
							    p_src_alias_guid,
							    NULL, p_sgid,
							    p_dgid, &pr_list);
				p_src_alias_guid = (osm_alias_guid_t *) cl_qmap_next(&p_src_alias_guid->map_item);
			}
		} else if (p_dest_port && !p_src_port) {
			/* Get all alias GUIDs for the dest port */
			p_dest_alias_guid = (osm_alias_guid_t *) cl_qmap_head(&sa->p_subn->alias_port_guid_tbl);
			while (p_dest_alias_guid !=
			       (osm_alias_guid_t *) cl_qmap_end(&sa->p_subn->alias_port_guid_tbl)) {
				if (osm_get_port_by_alias_guid(sa->p_subn,
							       p_dest_alias_guid->alias_guid) ==
				    p_dest_port)
					osm_pr_process_half(sa, p_sa_mad,
							    requester_port,
							    NULL,
							    p_dest_alias_guid,
							    p_sgid, p_dgid,
							    &pr_list);
				p_dest_alias_guid = (osm_alias_guid_t *) cl_qmap_next(&p_dest_alias_guid->map_item);
			}
		} else {
			/* Get all alias GUIDs for the src port */
			p_src_alias_guid = (osm_alias_guid_t *) cl_qmap_head(&sa->p_subn->alias_port_guid_tbl);
			while (p_src_alias_guid !=
			       (osm_alias_guid_t *) cl_qmap_end(&sa->p_subn->alias_port_guid_tbl)) {
				if (osm_get_port_by_alias_guid(sa->p_subn,
							       p_src_alias_guid->alias_guid) ==
				    p_src_port) {
					/* Get all alias GUIDs for the dest port */
					p_dest_alias_guid = (osm_alias_guid_t *) cl_qmap_head(&sa->p_subn->alias_port_guid_tbl);
					while (p_dest_alias_guid !=
					       (osm_alias_guid_t *) cl_qmap_end(&sa->p_subn->alias_port_guid_tbl)) {
						if (osm_get_port_by_alias_guid(sa->p_subn,
									       p_dest_alias_guid->alias_guid) ==
						    p_dest_port)
						osm_pr_process_pair(sa,
								    p_sa_mad,
								    requester_port,
								    p_src_alias_guid,
								    p_dest_alias_guid,
								    p_sgid,
								    p_dgid,
								    &pr_list);
						if (p_sa_mad->method == IB_MAD_METHOD_GET &&
						    cl_qlist_count(&pr_list) > 0)
							break;
						p_dest_alias_guid = (osm_alias_guid_t *) cl_qmap_next(&p_dest_alias_guid->map_item);
					}
				}
				if (p_sa_mad->method == IB_MAD_METHOD_GET &&
				    cl_qlist_count(&pr_list) > 0)
					break;
				p_src_alias_guid = (osm_alias_guid_t *) cl_qmap_next(&p_src_alias_guid->map_item);
			}
		}
	}

Unlock:
	cl_plock_release(sa->p_lock);

	/* Now, (finally) respond to the PathRecord request */
	osm_sa_respond(sa, p_madw, sizeof(ib_path_rec_t), &pr_list);

Exit:
	OSM_LOG_EXIT(sa->p_log);
}