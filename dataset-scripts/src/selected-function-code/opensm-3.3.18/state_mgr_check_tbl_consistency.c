static void state_mgr_check_tbl_consistency(IN osm_sm_t * sm)
{
	cl_qmap_t *p_port_guid_tbl;
	osm_port_t *p_port;
	osm_port_t *p_next_port;
	cl_ptr_vector_t *p_port_lid_tbl;
	size_t max_lid, ref_size, curr_size, lid;
	osm_port_t *p_port_ref, *p_port_stored;
	cl_ptr_vector_t ref_port_lid_tbl;
	uint16_t min_lid_ho;
	uint16_t max_lid_ho;
	uint16_t lid_ho;

	OSM_LOG_ENTER(sm->p_log);

	if (sm->lid_mgr.dirty == FALSE)
		goto Exit;

	sm->lid_mgr.dirty = FALSE;

	cl_ptr_vector_construct(&ref_port_lid_tbl);
	cl_ptr_vector_init(&ref_port_lid_tbl,
			   cl_ptr_vector_get_size(&sm->p_subn->port_lid_tbl),
			   OSM_SUBNET_VECTOR_GROW_SIZE);

	p_port_guid_tbl = &sm->p_subn->port_guid_tbl;

	/* Let's go over all the ports according to port_guid_tbl,
	 * and add the port to a reference port_lid_tbl. */
	p_next_port = (osm_port_t *) cl_qmap_head(p_port_guid_tbl);
	while (p_next_port != (osm_port_t *) cl_qmap_end(p_port_guid_tbl)) {
		p_port = p_next_port;
		p_next_port =
		    (osm_port_t *) cl_qmap_next(&p_next_port->map_item);

		osm_port_get_lid_range_ho(p_port, &min_lid_ho, &max_lid_ho);
		for (lid_ho = min_lid_ho; lid_ho <= max_lid_ho; lid_ho++)
			cl_ptr_vector_set(&ref_port_lid_tbl, lid_ho, p_port);
	}

	p_port_lid_tbl = &sm->p_subn->port_lid_tbl;

	ref_size = cl_ptr_vector_get_size(&ref_port_lid_tbl);
	curr_size = cl_ptr_vector_get_size(p_port_lid_tbl);
	/* They should be the same, but compare it anyway */
	max_lid = (ref_size > curr_size) ? ref_size : curr_size;

	for (lid = 1; lid < max_lid; lid++) {
		p_port_ref = NULL;
		p_port_stored = NULL;
		cl_ptr_vector_at(p_port_lid_tbl, lid, (void *)&p_port_stored);
		cl_ptr_vector_at(&ref_port_lid_tbl, lid, (void *)&p_port_ref);

		if (p_port_stored == p_port_ref)
			/* This is the "good" case - both entries are the
			 * same for this lid. Nothing to do. */
			continue;

		if (p_port_ref == NULL) {
			/* There is an object in the subnet database for this
			 * lid, but no such object exists in the reference
			 * port_list_tbl. This can occur if we wanted to assign
			 * a certain port with some lid (different than the one
			 * pre-assigned to it), and the port didn't get the
			 * PortInfo Set request. Due to this, the port is
			 * updated with its original lid in our database, but
			 * with the new lid we wanted to give it in our
			 * port_lid_tbl. */
			OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 3322: "
				"lid %zu is wrongly assigned to port 0x%016"
				PRIx64 " (\'%s\' port %u) in port_lid_tbl\n",
				lid,
				cl_ntoh64(osm_port_get_guid(p_port_stored)),
				p_port_stored->p_node->print_desc,
				p_port_stored->p_physp->port_num);
			cl_ptr_vector_set(p_port_lid_tbl, lid, NULL);
		} else if (p_port_stored == NULL)
			/* There is an object in the new database, but no
			 * object in our subnet database. This is the matching
			 * case of the prior check - the port still has its
			 * original lid. */
			OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 3323: "
				"port 0x%016" PRIx64 " (\'%s\' port %u)"
				" exists in new port_lid_tbl under lid %zu,"
				" but missing in subnet port_lid_tbl db\n",
				cl_ntoh64(osm_port_get_guid(p_port_ref)),
				p_port_ref->p_node->print_desc,
				p_port_ref->p_physp->port_num, lid);
		else {
			/* if we reached here then p_port_stored != p_port_ref.
			 * We were trying to set a lid to p_port_stored, but
			 * it didn't reach it, and p_port_ref also didn't get
			 * the lid update. */
			OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 3324: "
				"lid %zu has port 0x%016" PRIx64
				" (\'%s\' port %u) in new port_lid_tbl db, "
				"and port 0x%016" PRIx64 " (\'%s\' port %u)"
				" in subnet port_lid_tbl db\n", lid,
				cl_ntoh64(osm_port_get_guid(p_port_ref)),
				p_port_ref->p_node->print_desc,
				p_port_ref->p_physp->port_num,
				cl_ntoh64(osm_port_get_guid(p_port_stored)),
				p_port_stored->p_node->print_desc,
				p_port_stored->p_physp->port_num);
			cl_ptr_vector_set(p_port_lid_tbl, lid, NULL);
		}
		/* Make sure we'll do another heavy sweep. */
		sm->p_subn->subnet_initialization_error = TRUE;
	}

	cl_ptr_vector_destroy(&ref_port_lid_tbl);

Exit:
	OSM_LOG_EXIT(sm->p_log);
}