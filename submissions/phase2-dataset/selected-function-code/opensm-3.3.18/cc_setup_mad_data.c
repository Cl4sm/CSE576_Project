static void cc_setup_mad_data(osm_sm_t * p_sm)
{
	osm_congestion_control_t *p_cc = &p_sm->p_subn->p_osm->cc;
	osm_subn_opt_t *p_opt = &p_sm->p_subn->opt;
	uint16_t ccti_limit;
	int i;

	/* Switch Congestion Setting */
	p_cc->sw_cong_setting.control_map = p_opt->cc_sw_cong_setting_control_map;

	memcpy(p_cc->sw_cong_setting.victim_mask,
	       p_opt->cc_sw_cong_setting_victim_mask,
	       IB_CC_PORT_MASK_DATA_SIZE);

	memcpy(p_cc->sw_cong_setting.credit_mask,
	       p_opt->cc_sw_cong_setting_credit_mask,
	       IB_CC_PORT_MASK_DATA_SIZE);

	/* threshold is 4 bits, takes up upper nibble of byte */
	p_cc->sw_cong_setting.threshold_resv = (p_opt->cc_sw_cong_setting_threshold << 4);

	p_cc->sw_cong_setting.packet_size = p_opt->cc_sw_cong_setting_packet_size;

	/* cs threshold is 4 bits, takes up upper nibble of short */
	p_cc->sw_cong_setting.cs_threshold_resv =
		cl_hton16(p_opt->cc_sw_cong_setting_credit_starvation_threshold << 12);

	p_cc->sw_cong_setting.cs_return_delay =
		cl_hton16(p_opt->cc_sw_cong_setting_credit_starvation_return_delay.shift << 14
			  | p_opt->cc_sw_cong_setting_credit_starvation_return_delay.multiplier);

	p_cc->sw_cong_setting.marking_rate = p_opt->cc_sw_cong_setting_marking_rate;

	/* CA Congestion Setting */
	p_cc->ca_cong_setting.port_control = p_opt->cc_ca_cong_setting_port_control;
	p_cc->ca_cong_setting.control_map = p_opt->cc_ca_cong_setting_control_map;

	for (i = 0; i < IB_CA_CONG_ENTRY_DATA_SIZE; i++) {
		ib_ca_cong_entry_t *p_entry;

		p_entry = &p_cc->ca_cong_setting.entry_list[i];

		p_entry->ccti_timer = p_opt->cc_ca_cong_entries[i].ccti_timer;
		p_entry->ccti_increase = p_opt->cc_ca_cong_entries[i].ccti_increase;
		p_entry->trigger_threshold = p_opt->cc_ca_cong_entries[i].trigger_threshold;
		p_entry->ccti_min = p_opt->cc_ca_cong_entries[i].ccti_min;
		p_entry->resv0 = 0;
		p_entry->resv1 = 0;
	}

	/* Congestion Control Table */

	/* if no entries, we will always send atleast 1 mad to set ccti_limit = 0 */
	if (!p_opt->cc_cct.entries_len)
		p_cc->cc_tbl_mads = 1;
	else {
		p_cc->cc_tbl_mads = p_opt->cc_cct.entries_len - 1;
		p_cc->cc_tbl_mads /= IB_CC_TBL_ENTRY_LIST_MAX;
		p_cc->cc_tbl_mads += 1;
	}

	CL_ASSERT(p_cc->cc_tbl_mads <= OSM_CCT_ENTRY_MAD_BLOCKS);

	if (!p_opt->cc_cct.entries_len)
		ccti_limit = 0;
	else
		ccti_limit = p_opt->cc_cct.entries_len - 1;

	for (i = 0; i < p_cc->cc_tbl_mads; i++) {
		int j;

		p_cc->cc_tbl[i].ccti_limit = cl_hton16(ccti_limit);
		p_cc->cc_tbl[i].resv = 0;

		memset(p_cc->cc_tbl[i].entry_list,
		       '\0',
		       sizeof(p_cc->cc_tbl[i].entry_list));

		if (!ccti_limit)
			break;

		for (j = 0; j < IB_CC_TBL_ENTRY_LIST_MAX; j++) {
			int k;

			k = (i * IB_CC_TBL_ENTRY_LIST_MAX) + j;
			p_cc->cc_tbl[i].entry_list[j].shift_multiplier =
				cl_hton16(p_opt->cc_cct.entries[k].shift << 14
					  | p_opt->cc_cct.entries[k].multiplier);
		}
	}
}