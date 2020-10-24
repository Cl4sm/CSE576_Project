static void pr_rcv_get_port_pair_paths(IN osm_sa_t * sa,
				       IN const ib_sa_mad_t *sa_mad,
				       IN const osm_port_t * p_req_port,
				       IN const osm_alias_guid_t * p_src_alias_guid,
				       IN const osm_alias_guid_t * p_dest_alias_guid,
				       IN const ib_gid_t * p_sgid,
				       IN const ib_gid_t * p_dgid,
				       IN cl_qlist_t * p_list)
{
	const ib_path_rec_t *p_pr = ib_sa_mad_get_payload_ptr(sa_mad);
	ib_net64_t comp_mask = sa_mad->comp_mask;
	osm_sa_item_t *p_pr_item;
	uint16_t src_lid_min_ho;
	uint16_t src_lid_max_ho;
	uint16_t dest_lid_min_ho;
	uint16_t dest_lid_max_ho;
	uint16_t src_lid_ho;
	uint16_t dest_lid_ho;
	uint32_t path_num;
	uint8_t preference;
	unsigned iterations, src_offset, dest_offset;

	OSM_LOG_ENTER(sa->p_log);

	OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
		"Src port 0x%016" PRIx64 ", Dst port 0x%016" PRIx64 "\n",
		cl_ntoh64(p_src_alias_guid->alias_guid),
		cl_ntoh64(p_dest_alias_guid->alias_guid));

	/* Check that the req_port, src_port and dest_port all share a
	   pkey. The check is done on the default physical port of the ports. */
	if (osm_port_share_pkey(sa->p_log, p_req_port,
				p_src_alias_guid->p_base_port,
				sa->p_subn->opt.allow_both_pkeys) == FALSE
	    || osm_port_share_pkey(sa->p_log, p_req_port,
				   p_dest_alias_guid->p_base_port,
				   sa->p_subn->opt.allow_both_pkeys) == FALSE
	    || osm_port_share_pkey(sa->p_log, p_src_alias_guid->p_base_port,
				   p_dest_alias_guid->p_base_port,
				   sa->p_subn->opt.allow_both_pkeys) == FALSE)
		/* One of the pairs doesn't share a pkey so the path is disqualified. */
		goto Exit;

	/*
	   We shouldn't be here if the paths are disqualified in some way...
	   Thus, we assume every possible connection is valid.

	   We desire to return high-quality paths first.
	   In OpenSM, higher quality means least overlap with other paths.
	   This is acheived in practice by returning paths with
	   different LID value on each end, which means these
	   paths are more redundant that paths with the same LID repeated
	   on one side.  For example, in OpenSM the paths between two
	   endpoints with LMC = 1 might be as follows:

	   Port A, LID 1 <-> Port B, LID 3
	   Port A, LID 1 <-> Port B, LID 4
	   Port A, LID 2 <-> Port B, LID 3
	   Port A, LID 2 <-> Port B, LID 4

	   The OpenSM unicast routing algorithms attempt to disperse each path
	   to as varied a physical path as is reasonable.  1<->3 and 1<->4 have
	   more physical overlap (hence less redundancy) than 1<->3 and 2<->4.

	   OpenSM ranks paths in three preference groups:

	   Preference Value    Description
	   ----------------    -------------------------------------------
	   0             Redundant in both directions with other
	   pref value = 0 paths

	   1             Redundant in one direction with other
	   pref value = 0 and pref value = 1 paths

	   2             Not redundant in either direction with
	   other paths

	   3-FF          Unused

	   SA clients don't need to know these details, only that the lower
	   preference paths are preferred, as stated in the spec.  The paths
	   may not actually be physically redundant depending on the topology
	   of the subnet, but the point of LMC > 0 is to offer redundancy,
	   so it is assumed that the subnet is physically appropriate for the
	   specified LMC value.  A more advanced implementation would inspect for
	   physical redundancy, but I'm not going to bother with that now.
	 */

	/*
	   Refine our search if the client specified end-point LIDs
	 */
	if (comp_mask & IB_PR_COMPMASK_DLID)
		dest_lid_max_ho = dest_lid_min_ho = cl_ntoh16(p_pr->dlid);
	else
		osm_port_get_lid_range_ho(p_dest_alias_guid->p_base_port,
					  &dest_lid_min_ho, &dest_lid_max_ho);

	if (comp_mask & IB_PR_COMPMASK_SLID)
		src_lid_max_ho = src_lid_min_ho = cl_ntoh16(p_pr->slid);
	else
		osm_port_get_lid_range_ho(p_src_alias_guid->p_base_port,
					  &src_lid_min_ho, &src_lid_max_ho);

	if (src_lid_min_ho == 0) {
		OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
			"Obtained source LID of 0. No such LID possible "
			"(%s port %d)\n",
			p_src_alias_guid->p_base_port->p_node->print_desc,
			p_src_alias_guid->p_base_port->p_physp->port_num);
		goto Exit;
	}

	if (dest_lid_min_ho == 0) {
		OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
			"Obtained destination LID of 0. No such LID possible "
			"(%s port %d)\n",
			p_dest_alias_guid->p_base_port->p_node->print_desc,
			p_dest_alias_guid->p_base_port->p_physp->port_num);
		goto Exit;
	}

	OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
		"Src LIDs [%u-%u], Dest LIDs [%u-%u]\n",
		src_lid_min_ho, src_lid_max_ho,
		dest_lid_min_ho, dest_lid_max_ho);

	src_lid_ho = src_lid_min_ho;
	dest_lid_ho = dest_lid_min_ho;

	/*
	   Preferred paths come first in OpenSM
	 */
	preference = 0;
	path_num = 0;

	/* If SubnAdmGet, assume NumbPaths 1 (1.2 erratum) */
	if (sa_mad->method == IB_MAD_METHOD_GET)
		iterations = 1;
	else if (comp_mask & IB_PR_COMPMASK_NUMBPATH)
		iterations = ib_path_rec_num_path(p_pr);
	else
		iterations = (unsigned) (-1);

	while (path_num < iterations) {
		/*
		   These paths are "fully redundant"
		 */

		p_pr_item = pr_rcv_get_lid_pair_path(sa, p_pr, p_src_alias_guid,
						     p_dest_alias_guid,
						     p_sgid, p_dgid,
						     src_lid_ho, dest_lid_ho,
						     comp_mask, preference);

		if (p_pr_item) {
			cl_qlist_insert_tail(p_list, &p_pr_item->list_item);
			++path_num;
		}

		if (++src_lid_ho > src_lid_max_ho)
			break;

		if (++dest_lid_ho > dest_lid_max_ho)
			break;
	}

	/*
	   Check if we've accumulated all the paths that the user cares to see
	 */
	if (path_num == iterations)
		goto Exit;

	/*
	   Don't bother reporting preference 1 paths for now.
	   It's more trouble than it's worth and can only occur
	   if ports have different LMC values, which isn't supported
	   by OpenSM right now anyway.
	 */
	preference = 2;
	src_lid_ho = src_lid_min_ho;
	dest_lid_ho = dest_lid_min_ho;
	src_offset = 0;
	dest_offset = 0;

	/*
	   Iterate over the remaining paths
	 */
	while (path_num < iterations) {
		dest_offset++;
		dest_lid_ho++;

		if (dest_lid_ho > dest_lid_max_ho) {
			src_offset++;
			src_lid_ho++;

			if (src_lid_ho > src_lid_max_ho)
				break;	/* done */

			dest_offset = 0;
			dest_lid_ho = dest_lid_min_ho;
		}

		/*
		   These paths are "fully non-redundant" with paths already
		   identified above and consequently not of much value.

		   Don't return paths we already identified above, as indicated
		   by the offset values being equal.
		 */
		if (src_offset == dest_offset)
			continue;	/* already reported */

		p_pr_item = pr_rcv_get_lid_pair_path(sa, p_pr, p_src_alias_guid,
						     p_dest_alias_guid, p_sgid,
						     p_dgid, src_lid_ho,
						     dest_lid_ho, comp_mask,
						     preference);

		if (p_pr_item) {
			cl_qlist_insert_tail(p_list, &p_pr_item->list_item);
			++path_num;
		}
	}

Exit:
	OSM_LOG_EXIT(sa->p_log);
}