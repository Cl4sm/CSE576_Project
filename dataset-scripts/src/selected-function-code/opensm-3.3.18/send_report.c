static ib_api_status_t send_report(IN osm_infr_t * p_infr_rec,	/* the informinfo */
				   IN ib_mad_notice_attr_t * p_ntc	/* notice to send */
    )
{
	osm_madw_t *p_report_madw;
	ib_mad_notice_attr_t *p_report_ntc;
	ib_mad_t *p_mad;
	ib_sa_mad_t *p_sa_mad;
	static atomic32_t trap_fwd_trans_id = 0x02DAB000;
	ib_api_status_t status = IB_SUCCESS;
	osm_log_t *p_log = p_infr_rec->sa->p_log;
	ib_net64_t tid;

	OSM_LOG_ENTER(p_log);

	/* HACK: who switches or uses the src and dest GIDs in the grh_info ?? */

	/* it is better to use LIDs since the GIDs might not be there for SMI traps */
	OSM_LOG(p_log, OSM_LOG_DEBUG, "Forwarding Notice Event from LID %u"
		" to InformInfo LID %u GUID 0x%" PRIx64 ", TID 0x%X\n",
		cl_ntoh16(p_ntc->issuer_lid),
		cl_ntoh16(p_infr_rec->report_addr.dest_lid),
		cl_ntoh64(p_infr_rec->inform_record.subscriber_gid.unicast.interface_id),
		trap_fwd_trans_id);

	/* get the MAD to send */
	p_report_madw = osm_mad_pool_get(p_infr_rec->sa->p_mad_pool,
					 p_infr_rec->h_bind, MAD_BLOCK_SIZE,
					 &(p_infr_rec->report_addr));

	if (!p_report_madw) {
		OSM_LOG(p_log, OSM_LOG_ERROR, "ERR 0203: "
			"Cannot send report to LID %u, osm_mad_pool_get failed\n",
			cl_ntoh16(p_infr_rec->report_addr.dest_lid));
		status = IB_ERROR;
		goto Exit;
	}

	p_report_madw->resp_expected = TRUE;

	/* advance trap trans id (cant simply ++ on some systems inside ntoh) */
	tid = cl_hton64((uint64_t) cl_atomic_inc(&trap_fwd_trans_id) &
			(uint64_t) (0xFFFFFFFF));
	if (trap_fwd_trans_id == 0)
		tid = cl_hton64((uint64_t) cl_atomic_inc(&trap_fwd_trans_id) &
				(uint64_t) (0xFFFFFFFF));
	p_mad = osm_madw_get_mad_ptr(p_report_madw);
	ib_mad_init_new(p_mad, IB_MCLASS_SUBN_ADM, 2, IB_MAD_METHOD_REPORT,
			tid, IB_MAD_ATTR_NOTICE, 0);

	p_sa_mad = osm_madw_get_sa_mad_ptr(p_report_madw);

	p_report_ntc = (ib_mad_notice_attr_t *) & (p_sa_mad->data);

	/* copy the notice */
	*p_report_ntc = *p_ntc;

	/* The TRUE is for: response is expected */
	osm_sa_send(p_infr_rec->sa, p_report_madw, TRUE);

Exit:
	OSM_LOG_EXIT(p_log);
	return status;
}