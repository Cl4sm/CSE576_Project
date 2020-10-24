osm_switch_t *osm_switch_new(IN osm_node_t * p_node,
			     IN const osm_madw_t * p_madw)
{
	osm_switch_t *p_sw;
	ib_switch_info_t *p_si;
	ib_smp_t *p_smp;
	uint8_t num_ports;
	uint32_t port_num;

	CL_ASSERT(p_madw);
	CL_ASSERT(p_node);

	p_smp = osm_madw_get_smp_ptr(p_madw);
	p_si = ib_smp_get_payload_ptr(p_smp);
	num_ports = osm_node_get_num_physp(p_node);

	CL_ASSERT(p_smp->attr_id == IB_MAD_ATTR_SWITCH_INFO);

	if (!p_si->lin_cap) /* The switch doesn't support LFT */
		return NULL;

	p_sw = malloc(sizeof(*p_sw));
	if (!p_sw)
		return NULL;

	memset(p_sw, 0, sizeof(*p_sw));

	p_sw->p_node = p_node;
	p_sw->switch_info = *p_si;
	p_sw->num_ports = num_ports;
	p_sw->need_update = 2;

	p_sw->p_prof = malloc(sizeof(*p_sw->p_prof) * num_ports);
	if (!p_sw->p_prof)
		goto err;

	memset(p_sw->p_prof, 0, sizeof(*p_sw->p_prof) * num_ports);

	osm_mcast_tbl_init(&p_sw->mcast_tbl, osm_node_get_num_physp(p_node),
			   cl_ntoh16(p_si->mcast_cap));

	for (port_num = 0; port_num < num_ports; port_num++)
		osm_port_prof_construct(&p_sw->p_prof[port_num]);

	return p_sw;

err:
	osm_switch_delete(&p_sw);
	return NULL;
}