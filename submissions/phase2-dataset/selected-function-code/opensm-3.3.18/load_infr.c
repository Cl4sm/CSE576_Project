static int load_infr(osm_opensm_t * p_osm, ib_inform_info_record_t * iir,
		     osm_mad_addr_t * addr)
{
	osm_infr_t infr, *p_infr;
	int ret = 0;

	infr.h_bind = p_osm->sa.mad_ctrl.h_bind;
	infr.sa = &p_osm->sa;
	/* other possible way to restore mad_addr partially is
	   to extract qpn from InformInfo and to find lid by gid */
	infr.report_addr = *addr;
	infr.inform_record = *iir;

	cl_plock_excl_acquire(&p_osm->lock);
	if (osm_infr_get_by_rec(&p_osm->subn, &p_osm->log, &infr)) {
		OSM_LOG(&p_osm->log, OSM_LOG_VERBOSE,
			"InformInfo Record already exists\n");
		goto _out;
	}

	if (!(p_infr = osm_infr_new(&infr))) {
		OSM_LOG(&p_osm->log, OSM_LOG_ERROR,
			"cannot allocate new infr struct\n");
		ret = -1;
		goto _out;
	}

	OSM_LOG(&p_osm->log, OSM_LOG_DEBUG, "adding InformInfo Record...\n");

	osm_infr_insert_to_db(&p_osm->subn, &p_osm->log, p_infr);

_out:
	cl_plock_release(&p_osm->lock);

	return ret;
}