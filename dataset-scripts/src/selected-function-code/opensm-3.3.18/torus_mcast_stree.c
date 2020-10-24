static
ib_api_status_t torus_mcast_stree(void *context, osm_mgrp_box_t *mgb)
{
	struct torus_context *ctx = context;
	struct torus *t = ctx->torus;
	cl_map_item_t *m_item = NULL;
	cl_list_item_t *l_item = NULL;
	osm_port_t *osm_port;
	osm_switch_t *osm_sw;
	struct endpoint *port;
	unsigned port_cnt = 0, max_depth = 0;

	osm_purge_mtree(&ctx->osm->sm, mgb);

	/*
	 * Build a spanning tree for a multicast group by first marking
	 * the torus endpoints that are participating in the group.
	 * Then do a depth-first search of the torus master spanning
	 * tree to build up the spanning tree specific to this group.
	 *
	 * Since the torus master spanning tree is constructed specifically
	 * to guarantee that multicast will not deadlock against unicast
	 * when they share VLs, we can be sure that any multicast group
	 * spanning tree constructed this way has the same property.
	 */
	while ((osm_port = next_mgrp_box_port(mgb, &l_item, &m_item))) {
		port = osm_port->priv;
		if (!(port && port->osm_port == osm_port)) {
			port = osm_port_relink_endpoint(osm_port);
			if (!port) {
				guid_t id;
				id = osm_node_get_node_guid(osm_port->p_node);
				OSM_LOG(&ctx->osm->log, OSM_LOG_ERROR,
					"ERR 4E41: osm_port (GUID 0x%04"PRIx64") "
					"not in torus fabric description\n",
					cl_ntoh64(id));
				continue;
			}
		}
		/*
		 * If this is a CA port, mark the switch port at the
		 * other end of this port's link.
		 *
		 * By definition, a CA port is connected to end[1] of a link,
		 * and the switch port is end[0].  See build_ca_link() and
		 * link_srcsink().
		 */
		if (port->link)
			port = &port->link->end[0];
		port->tmp = osm_port;
	}
	/*
	 * It might be we got called in a window between a switch getting
	 * removed from the fabric, and torus-2QoS getting to rebuild its
	 * fabric representation.  If that were to happen, our
	 * master_stree_root->osm_switch pointer might be stale.  Look up
	 * the osm_switch by GUID to be sure it's not.
	 *
	 * Also, call into mcast_stree_branch with depth = -1, because
	 * depth at root switch needs to be 0.
	 */
	osm_sw = (osm_switch_t *)cl_qmap_get(&ctx->osm->subn.sw_guid_tbl,
					     t->master_stree_root->n_id);
	if (!(osm_sw && t->master_stree_root->osm_switch == osm_sw)) {
		OSM_LOG(&ctx->osm->log, OSM_LOG_ERROR,
			"ERR 4E42: stale pointer to osm_sw (GUID 0x%04"PRIx64")\n",
			cl_ntoh64(t->master_stree_root->n_id));
		return IB_ERROR;
	}
	mgb->root = mcast_stree_branch(t->master_stree_root, osm_sw,
				       mgb, -1, &port_cnt, &max_depth);

	OSM_LOG(&ctx->osm->log, OSM_LOG_VERBOSE,
		"Configured MLID 0x%X for %u ports, max tree depth = %u\n",
		mgb->mlid, port_cnt, max_depth);

	return IB_SUCCESS;
}