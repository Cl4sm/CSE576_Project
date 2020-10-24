static ftree_sw_t *sw_create(IN ftree_fabric_t * p_ftree,
			     IN osm_switch_t * p_osm_sw)
{
	ftree_sw_t *p_sw;
	uint8_t ports_num;

	/* make sure that the switch has ports */
	if (p_osm_sw->num_ports == 1)
		return NULL;

	p_sw = (ftree_sw_t *) malloc(sizeof(ftree_sw_t));
	if (p_sw == NULL)
		return NULL;
	memset(p_sw, 0, sizeof(ftree_sw_t));

	p_sw->p_osm_sw = p_osm_sw;
	p_sw->rank = 0xFFFFFFFF;
	tuple_init(p_sw->tuple);

	p_sw->base_lid =
	    cl_ntoh16(osm_node_get_base_lid(p_sw->p_osm_sw->p_node, 0));

	ports_num = osm_node_get_num_physp(p_sw->p_osm_sw->p_node);
	p_sw->down_port_groups =
	    (ftree_port_group_t **) malloc(ports_num *
					   sizeof(ftree_port_group_t *));
	if (p_sw->down_port_groups == NULL)
		goto FREE_P_SW;
	memset(p_sw->down_port_groups, 0, ports_num * sizeof(ftree_port_group_t *));

	p_sw->up_port_groups =
	    (ftree_port_group_t **) malloc(ports_num *
					   sizeof(ftree_port_group_t *));
	if (p_sw->up_port_groups == NULL)
		goto FREE_DOWN;
	memset(p_sw->up_port_groups, 0, ports_num * sizeof(ftree_port_group_t *));

	p_sw->sibling_port_groups =
	    (ftree_port_group_t **) malloc(ports_num *
					   sizeof(ftree_port_group_t *));
	if (p_sw->sibling_port_groups == NULL)
		goto FREE_UP;
	memset(p_sw->sibling_port_groups, 0, ports_num * sizeof(ftree_port_group_t *));

	/* initialize lft buffer */
	memset(p_osm_sw->new_lft, OSM_NO_PATH, p_osm_sw->lft_size);
	p_sw->hops = malloc((p_osm_sw->max_lid_ho + 1) * sizeof(*(p_sw->hops)));
	if (p_sw->hops == NULL)
		goto FREE_SIBLING;

	memset(p_sw->hops, OSM_NO_PATH, p_osm_sw->max_lid_ho + 1);

	return p_sw;

FREE_SIBLING:
	free(p_sw->sibling_port_groups);
FREE_UP:
	free(p_sw->up_port_groups);
FREE_DOWN:
	free(p_sw->down_port_groups);
FREE_P_SW:
	free(p_sw);
	return NULL;
}				/* sw_create() */