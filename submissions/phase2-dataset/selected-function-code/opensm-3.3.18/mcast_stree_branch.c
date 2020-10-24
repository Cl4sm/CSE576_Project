static
osm_mtree_node_t *mcast_stree_branch(struct t_switch *sw, osm_switch_t *osm_sw,
				     osm_mgrp_box_t *mgb, unsigned depth,
				     unsigned *port_cnt, unsigned *max_depth)
{
	osm_mtree_node_t *mtn = NULL;
	osm_mcast_tbl_t *mcast_tbl, *ds_mcast_tbl;
	osm_node_t *ds_node;
	struct t_switch *ds_sw;
	struct port_grp *ptgrp;
	struct link *link;
	struct endpoint *port;
	unsigned g, p;
	unsigned mcast_fwd_ports = 0, mcast_end_ports = 0;

	depth++;

	if (osm_sw->priv != sw) {
		OSM_LOG(&sw->torus->osm->log, OSM_LOG_ERROR,
			"ERR 4E3E: osm_sw (GUID 0x%04"PRIx64") "
			"not in torus fabric description\n",
			cl_ntoh64(osm_node_get_node_guid(osm_sw->p_node)));
		goto out;
	}
	if (!osm_switch_supports_mcast(osm_sw)) {
		OSM_LOG(&sw->torus->osm->log, OSM_LOG_ERROR,
			"ERR 4E3F: osm_sw (GUID 0x%04"PRIx64") "
			"does not support multicast\n",
			cl_ntoh64(osm_node_get_node_guid(osm_sw->p_node)));
		goto out;
	}
	mtn = osm_mtree_node_new(osm_sw);
	if (!mtn) {
		OSM_LOG(&sw->torus->osm->log, OSM_LOG_ERROR,
			"ERR 4E46: Insufficient memory to build multicast tree\n");
		goto out;
	}
	mcast_tbl = osm_switch_get_mcast_tbl_ptr(osm_sw);
	/*
	 * Recurse to downstream switches, i.e. those closer to master
	 * spanning tree branch tips.
	 *
	 * Note that if there are multiple ports in this port group, i.e.,
	 * multiple parallel links, we can pick any one of them to use for
	 * any individual MLID without causing loops.  Pick one based on MLID
	 * for now, until someone turns up evidence we need to be smarter.
	 *
	 * Also, it might be we got called in a window between a switch getting
	 * removed from the fabric, and torus-2QoS getting to rebuild its
	 * fabric representation.  If that were to happen, our next hop
	 * osm_switch pointer might be stale.  Look it up via opensm's fabric
	 * description to be sure it's not.
	 */
	for (g = 0; g < 2 * TORUS_MAX_DIM; g++) {
		ptgrp = &sw->ptgrp[g];
		if (!ptgrp->to_stree_tip)
			continue;

		p = mgb->mlid % ptgrp->port_cnt;/* port # in port group */
		p = ptgrp->port[p]->port;	/* now port # in switch */

		ds_node = osm_node_get_remote_node(osm_sw->p_node, p, NULL);
		ds_sw = ptgrp->to_stree_tip->sw;

		if (!(ds_node && ds_node->sw &&
		      ds_sw->osm_switch == ds_node->sw)) {
			OSM_LOG(&sw->torus->osm->log, OSM_LOG_ERROR,
				"ERR 4E40: stale pointer to osm_sw "
				"(GUID 0x%04"PRIx64")\n", cl_ntoh64(ds_sw->n_id));
			continue;
		}
		mtn->child_array[p] =
			mcast_stree_branch(ds_sw, ds_node->sw, mgb,
					   depth, port_cnt, max_depth);
		if (!mtn->child_array[p])
			continue;

		osm_mcast_tbl_set(mcast_tbl, mgb->mlid, p);
		mcast_fwd_ports++;
		/*
		 * Since we forward traffic for this multicast group on this
		 * port, cause the switch on the other end of the link
		 * to forward traffic back to us.  Do it now since have at
		 * hand the link used; otherwise it'll be hard to figure out
		 * later, and if we get it wrong we get a MC routing loop.
		 */
		link = sw->port[p]->link;
		ds_mcast_tbl = osm_switch_get_mcast_tbl_ptr(ds_node->sw);

		if (&link->end[0] == sw->port[p])
			osm_mcast_tbl_set(ds_mcast_tbl, mgb->mlid,
					  link->end[1].port);
		else
			osm_mcast_tbl_set(ds_mcast_tbl, mgb->mlid,
					  link->end[0].port);
	}
	/*
	 * Add any host ports marked as in mcast group into spanning tree.
	 */
	ptgrp = &sw->ptgrp[2 * TORUS_MAX_DIM];
	for (p = 0; p < ptgrp->port_cnt; p++) {
		port = ptgrp->port[p];
		if (port->tmp) {
			port->tmp = NULL;
			mtn->child_array[port->port] = OSM_MTREE_LEAF;
			osm_mcast_tbl_set(mcast_tbl, mgb->mlid, port->port);
			mcast_end_ports++;
		}
	}
	if (!(mcast_end_ports || mcast_fwd_ports)) {
		osm_mtree_destroy(mtn);
		mtn = NULL;
	} else if (depth > *max_depth)
		*max_depth = depth;

	*port_cnt += mcast_end_ports;
out:
	return mtn;
}