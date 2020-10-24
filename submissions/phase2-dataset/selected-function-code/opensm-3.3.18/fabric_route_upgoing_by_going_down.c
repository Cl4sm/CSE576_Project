static boolean_t
fabric_route_upgoing_by_going_down(IN ftree_fabric_t * p_ftree,
				   IN ftree_sw_t * p_sw,
				   IN ftree_sw_t * p_prev_sw,
				   IN uint16_t target_lid,
				   IN boolean_t is_main_path,
				   IN boolean_t is_target_a_sw,
				   IN uint8_t current_hops)
{
	ftree_sw_t *p_remote_sw;
	uint16_t ports_num;
	ftree_port_group_t *p_group;
	ftree_port_t *p_port;
	ftree_port_t *p_min_port;
	uint16_t j;
	uint16_t k;
	boolean_t created_route = FALSE;
	boolean_t routed = 0;
	uint8_t least_hops;

	/* if there is no down-going ports */
	if (p_sw->down_port_groups_num == 0)
		return FALSE;

	/* foreach down-going port group (in load order) */
	bubble_sort_up(p_sw->down_port_groups, p_sw->down_port_groups_num);

	if (p_sw->sibling_port_groups_num > 0)
		bubble_sort_siblings(p_sw->sibling_port_groups,
				     p_sw->sibling_port_groups_num);

	for (k = 0;
	     k <
	     (p_sw->down_port_groups_num +
	      ((target_lid != 0) ? p_sw->sibling_port_groups_num : 0)); k++) {

		if (k < p_sw->down_port_groups_num) {
			p_group = p_sw->down_port_groups[k];
		} else {
			p_group =
			    p_sw->sibling_port_groups[k -
						      p_sw->
						      down_port_groups_num];
		}

		/* If this port group doesn't point to a switch, mark
		   that the route was created and skip to the next group */
		if (p_group->remote_node_type != IB_NODE_TYPE_SWITCH) {
			created_route = TRUE;
			continue;
		}

		if (p_prev_sw
		    && p_group->remote_base_lid == p_prev_sw->base_lid) {
			/* This port group has a port that was used when we entered this switch,
			   which means that the current group points to the switch where we were
			   at the previous step of the algorithm (before going up).
			   Skipping this group. */
			continue;
		}

		/* find the least loaded port of the group (in indexing order) */
		p_min_port = NULL;
		ports_num = (uint16_t) cl_ptr_vector_get_size(&p_group->ports);
		if(ports_num == 0)
			continue;

		for (j = 0; j < ports_num; j++) {
			cl_ptr_vector_at(&p_group->ports, j, (void *)&p_port);
			/* first port that we're checking - set as port with the lowest load */
			/* or this port is less loaded - use it as min */
			if (!p_min_port ||
			    p_port->counter_up < p_min_port->counter_up)
				p_min_port = p_port;
		}
		/* At this point we have selected a port in this group with the
		   lowest load of upgoing routes.
		   Set on the remote switch how to get to the target_lid -
		   set LFT(target_lid) on the remote switch to the remote port */
		p_remote_sw = p_group->remote_hca_or_sw.p_sw;
		least_hops = sw_get_least_hops(p_remote_sw, target_lid);

		if (least_hops != OSM_NO_PATH) {
			/* Loop in the fabric - we already routed the remote switch
			   on our way UP, and now we see it again on our way DOWN */
			OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_DEBUG,
				"Loop of length %d in the fabric:\n                             "
				"Switch %s (LID %u) closes loop through switch %s (LID %u)\n",
				current_hops,
				tuple_to_str(p_remote_sw->tuple),
				p_group->base_lid,
				tuple_to_str(p_sw->tuple),
				p_group->remote_base_lid);
			/* We skip only if we have come through a longer path */
			if (current_hops + 1 >= least_hops)
				continue;
		}

		/* Four possible cases:
		 *
		 *  1. is_main_path == TRUE:
		 *      - going DOWN(TRUE,TRUE) through ALL the groups
		 *         + promoting port counter
		 *         + setting path in remote switch fwd tbl
		 *         + setting hops in remote switch on all the ports of each group
		 *
		 *  2. is_main_path == FALSE:
		 *      - going DOWN(TRUE,FALSE) through ALL the groups but only if
		 *        the remote (lower) switch hasn't been already configured
		 *        for this target LID (or with a longer path)
		 *         + promoting port counter
		 *         + setting path in remote switch fwd tbl if it hasn't been set yet
		 *         + setting hops in remote switch on all the ports of each group
		 *           if it hasn't been set yet
		 */

		/* setting fwd tbl port only */
		p_remote_sw->p_osm_sw->new_lft[target_lid] =
			    p_min_port->remote_port_num;
		OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_DEBUG,
				"Switch %s: set path to CA LID %u through port %u\n",
				tuple_to_str(p_remote_sw->tuple),
				target_lid, p_min_port->remote_port_num);

		/* On the remote switch that is pointed by the p_group,
			set hops for ALL the ports in the remote group. */

		set_hops_on_remote_sw(p_group, target_lid,
				      current_hops + 1, is_target_a_sw);

		/* Recursion step:
		   Assign upgoing ports by stepping down, starting on REMOTE switch */
		routed = fabric_route_upgoing_by_going_down(p_ftree, p_remote_sw,	/* remote switch - used as a route-upgoing alg. start point */
							    NULL,	/* prev. position - NULL to mark that we went down and not up */
							    target_lid,	/* LID that we're routing to */
							    is_main_path,	/* whether this is path to HCA that should by tracked by counters */
							    is_target_a_sw,	/* Whether target lid is a switch or not */
							    current_hops + 1);	/* Number of hops done to this point */
		created_route |= routed;
		/* Counters are promoted only if a route toward a node is created */
		if (routed) {
			p_min_port->counter_up++;
			p_group->counter_up++;
			p_group->hca_or_sw.p_sw->counter_up_changed = TRUE;
		}
	}
	/* done scanning all the down-going port groups */

	/* if the route was created, promote the index that
	   indicates which group should we start with when
	   going through all the downgoing groups */
	if (created_route)
		p_sw->down_port_groups_idx = (p_sw->down_port_groups_idx + 1)
		    % p_sw->down_port_groups_num;

	return created_route;
}				/* fabric_route_upgoing_by_going_down() */