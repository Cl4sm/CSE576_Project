osm_vendor_t *osm_vendor_new(IN osm_log_t * const p_log,
			     IN const uint32_t timeout)
{
	ib_api_status_t status;
	osm_vendor_t *p_vend;

	OSM_LOG_ENTER(p_log);

	CL_ASSERT(p_log);

	p_vend = malloc(sizeof(*p_vend));
	if (p_vend != NULL) {
		memset(p_vend, 0, sizeof(*p_vend));

		status = osm_vendor_init(p_vend, p_log, timeout);
		if (status != IB_SUCCESS) {
			osm_vendor_delete(&p_vend);
		}
	} else {
		osm_log(p_vend->p_log, OSM_LOG_ERROR,
			"osm_vendor_new: ERR 7301: "
			"Fail to allocate vendor object.\n");
	}

	OSM_LOG_EXIT(p_log);
	return (p_vend);
}