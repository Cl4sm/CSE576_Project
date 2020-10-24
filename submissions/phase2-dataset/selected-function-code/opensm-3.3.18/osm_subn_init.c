ib_api_status_t osm_subn_init(IN osm_subn_t * p_subn, IN osm_opensm_t * p_osm,
			      IN const osm_subn_opt_t * p_opt)
{
	cl_status_t status;

	p_subn->p_osm = p_osm;

	status = cl_ptr_vector_init(&p_subn->port_lid_tbl,
				    OSM_SUBNET_VECTOR_MIN_SIZE,
				    OSM_SUBNET_VECTOR_GROW_SIZE);
	if (status != CL_SUCCESS)
		return status;

	status = cl_ptr_vector_set_capacity(&p_subn->port_lid_tbl,
					    OSM_SUBNET_VECTOR_CAPACITY);
	if (status != CL_SUCCESS)
		return status;

	/*
	   LID zero is not valid.  NULL out this entry for the
	   convenience of other code.
	 */
	cl_ptr_vector_set(&p_subn->port_lid_tbl, 0, NULL);

	p_subn->opt = *p_opt;
	p_subn->max_ucast_lid_ho = IB_LID_UCAST_END_HO;
	p_subn->max_mcast_lid_ho = IB_LID_MCAST_END_HO;
	p_subn->min_ca_mtu = IB_MAX_MTU;
	p_subn->min_ca_rate = IB_MAX_RATE;
	p_subn->min_data_vls = IB_MAX_NUM_VLS - 1;
	p_subn->min_sw_data_vls = IB_MAX_NUM_VLS - 1;
	p_subn->ignore_existing_lfts = TRUE;

	/* we assume master by default - so we only need to set it true if STANDBY */
	p_subn->coming_out_of_standby = FALSE;
	p_subn->sweeping_enabled = TRUE;
	p_subn->last_sm_port_state = 1;

	/* Initialize the guid2mkey database */
	p_subn->p_g2m = osm_db_domain_init(&(p_osm->db), "guid2mkey");
	if (!p_subn->p_g2m) {
		OSM_LOG(&(p_osm->log), OSM_LOG_ERROR, "ERR 7510: "
			"Error initializing Guid-to-MKey persistent database\n");
		return IB_ERROR;
	}

	if (osm_db_restore(p_subn->p_g2m)) {
#ifndef __WIN__
		/*
		 * When Windows is BSODing, it might corrupt files that
		 * were previously opened for writing, even if the files
		 * are closed, so we might see corrupted guid2mkey file.
		 */
		if (p_subn->opt.exit_on_fatal) {
			osm_log(&(p_osm->log), OSM_LOG_SYS,
				"FATAL: Error restoring Guid-to-Mkey "
				"persistent database\n");
			return IB_ERROR;
		} else
#endif
			OSM_LOG(&(p_osm->log), OSM_LOG_ERROR,
				"ERR 7511: Error restoring Guid-to-Mkey "
				"persistent database\n");
	}

	subn_validate_g2m(p_subn);

	/* Initialize the neighbor database */
	p_subn->p_neighbor = osm_db_domain_init(&(p_osm->db), "neighbors");
	if (!p_subn->p_neighbor) {
		OSM_LOG(&(p_osm->log), OSM_LOG_ERROR, "ERR 7520: Error "
			"initializing neighbor link persistent database\n");
		return IB_ERROR;
	}

	if (osm_db_restore(p_subn->p_neighbor)) {
#ifndef __WIN__
		/*
		 * When Windows is BSODing, it might corrupt files that
		 * were previously opened for writing, even if the files
		 * are closed, so we might see corrupted neighbors file.
		 */
		if (p_subn->opt.exit_on_fatal) {
			osm_log(&(p_osm->log), OSM_LOG_SYS,
				"FATAL: Error restoring neighbor link "
				"persistent database\n");
			return IB_ERROR;
		} else
#endif
			OSM_LOG(&(p_osm->log), OSM_LOG_ERROR,
				"ERR 7521: Error restoring neighbor link "
				"persistent database\n");
	}

	subn_validate_neighbor(p_subn);

	return IB_SUCCESS;
}