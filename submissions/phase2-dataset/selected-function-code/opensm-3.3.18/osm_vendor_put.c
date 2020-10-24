void
osm_vendor_put(IN osm_bind_handle_t h_bind, IN osm_vend_wrap_t * const p_vw)
{

	osmv_bind_obj_t *p_bo = (osmv_bind_obj_t *) h_bind;
	osm_vendor_t const *p_vend = p_bo->p_vendor;

	if (p_bo->is_closing != TRUE) {
		OSM_LOG_ENTER(p_vend->p_log);

		CL_ASSERT(p_vw);
		CL_ASSERT(p_vw->p_mad);

		if (osm_log_get_level(p_vend->p_log) >= OSM_LOG_DEBUG) {
			osm_log(p_vend->p_log, OSM_LOG_DEBUG,
				"osm_vendor_put: " "Retiring MAD %p.\n",
				p_vw->p_mad);
		}

		free(p_vw->p_mad);
		p_vw->p_mad = NULL;

		OSM_LOG_EXIT(p_vend->p_log);
	}
}