void osm_physp_destroy(IN osm_physp_t * p_physp)
{
	size_t num_slvl, i;

	/* the physp might be uninitialized */
	if (p_physp->port_guid) {
		if (p_physp->p_guids)
			free(p_physp->p_guids);

		/* free the SL2VL Tables */
		num_slvl = cl_ptr_vector_get_size(&p_physp->slvl_by_port);
		for (i = 0; i < num_slvl; i++)
			free(cl_ptr_vector_get(&p_physp->slvl_by_port, i));
		cl_ptr_vector_destroy(&p_physp->slvl_by_port);

		/* free the P_Key Tables */
		osm_pkey_tbl_destroy(&p_physp->pkeys);

		memset(p_physp, 0, sizeof(*p_physp));
		osm_dr_path_construct(&p_physp->dr_path);	/* clear dr_path */
	}
}