static void vl15_send_mad(osm_vl15_t * p_vl, osm_madw_t * p_madw)
{
	ib_api_status_t status;
	boolean_t resp_expected = p_madw->resp_expected;
	ib_smp_t * p_smp;
	ib_net16_t attr_id;
	uint8_t method;

	p_smp = osm_madw_get_smp_ptr(p_madw);
	method = p_smp->method;
	attr_id = p_smp->attr_id;

	/*
	   Non-response-expected mads are not throttled on the wire
	   since we can have no confirmation that they arrived
	   at their destination.
	 */
	if (resp_expected)
		/*
		   Note that other threads may not see the response MAD
		   arrive before send() even returns.
		   In that case, the wire count would temporarily go negative.
		   To avoid this confusion, preincrement the counts on the
		   assumption that send() will succeed.
		 */
		cl_atomic_inc(&p_vl->p_stats->qp0_mads_outstanding_on_wire);
	else
		cl_atomic_inc(&p_vl->p_stats->qp0_unicasts_sent);

	cl_atomic_inc(&p_vl->p_stats->qp0_mads_sent);

	status = osm_vendor_send(osm_madw_get_bind_handle(p_madw),
				 p_madw, p_madw->resp_expected);

	if (status == IB_SUCCESS) {
		OSM_LOG(p_vl->p_log, OSM_LOG_DEBUG,
			"%u QP0 MADs on wire, %u outstanding, "
			"%u unicasts sent, %u total sent\n",
			p_vl->p_stats->qp0_mads_outstanding_on_wire,
			p_vl->p_stats->qp0_mads_outstanding,
			p_vl->p_stats->qp0_unicasts_sent,
			p_vl->p_stats->qp0_mads_sent);
		return;
	}

	OSM_LOG(p_vl->p_log, OSM_LOG_ERROR, "ERR 3E03: "
		"MAD send failed (%s)\n", ib_get_err_str(status));

	/*
	   The MAD was never successfully sent, so
	   fix up the pre-incremented count values.
	 */

	/* Decrement qp0_mads_sent that were incremented in the code above.
	   qp0_mads_outstanding will be decremented by send error callback
	   (called by osm_vendor_send() */
	cl_atomic_dec(&p_vl->p_stats->qp0_mads_sent);
	if (!resp_expected) {
		cl_atomic_dec(&p_vl->p_stats->qp0_unicasts_sent);
		return;
	}

	/* need to cause heavy-sweep if resp_expected MAD sending failed */
	OSM_LOG(p_vl->p_log, OSM_LOG_ERROR, "ERR 3E04: "
		"%s method failed for attribute 0x%X (%s)\n",
		method == IB_MAD_METHOD_SET ? "SET" : "GET",
		cl_ntoh16(attr_id), ib_get_sm_attr_str(attr_id));

	p_vl->p_subn->subnet_initialization_error = TRUE;

}