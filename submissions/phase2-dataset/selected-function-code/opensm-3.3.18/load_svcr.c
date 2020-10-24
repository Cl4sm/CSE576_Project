static int load_svcr(osm_opensm_t * p_osm, ib_service_record_t * sr,
		     uint32_t modified_time, uint32_t lease_period)
{
	osm_svcr_t *p_svcr;
	int ret = 0;

	cl_plock_excl_acquire(&p_osm->lock);

	if (osm_svcr_get_by_rid(&p_osm->subn, &p_osm->log, sr)) {
		OSM_LOG(&p_osm->log, OSM_LOG_VERBOSE,
			"ServiceRecord already exists\n");
		goto _out;
	}

	if (!(p_svcr = osm_svcr_new(sr))) {
		OSM_LOG(&p_osm->log, OSM_LOG_ERROR,
			"cannot allocate new service struct\n");
		ret = -1;
		goto _out;
	}

	p_svcr->modified_time = modified_time;
	p_svcr->lease_period = lease_period;

	OSM_LOG(&p_osm->log, OSM_LOG_DEBUG, "adding ServiceRecord...\n");

	osm_svcr_insert_to_db(&p_osm->subn, &p_osm->log, p_svcr);

	if (lease_period != 0xffffffff)
		cl_timer_trim(&p_osm->sa.sr_timer, 1000);

_out:
	cl_plock_release(&p_osm->lock);

	return ret;
}