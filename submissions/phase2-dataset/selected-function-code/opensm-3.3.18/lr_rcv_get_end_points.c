static ib_net16_t lr_rcv_get_end_points(IN osm_sa_t * sa,
					IN const osm_madw_t * p_madw,
					OUT const osm_port_t ** pp_src_port,
					OUT const osm_port_t ** pp_dest_port)
{
	const ib_link_record_t *p_lr;
	const ib_sa_mad_t *p_sa_mad;
	ib_net64_t comp_mask;
	ib_net16_t sa_status = IB_SA_MAD_STATUS_SUCCESS;

	OSM_LOG_ENTER(sa->p_log);

	/*
	   Determine what fields are valid and then get a pointer
	   to the source and destination port objects, if possible.
	 */
	p_sa_mad = osm_madw_get_sa_mad_ptr(p_madw);
	p_lr = (ib_link_record_t *) ib_sa_mad_get_payload_ptr(p_sa_mad);

	comp_mask = p_sa_mad->comp_mask;
	*pp_src_port = NULL;
	*pp_dest_port = NULL;

	if (comp_mask & IB_LR_COMPMASK_FROM_LID) {
		*pp_src_port = osm_get_port_by_lid(sa->p_subn, p_lr->from_lid);
		if (!*pp_src_port) {
			/*
			   This 'error' is the client's fault (bad lid) so
			   don't enter it as an error in our own log.
			   Return an error response to the client.
			 */
			OSM_LOG(sa->p_log, OSM_LOG_VERBOSE,
				"No source port with LID %u\n",
				cl_ntoh16(p_lr->from_lid));

			sa_status = IB_SA_MAD_STATUS_NO_RECORDS;
			goto Exit;
		}
	}

	if (comp_mask & IB_LR_COMPMASK_TO_LID) {
		*pp_dest_port = osm_get_port_by_lid(sa->p_subn, p_lr->to_lid);
		if (!*pp_dest_port) {
			/*
			   This 'error' is the client's fault (bad lid) so
			   don't enter it as an error in our own log.
			   Return an error response to the client.
			 */
			OSM_LOG(sa->p_log, OSM_LOG_VERBOSE,
				"No dest port with LID %u\n",
				cl_ntoh16(p_lr->to_lid));

			sa_status = IB_SA_MAD_STATUS_NO_RECORDS;
			goto Exit;
		}
	}

Exit:
	OSM_LOG_EXIT(sa->p_log);
	return sa_status;
}