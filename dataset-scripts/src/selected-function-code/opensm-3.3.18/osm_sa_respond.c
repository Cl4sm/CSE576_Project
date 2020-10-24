void osm_sa_respond(osm_sa_t *sa, osm_madw_t *madw, size_t attr_size,
		    cl_qlist_t *list)
{
	cl_list_item_t *item;
	osm_madw_t *resp_madw;
	ib_sa_mad_t *sa_mad, *resp_sa_mad;
	unsigned num_rec, i;
#ifndef VENDOR_RMPP_SUPPORT
	unsigned trim_num_rec;
#endif
	unsigned char *p;

	sa_mad = osm_madw_get_sa_mad_ptr(madw);
	num_rec = cl_qlist_count(list);

	/*
	 * C15-0.1.30:
	 * If we do a SubnAdmGet and got more than one record it is an error!
	 */
	if (sa_mad->method == IB_MAD_METHOD_GET && num_rec > 1) {
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 4C05: "
			"Got %u records for SubnAdmGet(%s) comp_mask 0x%016" PRIx64 "\n",
			num_rec, ib_get_sa_attr_str(sa_mad->attr_id),
			cl_ntoh64(sa_mad->comp_mask));
		osm_sa_send_error(sa, madw, IB_SA_MAD_STATUS_TOO_MANY_RECORDS);
		goto Exit;
	}

#ifndef VENDOR_RMPP_SUPPORT
	trim_num_rec = (MAD_BLOCK_SIZE - IB_SA_MAD_HDR_SIZE) / attr_size;
	if (trim_num_rec < num_rec) {
		OSM_LOG(sa->p_log, OSM_LOG_VERBOSE,
			"Number of records:%u trimmed to:%u to fit in one MAD\n",
			num_rec, trim_num_rec);
		num_rec = trim_num_rec;
	}
#endif

	OSM_LOG(sa->p_log, OSM_LOG_DEBUG, "Returning %u records\n", num_rec);

	if (sa_mad->method == IB_MAD_METHOD_GET && num_rec == 0) {
		osm_sa_send_error(sa, madw, IB_SA_MAD_STATUS_NO_RECORDS);
		goto Exit;
	}

	/*
	 * Get a MAD to reply. Address of Mad is in the received mad_wrapper
	 */
	resp_madw = osm_mad_pool_get(sa->p_mad_pool, madw->h_bind,
				     num_rec * attr_size + IB_SA_MAD_HDR_SIZE,
				     &madw->mad_addr);
	if (!resp_madw) {
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 4C06: "
			"osm_mad_pool_get failed\n");
		osm_sa_send_error(sa, madw, IB_SA_MAD_STATUS_NO_RESOURCES);
		goto Exit;
	}

	resp_sa_mad = osm_madw_get_sa_mad_ptr(resp_madw);

	/*
	   Copy the MAD header back into the response mad.
	   Set the 'R' bit and the payload length,
	   Then copy all records from the list into the response payload.
	 */

	memcpy(resp_sa_mad, sa_mad, IB_SA_MAD_HDR_SIZE);
	if (resp_sa_mad->method == IB_MAD_METHOD_SET)
		resp_sa_mad->method = IB_MAD_METHOD_GET;
	resp_sa_mad->method |= IB_MAD_METHOD_RESP_MASK;
	/* C15-0.1.5 - always return SM_Key = 0 (table 185 p 884) */
	resp_sa_mad->sm_key = 0;

	/* Fill in the offset (paylen will be done by the rmpp SAR) */
	resp_sa_mad->attr_offset = num_rec ? ib_get_attr_offset(attr_size) : 0;

	p = ib_sa_mad_get_payload_ptr(resp_sa_mad);

#ifndef VENDOR_RMPP_SUPPORT
	/* we support only one packet RMPP - so we will set the first and
	   last flags for gettable */
	if (resp_sa_mad->method == IB_MAD_METHOD_GETTABLE_RESP) {
		resp_sa_mad->rmpp_type = IB_RMPP_TYPE_DATA;
		resp_sa_mad->rmpp_flags =
		    IB_RMPP_FLAG_FIRST | IB_RMPP_FLAG_LAST |
		    IB_RMPP_FLAG_ACTIVE;
	}
#else
	/* forcefully define the packet as RMPP one */
	if (resp_sa_mad->method == IB_MAD_METHOD_GETTABLE_RESP)
		resp_sa_mad->rmpp_flags = IB_RMPP_FLAG_ACTIVE;
#endif

	for (i = 0; i < num_rec; i++) {
		item = cl_qlist_remove_head(list);
		memcpy(p, ((osm_sa_item_t *)item)->resp.data, attr_size);
		p += attr_size;
		free(item);
	}

	osm_dump_sa_mad_v2(sa->p_log, resp_sa_mad, FILE_ID, OSM_LOG_FRAMES);
	osm_sa_send(sa, resp_madw, FALSE);

Exit:
	/* need to set the mem free ... */
	item = cl_qlist_remove_head(list);
	while (item != cl_qlist_end(list)) {
		free(item);
		item = cl_qlist_remove_head(list);
	}
}