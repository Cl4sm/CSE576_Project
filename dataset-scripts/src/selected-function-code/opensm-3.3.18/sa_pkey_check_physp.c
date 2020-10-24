static void sa_pkey_check_physp(IN osm_sa_t * sa, IN osm_physp_t * p_physp,
				osm_pkey_search_ctxt_t * p_ctxt)
{
	ib_net64_t comp_mask = p_ctxt->comp_mask;
	uint16_t block, num_blocks;

	OSM_LOG_ENTER(sa->p_log);

	/* we got here with the phys port - all is left is to get the right block */
	if (comp_mask & IB_PKEY_COMPMASK_BLOCK) {
		sa_pkey_create(sa, p_physp, p_ctxt, p_ctxt->block_num);
	} else {
		num_blocks =
		    osm_pkey_tbl_get_num_blocks(osm_physp_get_pkey_tbl
						(p_physp));
		for (block = 0; block < num_blocks; block++)
			sa_pkey_create(sa, p_physp, p_ctxt, block);
	}

	OSM_LOG_EXIT(sa->p_log);
}