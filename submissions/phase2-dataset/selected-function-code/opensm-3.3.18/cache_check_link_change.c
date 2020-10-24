static void
cache_check_link_change(osm_ucast_mgr_t * p_mgr,
			osm_physp_t * p_physp_1, osm_physp_t * p_physp_2)
{
	OSM_LOG_ENTER(p_mgr->p_log);
	CL_ASSERT(p_physp_1 && p_physp_2);

	if (!p_mgr->cache_valid)
		goto Exit;

	if (!p_physp_1->p_remote_physp && !p_physp_2->p_remote_physp)
		/* both ports were down - new link */
		goto Exit;

	/* unicast cache cannot tolerate any link location change */

	if ((p_physp_1->p_remote_physp &&
	     p_physp_1->p_remote_physp->p_remote_physp) ||
	    (p_physp_2->p_remote_physp &&
	     p_physp_2->p_remote_physp->p_remote_physp)) {
		OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
			"Link location change discovered\n");
		osm_ucast_cache_invalidate(p_mgr);
		goto Exit;
	}
Exit:
	OSM_LOG_EXIT(p_mgr->p_log);
}