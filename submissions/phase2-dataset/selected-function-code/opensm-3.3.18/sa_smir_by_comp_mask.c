static void sa_smir_by_comp_mask(IN osm_sa_t * sa,
				 IN const osm_remote_sm_t * p_rem_sm,
				 osm_smir_search_ctxt_t * p_ctxt)
{
	const ib_sminfo_record_t *const p_rcvd_rec = p_ctxt->p_rcvd_rec;
	const osm_physp_t *const p_req_physp = p_ctxt->p_req_physp;
	ib_net64_t const comp_mask = p_ctxt->comp_mask;
	osm_port_t *p_port;

	OSM_LOG_ENTER(sa->p_log);

	if (comp_mask & IB_SMIR_COMPMASK_GUID) {
		if (p_rem_sm->smi.guid != p_rcvd_rec->sm_info.guid)
			goto Exit;
	}

	if (comp_mask & IB_SMIR_COMPMASK_PRIORITY) {
		if (ib_sminfo_get_priority(&p_rem_sm->smi) !=
		    ib_sminfo_get_priority(&p_rcvd_rec->sm_info))
			goto Exit;
	}

	if (comp_mask & IB_SMIR_COMPMASK_SMSTATE) {
		if (ib_sminfo_get_state(&p_rem_sm->smi) !=
		    ib_sminfo_get_state(&p_rcvd_rec->sm_info))
			goto Exit;
	}

	/* Implement any other needed search cases */
	p_port = osm_get_port_by_guid(sa->p_subn, p_rem_sm->smi.guid);

        if (p_port == NULL) {
                OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 2810: "
                        "No port for remote sm\n");
                goto Exit;
        }

	smir_rcv_new_smir(sa, p_port, p_ctxt->p_list,
			  p_rem_sm->smi.guid, p_rem_sm->smi.act_count,
			  p_rem_sm->smi.pri_state, p_req_physp);

Exit:
	OSM_LOG_EXIT(sa->p_log);
}