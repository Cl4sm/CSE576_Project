void osm_pr_process_half(IN osm_sa_t * sa, IN const ib_sa_mad_t * sa_mad,
				IN const osm_port_t * requester_port,
				IN const osm_alias_guid_t * p_src_alias_guid,
				IN const osm_alias_guid_t * p_dest_alias_guid,
				IN const ib_gid_t * p_sgid,
				IN const ib_gid_t * p_dgid,
				IN cl_qlist_t * p_list)
{
	const cl_qmap_t *p_tbl;
	const osm_alias_guid_t *p_alias_guid;

	OSM_LOG_ENTER(sa->p_log);

	/*
	   Iterate over every port, looking for matches...
	   A path record from a port to itself is legit, so no
	   need to special case that one.
	 */
	p_tbl = &sa->p_subn->alias_port_guid_tbl;

	if (p_src_alias_guid) {
		/*
		   The src port if fixed, so iterate over destination ports.
		 */
		p_alias_guid = (osm_alias_guid_t *) cl_qmap_head(p_tbl);
		while (p_alias_guid != (osm_alias_guid_t *) cl_qmap_end(p_tbl)) {
			pr_rcv_get_port_pair_paths(sa, sa_mad, requester_port,
						   p_src_alias_guid,
						   p_alias_guid,
						   p_sgid, p_dgid, p_list);
			if (sa_mad->method == IB_MAD_METHOD_GET &&
			    cl_qlist_count(p_list) > 0)
				break;
			p_alias_guid = (osm_alias_guid_t *) cl_qmap_next(&p_alias_guid->map_item);
		}
	} else {
		/*
		   The dest port if fixed, so iterate over source ports.
		 */
		p_alias_guid = (osm_alias_guid_t *) cl_qmap_head(p_tbl);
		while (p_alias_guid != (osm_alias_guid_t *) cl_qmap_end(p_tbl)) {
			pr_rcv_get_port_pair_paths(sa, sa_mad, requester_port,
						   p_alias_guid,
						   p_dest_alias_guid, p_sgid,
						   p_dgid, p_list);
			if (sa_mad->method == IB_MAD_METHOD_GET &&
			    cl_qlist_count(p_list) > 0)
				break;
			p_alias_guid = (osm_alias_guid_t *) cl_qmap_next(&p_alias_guid->map_item);
		}
	}

	OSM_LOG_EXIT(sa->p_log);
}