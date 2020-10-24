static int alloc_mfts(osm_sm_t * sm)
{
	int i;
	cl_map_item_t *item;
	osm_switch_t *p_sw;

	for (i = sm->p_subn->max_mcast_lid_ho - IB_LID_MCAST_START_HO; i >= 0;
	     i--)
		if (sm->p_subn->mboxes[i])
			break;
	if (i < 0)
		return 0;

	/* Now, walk switches and (re)allocate multicast tables */
	for (item = cl_qmap_head(&sm->p_subn->sw_guid_tbl);
	     item != cl_qmap_end(&sm->p_subn->sw_guid_tbl);
	     item = cl_qmap_next(item)) {
		p_sw = (osm_switch_t *) item;
		if (osm_mcast_tbl_realloc(&p_sw->mcast_tbl, i))
			return -1;
	}
	return 0;
}