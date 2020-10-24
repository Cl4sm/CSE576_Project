static ib_api_status_t vlarb_update(osm_sm_t * sm, osm_physp_t * p,
				    uint8_t port_num, unsigned force_update,
				    const struct qos_config *qcfg,
				    cl_qlist_t *mad_list)
{
	ib_api_status_t status = IB_SUCCESS;
	ib_port_info_t *p_pi = &p->port_info;
	unsigned len;

	if (p_pi->vl_arb_low_cap > 0) {
		len = p_pi->vl_arb_low_cap < IB_NUM_VL_ARB_ELEMENTS_IN_BLOCK ?
		    p_pi->vl_arb_low_cap : IB_NUM_VL_ARB_ELEMENTS_IN_BLOCK;
		if ((status = vlarb_update_table_block(sm, p, port_num,
						       force_update,
						       &qcfg->vlarb_low[0],
						       len, 0,
						       mad_list)) != IB_SUCCESS)
			return status;
	}
	if (p_pi->vl_arb_low_cap > IB_NUM_VL_ARB_ELEMENTS_IN_BLOCK) {
		len = p_pi->vl_arb_low_cap % IB_NUM_VL_ARB_ELEMENTS_IN_BLOCK;
		if ((status = vlarb_update_table_block(sm, p, port_num,
						       force_update,
						       &qcfg->vlarb_low[1],
						       len, 1,
						       mad_list)) != IB_SUCCESS)
			return status;
	}
	if (p_pi->vl_arb_high_cap > 0) {
		len = p_pi->vl_arb_high_cap < IB_NUM_VL_ARB_ELEMENTS_IN_BLOCK ?
		    p_pi->vl_arb_high_cap : IB_NUM_VL_ARB_ELEMENTS_IN_BLOCK;
		if ((status = vlarb_update_table_block(sm, p, port_num,
						       force_update,
						       &qcfg->vlarb_high[0],
						       len, 2,
						       mad_list)) != IB_SUCCESS)
			return status;
	}
	if (p_pi->vl_arb_high_cap > IB_NUM_VL_ARB_ELEMENTS_IN_BLOCK) {
		len = p_pi->vl_arb_high_cap % IB_NUM_VL_ARB_ELEMENTS_IN_BLOCK;
		if ((status = vlarb_update_table_block(sm, p, port_num,
						       force_update,
						       &qcfg->vlarb_high[1],
						       len, 3,
						       mad_list)) != IB_SUCCESS)
			return status;
	}

	return status;
}