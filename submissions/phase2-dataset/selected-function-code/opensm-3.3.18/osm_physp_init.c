void osm_physp_init(IN osm_physp_t * p_physp, IN ib_net64_t port_guid,
		    IN uint8_t port_num, IN const struct osm_node *p_node,
		    IN osm_bind_handle_t h_bind, IN uint8_t hop_count,
		    IN const uint8_t * p_initial_path)
{
	uint16_t num_slvl, i;
	ib_slvl_table_t *p_slvl;

	CL_ASSERT(p_node);

	osm_physp_construct(p_physp);
	p_physp->port_guid = port_guid;
	p_physp->port_num = port_num;
	p_physp->healthy = TRUE;
	p_physp->need_update = 2;
	p_physp->p_node = (struct osm_node *)p_node;

	osm_dr_path_init(&p_physp->dr_path, hop_count, p_initial_path);

	/* allocate enough SL2VL tables */
	if (osm_node_get_type(p_node) == IB_NODE_TYPE_SWITCH)
		/* we need node num ports + 1 SL2VL tables */
		num_slvl = osm_node_get_num_physp(p_node) + 1;
	else
		/* An end node - we need only one SL2VL */
		num_slvl = 1;

	cl_ptr_vector_init(&p_physp->slvl_by_port, num_slvl, 1);
	for (i = 0; i < num_slvl; i++) {
		p_slvl = (ib_slvl_table_t *) malloc(sizeof(ib_slvl_table_t));
		if (!p_slvl)
			break;
		memset(p_slvl, 0, sizeof(ib_slvl_table_t));
		cl_ptr_vector_set(&p_physp->slvl_by_port, i, p_slvl);
	}

	/* initialize the pkey table */
	osm_pkey_tbl_init(&p_physp->pkeys);
}