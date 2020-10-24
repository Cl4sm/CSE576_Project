static int mcast_mgr_set_mftables(osm_sm_t * sm)
{
	cl_qmap_t *p_sw_tbl = &sm->p_subn->sw_guid_tbl;
	osm_switch_t *p_sw;
	osm_mcast_tbl_t *p_tbl;
	int block_notdone, ret = 0;
	int16_t block_num, max_block = -1;

	p_sw = (osm_switch_t *) cl_qmap_head(p_sw_tbl);
	while (p_sw != (osm_switch_t *) cl_qmap_end(p_sw_tbl)) {
		p_sw->mft_block_num = 0;
		p_sw->mft_position = 0;
		p_tbl = osm_switch_get_mcast_tbl_ptr(p_sw);
		if (osm_mcast_tbl_get_max_block_in_use(p_tbl) > max_block)
			max_block = osm_mcast_tbl_get_max_block_in_use(p_tbl);
		mcast_mgr_set_mfttop(sm, p_sw);
		p_sw = (osm_switch_t *) cl_qmap_next(&p_sw->map_item);
	}

	/* Stripe the MFT blocks across the switches */
	for (block_num = 0; block_num <= max_block; block_num++) {
		block_notdone = 1;
		while (block_notdone) {
			block_notdone = 0;
			p_sw = (osm_switch_t *) cl_qmap_head(p_sw_tbl);
			while (p_sw != (osm_switch_t *) cl_qmap_end(p_sw_tbl)) {
				if (p_sw->mft_block_num == block_num) {
					block_notdone = 1;
					if (mcast_mgr_set_mft_block(sm, p_sw,
								    p_sw->mft_block_num,
								    p_sw->mft_position))
						ret = -1;
					p_tbl = osm_switch_get_mcast_tbl_ptr(p_sw);
					if (++p_sw->mft_position > p_tbl->max_position) {
						p_sw->mft_position = 0;
						p_sw->mft_block_num++;
					}
				}
				p_sw = (osm_switch_t *) cl_qmap_next(&p_sw->map_item);
			}
		}
	}

	return ret;
}