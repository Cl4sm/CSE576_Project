void osm_opensm_destroy_finish(IN osm_opensm_t * p_osm)
{
	osm_db_destroy(&p_osm->db);
	if (p_osm->vl15_constructed && p_osm->mad_pool_constructed)
		osm_vl15_destroy(&p_osm->vl15, &p_osm->mad_pool);
	if (p_osm->mad_pool_constructed)
		osm_mad_pool_destroy(&p_osm->mad_pool);
	p_osm->vl15_constructed = FALSE;
	p_osm->mad_pool_constructed = FALSE;
	osm_vendor_delete(&p_osm->p_vendor);
	osm_subn_destroy(&p_osm->subn);
	cl_disp_destroy(&p_osm->disp);
#ifdef HAVE_LIBPTHREAD
	pthread_cond_destroy(&p_osm->stats.cond);
	pthread_mutex_destroy(&p_osm->stats.mutex);
#else
	cl_event_destroy(&p_osm->stats.event);
#endif
	if (p_osm->node_name_map)
		close_node_name_map(p_osm->node_name_map);
	cl_plock_destroy(&p_osm->lock);

	osm_log_destroy(&p_osm->log);
}