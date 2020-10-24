ib_api_status_t osm_req_get(IN osm_sm_t * sm, IN const osm_dr_path_t * p_path,
			    IN ib_net16_t attr_id, IN ib_net32_t attr_mod,
			    IN boolean_t find_mkey, ib_net64_t m_key,
			    IN cl_disp_msgid_t err_msg,
			    IN const osm_madw_context_t * p_context)
{
	osm_madw_t *p_madw;
	ib_api_status_t status = IB_SUCCESS;
	ib_net64_t m_key_calc;
	ib_net64_t tid;

	CL_ASSERT(sm);

	OSM_LOG_ENTER(sm->p_log);

	CL_ASSERT(p_path);
	CL_ASSERT(attr_id);

	/* do nothing if we are exiting ... */
	if (osm_exit_flag)
		goto Exit;

	/* p_context may be NULL. */

	p_madw = osm_mad_pool_get(sm->p_mad_pool, sm->mad_ctrl.h_bind,
				  MAD_BLOCK_SIZE, NULL);
	if (p_madw == NULL) {
		OSM_LOG(sm->p_log, OSM_LOG_ERROR,
			"ERR 1101: Unable to acquire MAD\n");
		status = IB_INSUFFICIENT_RESOURCES;
		goto Exit;
	}

	tid = cl_hton64((uint64_t) cl_atomic_inc(&sm->sm_trans_id)
						 & (uint64_t)(0xFFFFFFFF));
	if (tid == 0)
		tid = cl_hton64((uint64_t) cl_atomic_inc(&sm->sm_trans_id)
							 & (uint64_t)(0xFFFFFFFF));

	if (sm->p_subn->opt.m_key_lookup == TRUE) {
		if (find_mkey == TRUE)
			m_key_calc = req_determine_mkey(sm, p_path);
		else
			m_key_calc = m_key;
	} else
		m_key_calc = sm->p_subn->opt.m_key;

	OSM_LOG(sm->p_log, OSM_LOG_DEBUG,
		"Getting %s (0x%X), modifier 0x%X, TID 0x%" PRIx64
		", MKey 0x%016" PRIx64 "\n",
		ib_get_sm_attr_str(attr_id), cl_ntoh16(attr_id),
		cl_ntoh32(attr_mod), cl_ntoh64(tid), cl_ntoh64(m_key_calc));

	ib_smp_init_new(osm_madw_get_smp_ptr(p_madw), IB_MAD_METHOD_GET,
			tid, attr_id, attr_mod, p_path->hop_count,
			m_key_calc, p_path->path,
			IB_LID_PERMISSIVE, IB_LID_PERMISSIVE);

	p_madw->mad_addr.dest_lid = IB_LID_PERMISSIVE;
	p_madw->mad_addr.addr_type.smi.source_lid = IB_LID_PERMISSIVE;
	p_madw->resp_expected = TRUE;
	p_madw->fail_msg = err_msg;

	/*
	   Fill in the mad wrapper context for the recipient.
	   In this case, the only thing the recipient needs is the
	   guid value.
	 */

	if (p_context)
		p_madw->context = *p_context;

	osm_vl15_post(sm->p_vl15, p_madw);

Exit:
	OSM_LOG_EXIT(sm->p_log);
	return status;
}