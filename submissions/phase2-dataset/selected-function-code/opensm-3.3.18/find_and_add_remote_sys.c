static struct osm_remote_node *find_and_add_remote_sys(osm_switch_t * sw,
						       uint8_t port,
						       boolean_t dor, struct
						       osm_remote_guids_count
						       *r)
{
	unsigned i;
	osm_physp_t *p = osm_node_get_physp_ptr(sw->p_node, port);
	osm_node_t *node = p->p_remote_physp->p_node;
	uint8_t rem_port = osm_physp_get_port_num(p->p_remote_physp);

	for (i = 0; i < r->count; i++)
		if (r->guids[i].node == node)
			if (!dor || (r->guids[i].port == rem_port))
				return &r->guids[i];

	r->guids[i].node = node;
	r->guids[i].forwarded_to = 0;
	r->guids[i].port = rem_port;
	r->count++;
	return &r->guids[i];
}