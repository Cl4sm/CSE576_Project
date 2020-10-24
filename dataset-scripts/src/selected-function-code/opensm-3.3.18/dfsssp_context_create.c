static dfsssp_context_t *dfsssp_context_create(osm_opensm_t * p_osm,
					       osm_routing_engine_type_t
					       routing_type)
{
	dfsssp_context_t *dfsssp_ctx = NULL;

	/* allocate memory */
	dfsssp_ctx = (dfsssp_context_t *) malloc(sizeof(dfsssp_context_t));
	if (dfsssp_ctx) {
		/* set initial values */
		dfsssp_ctx->routing_type = routing_type;
		dfsssp_ctx->p_mgr = (osm_ucast_mgr_t *) & (p_osm->sm.ucast_mgr);
		dfsssp_ctx->adj_list = NULL;
		dfsssp_ctx->adj_list_size = 0;
		dfsssp_ctx->srcdest2vl_table = NULL;
		dfsssp_ctx->vl_split_count = NULL;
	} else {
		OSM_LOG(p_osm->sm.ucast_mgr.p_log, OSM_LOG_ERROR,
			"ERR AD04: cannot allocate memory for dfsssp_ctx in dfsssp_context_create\n");
		return NULL;
	}

	return dfsssp_ctx;
}