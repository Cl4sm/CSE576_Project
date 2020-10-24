static boolean_t
sa_path_rec_apply_tavor_mtu_limit(IN const ib_path_rec_t * p_pr,
				  IN const osm_port_t * p_src_port,
				  IN const osm_port_t * p_dest_port,
				  IN const ib_net64_t comp_mask)
{
	uint8_t required_mtu;

	/* only if at least one of the ports is a Tavor device */
	if (!sa_path_rec_is_tavor_port(p_src_port) &&
	    !sa_path_rec_is_tavor_port(p_dest_port))
		return FALSE;

	/*
	   we can apply the patch if either:
	   1. No MTU required
	   2. Required MTU <
	   3. Required MTU = 1K or 512 or 256
	   4. Required MTU > 256 or 512
	 */
	required_mtu = ib_path_rec_mtu(p_pr);
	if ((comp_mask & IB_PR_COMPMASK_MTUSELEC) &&
	    (comp_mask & IB_PR_COMPMASK_MTU)) {
		switch (ib_path_rec_mtu_sel(p_pr)) {
		case 0:	/* must be greater than */
		case 2:	/* exact match */
			if (IB_MTU_LEN_1024 < required_mtu)
				return FALSE;
			break;

		case 1:	/* must be less than */
			/* can't be disqualified by this one */
			break;

		case 3:	/* largest available */
			/* the ULP intentionally requested */
			/* the largest MTU possible */
			return FALSE;

		default:
			/* if we're here, there's a bug in ib_path_rec_mtu_sel() */
			CL_ASSERT(FALSE);
			break;
		}
	}

	return TRUE;
}