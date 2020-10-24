static unsigned build_new_mgid(osm_sa_t * sa, ib_net64_t comp_mask,
			       ib_member_rec_t * mcmr)
{
	static uint32_t uniq_count;
	ib_gid_t *mgid = &mcmr->mgid;
	uint8_t scope;
	unsigned i;

	/* use the given scope state only if requested! */
	if (comp_mask & IB_MCR_COMPMASK_SCOPE)
		ib_member_get_scope_state(mcmr->scope_state, &scope, NULL);
	else
	/* to guarantee no collision with other subnets use local scope! */
		scope = IB_MC_SCOPE_LINK_LOCAL;

	mgid->raw[0] = 0xff;
	mgid->raw[1] = 0x10 | scope;
	mgid->raw[2] = 0xa0;
	mgid->raw[3] = 0x1b;

	memcpy(&mgid->raw[4], &sa->p_subn->opt.subnet_prefix, sizeof(uint64_t));

	for (i = 0; i < 1000; i++) {
		memcpy(&mgid->raw[10], &uniq_count, 4);
		uniq_count++;
		if (!osm_get_mgrp_by_mgid(sa->p_subn, mgid))
			return 1;
	}

	return 0;
}