static void
pkey_mgr_process_partition_table(osm_log_t * p_log, osm_sm_t * sm,
				 const osm_prtn_t * p_prtn,
				 const boolean_t full)
{
	const cl_map_t *p_tbl =
	    full ? &p_prtn->full_guid_tbl : &p_prtn->part_guid_tbl;
	cl_map_iterator_t i, i_next;
	ib_net16_t pkey = p_prtn->pkey;
	osm_physp_t *p_physp;

	if (full)
		pkey |= cl_hton16(0x8000);

	i_next = cl_map_head(p_tbl);
	while (i_next != cl_map_end(p_tbl)) {
		i = i_next;
		i_next = cl_map_next(i);
		p_physp = cl_map_obj(i);
		if (p_physp)
			pkey_mgr_process_physical_port(p_log, sm, pkey,
						       p_physp);
	}
}