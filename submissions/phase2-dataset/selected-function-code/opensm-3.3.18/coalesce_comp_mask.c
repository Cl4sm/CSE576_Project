static uint8_t coalesce_comp_mask(IN osm_madw_t *p_madw)
{
	uint8_t comp_mask = 0;
	ib_sa_mad_t *p_sa_mad;

	p_sa_mad = osm_madw_get_sa_mad_ptr(p_madw);
	if (p_sa_mad->comp_mask & IB_GIR_COMPMASK_GID0)
		comp_mask |= 1<<0;
	if (p_sa_mad->comp_mask & IB_GIR_COMPMASK_GID1)
		comp_mask |= 1<<1;
	if (p_sa_mad->comp_mask & IB_GIR_COMPMASK_GID2)
		comp_mask |= 1<<2;
	if (p_sa_mad->comp_mask & IB_GIR_COMPMASK_GID3)
		comp_mask |= 1<<3;
	if (p_sa_mad->comp_mask & IB_GIR_COMPMASK_GID4)
		comp_mask |= 1<<4;
	if (p_sa_mad->comp_mask & IB_GIR_COMPMASK_GID5)
		comp_mask |= 1<<5;
	if (p_sa_mad->comp_mask & IB_GIR_COMPMASK_GID6)
		comp_mask |= 1<<6;
	if (p_sa_mad->comp_mask & IB_GIR_COMPMASK_GID7)
		comp_mask |= 1<<7;
	return comp_mask;
}