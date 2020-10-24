ib_api_status_t osm_lid_mgr_init(IN osm_lid_mgr_t * p_mgr, IN osm_sm_t * sm)
{
	ib_api_status_t status = IB_SUCCESS;

	OSM_LOG_ENTER(sm->p_log);

	osm_lid_mgr_construct(p_mgr);

	p_mgr->sm = sm;
	p_mgr->p_log = sm->p_log;
	p_mgr->p_subn = sm->p_subn;
	p_mgr->p_db = sm->p_db;
	p_mgr->p_lock = sm->p_lock;

	/* we initialize and restore the db domain of guid to lid map */
	p_mgr->p_g2l = osm_db_domain_init(p_mgr->p_db, "guid2lid");
	if (!p_mgr->p_g2l) {
		OSM_LOG(p_mgr->p_log, OSM_LOG_ERROR, "ERR 0316: "
			"Error initializing Guid-to-Lid persistent database\n");
		status = IB_ERROR;
		goto Exit;
	}

	cl_qlist_init(&p_mgr->free_ranges);

	/* we use the stored guid to lid table if not forced to reassign */
	if (!p_mgr->p_subn->opt.reassign_lids) {
		if (osm_db_restore(p_mgr->p_g2l)) {
#ifndef __WIN__
			/*
			 * When Windows is BSODing, it might corrupt files that
			 * were previously opened for writing, even if the files
			 * are closed, so we might see corrupted guid2lid file.
			 */
			if (p_mgr->p_subn->opt.exit_on_fatal) {
				osm_log_v2(p_mgr->p_log, OSM_LOG_SYS, FILE_ID,
					   "FATAL: Error restoring Guid-to-Lid "
					   "persistent database\n");
				status = IB_ERROR;
				goto Exit;
			} else
#endif
				OSM_LOG(p_mgr->p_log, OSM_LOG_ERROR,
					"ERR 0317: Error restoring Guid-to-Lid "
					"persistent database\n");
		}

		/* we need to make sure we did not get duplicates with
		   current lmc */
		lid_mgr_validate_db(p_mgr);
	}

Exit:
	OSM_LOG_EXIT(p_mgr->p_log);
	return status;
}