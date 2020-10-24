static void fabric_route_to_cns(IN ftree_fabric_t * p_ftree)
{
	ftree_sw_t *p_sw;
	ftree_hca_t *p_hca;
	ftree_port_group_t *p_leaf_port_group;
	ftree_port_group_t *p_hca_port_group;
	ftree_port_t *p_port;
	unsigned int i, j;
	uint16_t hca_lid;
	unsigned routed_targets_on_leaf;

	OSM_LOG_ENTER(&p_ftree->p_osm->log);

	/* for each leaf switch (in indexing order) */
	for (i = 0; i < p_ftree->leaf_switches_num; i++) {
		p_sw = p_ftree->leaf_switches[i];
		routed_targets_on_leaf = 0;

		/* for each HCA connected to this switch */
		for (j = 0; j < p_sw->down_port_groups_num; j++) {
			p_leaf_port_group = p_sw->down_port_groups[j];

			/* work with this port group only if the remote node is CA */
			if (p_leaf_port_group->remote_node_type !=
			    IB_NODE_TYPE_CA)
				continue;

			p_hca = p_leaf_port_group->remote_hca_or_sw.p_hca;

			/* work with this port group only if remote HCA has CNs */
			if (!p_hca->cn_num)
				continue;

			p_hca_port_group =
			    hca_get_port_group_by_remote_lid(p_hca,
							     p_leaf_port_group->
							     base_lid);
			CL_ASSERT(p_hca_port_group);

			/* work with this port group only if remote port is CN */
			if (!p_hca_port_group->is_cn)
				continue;

			/* obtain the LID of HCA port */
			hca_lid = p_leaf_port_group->remote_base_lid;

			/* set local LFT(LID) to the port that is connected to HCA */
			cl_ptr_vector_at(&p_leaf_port_group->ports, 0,
					 (void *)&p_port);
			p_sw->p_osm_sw->new_lft[hca_lid] = p_port->port_num;

			OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_DEBUG,
				"Switch %s: set path to CN LID %u through port %u\n",
				tuple_to_str(p_sw->tuple),
				hca_lid, p_port->port_num);

			/* set local min hop table(LID) to route to the CA */
			sw_set_hops(p_sw, hca_lid, p_port->port_num, 1, FALSE);

			/* Assign downgoing ports by stepping up.
			   Since we're routing here only CNs, we're routing it as REAL
			   LID and updating fat-tree balancing counters. */
			fabric_route_downgoing_by_going_up(p_ftree, p_sw,	/* local switch - used as a route-downgoing alg. start point */
							   NULL,	/* prev. position switch */
							   hca_lid,	/* LID that we're routing to */
							   TRUE,	/* whether this path to HCA should by tracked by counters */
							   FALSE,	/* whether target lid is a switch or not */
							   0,	/* Number of reverse hops allowed */
							   0,	/* Number of reverse hops done yet */
							   1);	/* Number of hops done yet */

			/* count how many real targets have been routed from this leaf switch */
			routed_targets_on_leaf++;
		}

		/* We're done with the real targets (all CNs) of this leaf switch.
		   Now route the dummy HCAs that are missing or that are non-CNs.
		   When routing to dummy HCAs we don't fill lid matrices. */
		if (p_ftree->max_cn_per_leaf > routed_targets_on_leaf) {
			OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_DEBUG,
				"Routing %u dummy CAs\n",
				p_ftree->max_cn_per_leaf -
				p_sw->down_port_groups_num);
			for (j = 0; j <
			     p_ftree->max_cn_per_leaf - routed_targets_on_leaf;
			     j++) {
				ftree_sw_t *p_next_sw, *p_ftree_sw;
				/* assign downgoing ports by stepping up */
				fabric_route_downgoing_by_going_up(p_ftree, p_sw,	/* local switch - used as a route-downgoing alg. start point */
								   NULL,	/* prev. position switch */
								   0,	/* LID that we're routing to - ignored for dummy HCA */
								   TRUE,	/* whether this path to HCA should by tracked by counters */
								   FALSE,	/* Whether the target LID is a switch or not */
								   0,	/* Number of reverse hops allowed */
								   0,	/* Number of reverse hops done yet */
								   1);	/* Number of hops done yet */

				p_next_sw = (ftree_sw_t *) cl_qmap_head(&p_ftree->sw_tbl);
				/* need to clean the LID 0 hops for dummy node */
				while (p_next_sw != (ftree_sw_t *) cl_qmap_end(&p_ftree->sw_tbl)) {
					p_ftree_sw = p_next_sw;
					p_next_sw = (ftree_sw_t *) cl_qmap_next(&p_ftree_sw->map_item);
					p_ftree_sw->hops[0] = OSM_NO_PATH;
					p_ftree_sw->p_osm_sw->new_lft[0] = OSM_NO_PATH;
				}

			}
		}
	}
	/* done going through all the leaf switches */
	OSM_LOG_EXIT(&p_ftree->p_osm->log);
}				/* fabric_route_to_cns() */