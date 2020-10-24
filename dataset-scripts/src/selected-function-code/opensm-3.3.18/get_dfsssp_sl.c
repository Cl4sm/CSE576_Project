static uint8_t get_dfsssp_sl(void *context, uint8_t hint_for_default_sl,
			     const ib_net16_t slid, const ib_net16_t dlid)
{
	dfsssp_context_t *dfsssp_ctx = (dfsssp_context_t *) context;
	osm_port_t *src_port, *dest_port;
	vltable_t *srcdest2vl_table = NULL;
	uint8_t *vl_split_count = NULL;
	osm_ucast_mgr_t *p_mgr = NULL;
	int32_t res = 0;

	if (dfsssp_ctx
	    && dfsssp_ctx->routing_type == OSM_ROUTING_ENGINE_TYPE_DFSSSP) {
		p_mgr = (osm_ucast_mgr_t *) dfsssp_ctx->p_mgr;
		srcdest2vl_table = (vltable_t *) (dfsssp_ctx->srcdest2vl_table);
		vl_split_count = (uint8_t *) (dfsssp_ctx->vl_split_count);
	}
	else
		return hint_for_default_sl;

	src_port = osm_get_port_by_lid(p_mgr->p_subn, slid);
	if (!src_port)
		return hint_for_default_sl;

	dest_port = osm_get_port_by_lid(p_mgr->p_subn, dlid);
	if (!dest_port)
		return hint_for_default_sl;

	if (!srcdest2vl_table)
		return hint_for_default_sl;

	res = vltable_get_vl(srcdest2vl_table, slid, dlid);

	/* we will randomly distribute the traffic over multiple VLs if
	   necessary for good balancing; therefore vl_split_count provides
	   the number of VLs to use for certain traffic
	 */
	if (res > -1) {
		if (vl_split_count[res] > 1)
			return (uint8_t) (res + rand()%(vl_split_count[res]));
		else
			return (uint8_t) res;
	} else
		return hint_for_default_sl;
}