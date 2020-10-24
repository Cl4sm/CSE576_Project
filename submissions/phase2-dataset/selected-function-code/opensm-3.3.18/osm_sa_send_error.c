void osm_sa_send_error(IN osm_sa_t * sa, IN const osm_madw_t * p_madw,
		       IN ib_net16_t sa_status)
{
	osm_madw_t *p_resp_madw;
	ib_sa_mad_t *p_resp_sa_mad;
	ib_sa_mad_t *p_sa_mad;

	OSM_LOG_ENTER(sa->p_log);

	/* avoid races - if we are exiting - exit */
	if (osm_exit_flag) {
		OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
			"Ignoring requested send after exit\n");
		goto Exit;
	}

	p_resp_madw = osm_mad_pool_get(sa->p_mad_pool,
				       p_madw->h_bind, MAD_BLOCK_SIZE,
				       &p_madw->mad_addr);

	if (p_resp_madw == NULL) {
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 4C07: "
			"Unable to acquire response MAD\n");
		goto Exit;
	}

	p_resp_sa_mad = osm_madw_get_sa_mad_ptr(p_resp_madw);
	p_sa_mad = osm_madw_get_sa_mad_ptr(p_madw);

	/*  Copy the MAD header back into the response mad */
	*p_resp_sa_mad = *p_sa_mad;
	p_resp_sa_mad->status = sa_status;

	if (p_resp_sa_mad->method == IB_MAD_METHOD_SET)
		p_resp_sa_mad->method = IB_MAD_METHOD_GET;
	else if (p_resp_sa_mad->method == IB_MAD_METHOD_GETTABLE)
		p_resp_sa_mad->attr_offset = 0;

	p_resp_sa_mad->method |= IB_MAD_METHOD_RESP_MASK;

	/*
	 * C15-0.1.5 - always return SM_Key = 0 (table 185 p 884)
	 */
	p_resp_sa_mad->sm_key = 0;

	/*
	 * o15-0.2.7 - The PathRecord Attribute ID shall be used in
	 * the response (to a SubnAdmGetMulti(MultiPathRecord)
	 */
	if (p_resp_sa_mad->attr_id == IB_MAD_ATTR_MULTIPATH_RECORD)
		p_resp_sa_mad->attr_id = IB_MAD_ATTR_PATH_RECORD;

	if (OSM_LOG_IS_ACTIVE_V2(sa->p_log, OSM_LOG_FRAMES))
		osm_dump_sa_mad_v2(sa->p_log, p_resp_sa_mad, FILE_ID, OSM_LOG_FRAMES);

	osm_sa_send(sa, p_resp_madw, FALSE);

Exit:
	OSM_LOG_EXIT(sa->p_log);
}