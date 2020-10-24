static void connect_switches(lash_t * p_lash, int sw1, int sw2, int phy_port_1)
{
	osm_log_t *p_log = &p_lash->p_osm->log;
	unsigned num = p_lash->switches[sw1]->node->num_links;
	switch_t *s1 = p_lash->switches[sw1];
	mesh_node_t *node = s1->node;
	switch_t *s2;
	link_t *l;
	unsigned int i;

	/*
	 * if doing mesh analysis:
	 *  - do not consider connections to self
	 *  - collapse multiple connections between
	 *    pair of switches to a single locical link
	 */
	if (p_lash->p_osm->subn.opt.do_mesh_analysis) {
		if (sw1 == sw2)
			return;

		/* see if we are already linked to sw2 */
		for (i = 0; i < num; i++) {
			l = node->links[i];

			if (node->links[i]->switch_id == sw2) {
				l->ports[l->num_ports++] = phy_port_1;
				return;
			}
		}
	}

	l = node->links[num];
	l->switch_id = sw2;
	l->link_id = -1;
	l->ports[l->num_ports++] = phy_port_1;

	s2 = p_lash->switches[sw2];
	for (i = 0; i < s2->node->num_links; i++) {
		if (s2->node->links[i]->switch_id == sw1) {
			s2->node->links[i]->link_id = num;
			l->link_id = i;
			break;
		}
	}

	node->num_links++;

	OSM_LOG(p_log, OSM_LOG_VERBOSE,
		"LASH connect: %d, %d, %d\n", sw1, sw2, phy_port_1);
}