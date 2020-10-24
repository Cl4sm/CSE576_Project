osm_madw_t *osm_mad_pool_get(IN osm_mad_pool_t * p_pool,
			     IN osm_bind_handle_t h_bind,
			     IN uint32_t total_size,
			     IN const osm_mad_addr_t * p_mad_addr)
{
	osm_madw_t *p_madw;
	ib_mad_t *p_mad;

	CL_ASSERT(h_bind != OSM_BIND_INVALID_HANDLE);
	CL_ASSERT(total_size);

	/*
	   First, acquire a mad wrapper from the mad wrapper pool.
	 */
	p_madw = malloc(sizeof(*p_madw));
	if (p_madw == NULL)
		goto Exit;

	osm_madw_init(p_madw, h_bind, total_size, p_mad_addr);

	/*
	   Next, acquire a wire mad of the specified size.
	 */
	p_mad = osm_vendor_get(h_bind, total_size, &p_madw->vend_wrap);
	if (p_mad == NULL) {
		/* Don't leak wrappers! */
		free(p_madw);
		p_madw = NULL;
		goto Exit;
	}

	cl_atomic_inc(&p_pool->mads_out);
	/*
	   Finally, attach the wire MAD to this wrapper.
	 */
	osm_madw_set_mad(p_madw, p_mad);

Exit:
	return p_madw;
}