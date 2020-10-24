static void fabric_route_to_non_cns(IN ftree_fabric_t * p_ftree)
{
	ftree_sw_t *p_sw;
	ftree_hca_t *p_hca;
	ftree_hca_t *p_next_hca;
	ftree_port_t *p_hca_port;
	ftree_port_group_t *p_hca_port_group;
	uint16_t hca_lid;
	unsigned port_num_on_switch;
	unsigned i;

	OSM_LOG_ENTER(&p_ftree->p_osm->log);

	p_next_hca = (ftree_hca_t *) cl_qmap_head(&p_ftree->hca_tbl);
	while (p_next_hca != (ftree_hca_t *) cl_qmap_end(&p_ftree->hca_tbl)) {
		p_hca = p_next_hca;
		p_next_hca = (ftree_hca_t *) cl_qmap_next(&p_hca->map_item);

		for (i = 0; i < p_hca->up_port_groups_num; i++) {
			p_hca_port_group = p_hca->up_port_groups[i];

			/* skip this port if it's CN, in which case it has been already routed */
			if (p_hca_port_group->is_cn)
				continue;

			/* skip this port if it is not connected to switch */
			if (p_hca_port_group->remote_node_type !=
			    IB_NODE_TYPE_SWITCH)
				continue;

			p_sw = p_hca_port_group->remote_hca_or_sw.p_sw;
			hca_lid = p_hca_port_group->base_lid;

			/* set switches  LFT(LID) to the port that is connected to HCA */
			cl_ptr_vector_at(&p_hca_port_group->ports, 0,
					 (void *)&p_hca_port);
			port_num_on_switch = p_hca_port->remote_port_num;
			p_sw->p_osm_sw->new_lft[hca_lid] = port_num_on_switch;

			OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_DEBUG,
				"Switch %s: set path to non-CN HCA LID %u through port %u\n",
				tuple_to_str(p_sw->tuple),
				hca_lid, port_num_on_switch);

			/* set local min hop table(LID) to route to the CA */
			sw_set_hops(p_sw, hca_lid, port_num_on_switch,	/* port num */
				    1, FALSE);	/* hops */

			/* Assign downgoing ports by stepping up.
			   We're routing REAL targets. They are not CNs and not included
			   in the leafs array, but we treat them as MAIN path to allow load
			   leveling, which means that the counters will be updated. */
			fabric_route_downgoing_by_going_up(p_ftree, p_sw,	/* local switch - used as a route-downgoing alg. start point */
							   NULL,	/* prev. position switch */
							   hca_lid,	/* LID that we're routing to */
							   TRUE,	/* whether this path to HCA should by tracked by counters */
							   FALSE,	/* Whether the target LID is a switch or not */
							   p_hca_port_group->is_io ? p_ftree->p_osm->subn.opt.max_reverse_hops : 0,	/* Number or reverse hops allowed */
							   0,	/* Number or reverse hops done yet */
							   1);	/* Number of hops done yet */
		}
		/* done with all the port groups of this HCA - go to next HCA */
	}

	OSM_LOG_EXIT(&p_ftree->p_osm->log);
}				/* fabric_route_to_non_cns() */