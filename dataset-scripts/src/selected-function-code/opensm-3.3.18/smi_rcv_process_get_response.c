static void smi_rcv_process_get_response(IN osm_sm_t * sm,
					 IN const osm_madw_t * p_madw)
{
	const ib_smp_t *p_smp;
	const ib_sm_info_t *p_smi;
	cl_qmap_t *p_sm_tbl;
	osm_port_t *p_port;
	ib_net64_t port_guid;
	osm_remote_sm_t *p_sm;

	OSM_LOG_ENTER(sm->p_log);

	CL_ASSERT(p_madw);

	p_smp = osm_madw_get_smp_ptr(p_madw);

	if (p_smp->method != IB_MAD_METHOD_GET_RESP) {
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 2F11: "
			"Unsupported response method 0x%X\n", p_smp->method);
		goto Exit;
	}

	p_smi = ib_smp_get_payload_ptr(p_smp);
	p_sm_tbl = &sm->p_subn->sm_guid_tbl;
	port_guid = p_smi->guid;

	osm_dump_sm_info_v2(sm->p_log, p_smi, FILE_ID, OSM_LOG_DEBUG);

	/* Check that the sm_key of the found SM is the same as ours,
	   or is zero. If not - OpenSM cannot continue with configuration!. */
	if (p_smi->sm_key != 0 && p_smi->sm_key != sm->p_subn->opt.sm_key) {
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 2F18: "
			"Got SM with sm_key that doesn't match our "
			"local key. Exiting\n");
		osm_log_v2(sm->p_log, OSM_LOG_SYS, FILE_ID,
			   "Found remote SM with non-matching sm_key. Exiting\n");
		osm_exit_flag = TRUE;
		goto Exit;
	}

	/* Determine if we already have another SM object for this SM. */
	CL_PLOCK_EXCL_ACQUIRE(sm->p_lock);

	p_port = osm_get_port_by_guid(sm->p_subn, port_guid);
	if (!p_port) {
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 2F12: "
			"No port object for this SM\n");
		goto _unlock_and_exit;
	}

	if (osm_port_get_guid(p_port) != p_smi->guid) {
		OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 2F13: "
			"Bogus SM port GUID, Expected 0x%016" PRIx64
			", Received 0x%016" PRIx64 "\n",
			cl_ntoh64(osm_port_get_guid(p_port)),
			cl_ntoh64(p_smi->guid));
		goto _unlock_and_exit;
	}

	if (port_guid == sm->p_subn->sm_port_guid) {
		OSM_LOG(sm->p_log, OSM_LOG_VERBOSE,
			"Self query response received - SM port 0x%016" PRIx64
			"\n", cl_ntoh64(port_guid));
		goto _unlock_and_exit;
	}

	p_sm = (osm_remote_sm_t *) cl_qmap_get(p_sm_tbl, port_guid);
	if (p_sm == (osm_remote_sm_t *) cl_qmap_end(p_sm_tbl)) {
		p_sm = malloc(sizeof(*p_sm));
		if (p_sm == NULL) {
			OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 2F14: "
				"Unable to allocate SM object\n");
			goto _unlock_and_exit;
		}

		osm_remote_sm_init(p_sm, p_smi);

		cl_qmap_insert(p_sm_tbl, port_guid, &p_sm->map_item);
	} else
		/* We already know this SM. Update the SMInfo attribute. */
		p_sm->smi = *p_smi;

	smi_rcv_process_get_sm(sm, p_sm,
			       osm_madw_get_smi_context_ptr(p_madw)->
			       light_sweep);

_unlock_and_exit:
	CL_PLOCK_RELEASE(sm->p_lock);

Exit:
	OSM_LOG_EXIT(sm->p_log);
}