ib_mad_t *osm_vendor_get(IN osm_bind_handle_t h_bind,
			 IN const uint32_t mad_size,
			 IN osm_vend_wrap_t * const p_vw)
{
	ib_mad_t *p_mad;
	osmv_bind_obj_t *p_bo = (osmv_bind_obj_t *) h_bind;
	osm_vendor_t const *p_vend = p_bo->p_vendor;
	uint32_t act_mad_size;

	OSM_LOG_ENTER(p_vend->p_log);

	CL_ASSERT(p_vw);

	if (mad_size < MAD_BLOCK_SIZE) {
		/* Stupid, but the applications want that! */
		act_mad_size = MAD_BLOCK_SIZE;
	} else {
		act_mad_size = mad_size;
	}

	/* allocate it */
	p_mad = (ib_mad_t *) malloc(act_mad_size);
	if (p_mad == NULL) {
		osm_log(p_vend->p_log, OSM_LOG_ERROR,
			"osm_vendor_get: ERR 7309: "
			"Error Obtaining MAD buffer.\n");
		goto Exit;
	}

	memset(p_mad, 0, act_mad_size);

	if (osm_log_get_level(p_vend->p_log) >= OSM_LOG_DEBUG) {
		osm_log(p_vend->p_log, OSM_LOG_DEBUG,
			"osm_vendor_get: "
			"Allocated MAD %p, size = %u.\n", p_mad, act_mad_size);
	}
	p_vw->p_mad = p_mad;

Exit:
	OSM_LOG_EXIT(p_vend->p_log);
	return (p_mad);
}