static boolean_t validate_sr(IN osm_sa_t * sa, IN const osm_madw_t * p_madw)
{
	boolean_t valid = TRUE;
	ib_sa_mad_t *p_sa_mad;
	ib_service_record_t *p_recvd_service_rec;

	OSM_LOG_ENTER(sa->p_log);

	p_sa_mad = osm_madw_get_sa_mad_ptr(p_madw);
	p_recvd_service_rec =
	    (ib_service_record_t *) ib_sa_mad_get_payload_ptr(p_sa_mad);

	valid = match_service_pkey_with_ports_pkey(sa, p_madw,
						   p_recvd_service_rec,
						   p_sa_mad->comp_mask);
	if (!valid) {
		OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
			"No Match for Service Pkey\n");
		valid = FALSE;
		goto Exit;
	}

	valid = match_name_to_key_association(sa, p_recvd_service_rec,
					      p_sa_mad->comp_mask);
	if (!valid) {
		OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
			"Service Record Name to key matching failed\n");
		valid = FALSE;
		goto Exit;
	}

Exit:
	OSM_LOG_EXIT(sa->p_log);
	return valid;
}