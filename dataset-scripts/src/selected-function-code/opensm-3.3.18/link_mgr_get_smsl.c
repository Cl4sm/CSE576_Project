static uint8_t link_mgr_get_smsl(IN osm_sm_t * sm, IN osm_physp_t * p_physp)
{
	osm_opensm_t *p_osm = sm->p_subn->p_osm;
	struct osm_routing_engine *re = p_osm->routing_engine_used;
	ib_net16_t slid;
	ib_net16_t smlid;
	uint8_t sl;

	OSM_LOG_ENTER(sm->p_log);

	if (!(re && re->path_sl &&
	      (slid = osm_physp_get_base_lid(p_physp)))) {
		/*
		 * Use default SL if routing engine does not provide a
		 * path SL lookup callback.
		 */
		OSM_LOG_EXIT(sm->p_log);
		return sm->p_subn->opt.sm_sl;
	}

	smlid = sm->p_subn->sm_base_lid;

	/* Call into routing engine to find proper SL */
	sl = re->path_sl(re->context, sm->p_subn->opt.sm_sl,
			 slid, smlid);

	OSM_LOG_EXIT(sm->p_log);
	return sl;
}