static ib_api_status_t vlarb_update_table_block(osm_sm_t * sm,
						osm_physp_t * p,
						uint8_t port_num,
						unsigned force_update,
						const ib_vl_arb_table_t *
						table_block,
						unsigned block_length,
						unsigned block_num,
						cl_qlist_t *mad_list)
{
	struct osm_routing_engine *re = sm->p_subn->p_osm->routing_engine_used;
	ib_vl_arb_table_t block;
	uint32_t attr_mod;
	unsigned vl_mask, i;
	qos_mad_item_t *p_mad;
	vl_mask = (1 << (ib_port_info_get_op_vls(&p->port_info) - 1)) - 1;

	memset(&block, 0, sizeof(block));
	memcpy(&block, table_block, block_length * sizeof(block.vl_entry[0]));

	if (re && re->update_vlarb)
		re->update_vlarb(re->context, p, port_num, &block,
				 block_length, block_num);

	for (i = 0; i < block_length; i++)
		block.vl_entry[i].vl &= vl_mask;

	if (!force_update &&
	    !memcmp(&p->vl_arb[block_num], &block,
		    block_length * sizeof(block.vl_entry[0])))
		return IB_SUCCESS;

	attr_mod = ((block_num + 1) << 16) | port_num;

	p_mad = osm_qos_mad_create(sm,p,sizeof(block),(uint8_t *) & block,
				   IB_MAD_ATTR_VL_ARBITRATION, attr_mod);

	if (!p_mad)
		return IB_INSUFFICIENT_MEMORY;

	/*
	 * Zero the stored VL Arbitration block, so in case the MAD will
	 * end up with error, we will resend it in the next sweep.
	 */
	memset(&p->vl_arb[block_num], 0,
	       block_length * sizeof(block.vl_entry[0]));

	cl_qlist_insert_tail(mad_list, &p_mad->list_item);

	return IB_SUCCESS;
}