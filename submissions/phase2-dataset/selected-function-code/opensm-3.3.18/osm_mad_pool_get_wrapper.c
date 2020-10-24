osm_madw_t *osm_mad_pool_get_wrapper(IN osm_mad_pool_t * p_pool,
				     IN osm_bind_handle_t h_bind,
				     IN uint32_t total_size,
				     IN const ib_mad_t * p_mad,
				     IN const osm_mad_addr_t * p_mad_addr)
{
	osm_madw_t *p_madw;

	CL_ASSERT(h_bind != OSM_BIND_INVALID_HANDLE);
	CL_ASSERT(total_size);
	CL_ASSERT(p_mad);

	/*
	   First, acquire a mad wrapper from the mad wrapper pool.
	 */
	p_madw = malloc(sizeof(*p_madw));
	if (p_madw == NULL)
		goto Exit;

	/*
	   Finally, initialize the wrapper object.
	 */
	cl_atomic_inc(&p_pool->mads_out);
	osm_madw_init(p_madw, h_bind, total_size, p_mad_addr);
	osm_madw_set_mad(p_madw, p_mad);

Exit:
	return p_madw;
}