static boolean_t
fabric_route_downgoing_by_going_up(IN ftree_fabric_t * p_ftree,
				   IN ftree_sw_t * p_sw,
				   IN ftree_sw_t * p_prev_sw,
				   IN uint16_t target_lid,
				   IN boolean_t is_main_path,
				   IN boolean_t is_target_a_sw,
				   IN uint16_t reverse_hop_credit,
				   IN uint16_t reverse_hops,
				   IN uint8_t current_hops)
{
	ftree_sw_t *p_remote_sw;
	uint16_t ports_num;
	ftree_port_group_t *p_group;
	ftree_port_t *p_port;
	ftree_port_group_t *p_min_group;
	ftree_port_t *p_min_port;
	uint16_t i;
	uint16_t j;
	boolean_t created_route = FALSE;
	boolean_t routed = FALSE;


	/* Assign upgoing ports by stepping down, starting on THIS switch */
	created_route = fabric_route_upgoing_by_going_down(p_ftree, p_sw,	/* local switch - used as a route-upgoing alg. start point */
							   p_prev_sw,	/* switch that we went up from (NULL means that we went down) */
							   target_lid,	/* LID that we're routing to */
							   is_main_path,	/* whether this path to HCA should by tracked by counters */
							   is_target_a_sw,	/* Whether target lid is a switch or not */
							   current_hops);	/* Number of hops done up to this point */

	/* recursion stop condition - if it's a root switch, */
	if (p_sw->rank == 0) {
		if (reverse_hop_credit > 0) {
			/* We go up by going down as we have some reverse_hop_credit left */
			/* We use the index to scatter a bit the reverse up routes */
			p_sw->down_port_groups_idx =
			    (p_sw->down_port_groups_idx +
			     1) % p_sw->down_port_groups_num;
			i = p_sw->down_port_groups_idx;
			for (j = 0; j < p_sw->down_port_groups_num; j++) {

				p_group = p_sw->down_port_groups[i];
				i = (i + 1) % p_sw->down_port_groups_num;

				/* Skip this port group unless it points to a switch */
				if (p_group->remote_node_type !=
				    IB_NODE_TYPE_SWITCH)
					continue;
				p_remote_sw = p_group->remote_hca_or_sw.p_sw;

				created_route |= fabric_route_downgoing_by_going_up(p_ftree, p_remote_sw,	/* remote switch - used as a route-downgoing alg. next step point */
										    p_sw,	/* this switch - prev. position switch for the function */
										    target_lid,	/* LID that we're routing to */
										    is_main_path,	/* whether this is path to HCA that should by tracked by counters */
										    is_target_a_sw,	/* Whether target lid is a switch or not */
										    reverse_hop_credit - 1,	/* Remaining reverse_hops allowed */
										    reverse_hops + 1,	/* Number of reverse_hops done up to this point */
										    current_hops
										    +
										    1);
			}

		}
		return created_route;
	}

	/* We should generate a list of port sorted by load so we can find easily the least
	 * going port and explore the other pots on secondary routes more easily (and quickly) */
	bubble_sort_down(p_sw->up_port_groups, p_sw->up_port_groups_num);

	p_min_group = p_sw->up_port_groups[0];
	/* Find the least loaded upgoing port in the selected group */
	p_min_port = NULL;
	ports_num = (uint16_t) cl_ptr_vector_get_size(&p_min_group->ports);
	for (j = 0; j < ports_num; j++) {
		cl_ptr_vector_at(&p_min_group->ports, j, (void *)&p_port);
		if (!p_min_port) {
			/* first port that we're checking - use
			   it as a port with the lowest load */
			p_min_port = p_port;
		} else if (p_port->counter_down < p_min_port->counter_down) {
			/* this port is less loaded - use it as min */
			p_min_port = p_port;
		}
	}

	/* At this point we have selected a group and port with the
	   lowest load of downgoing routes.
	   Set on the remote switch how to get to the target_lid -
	   set LFT(target_lid) on the remote switch to the remote port */
	p_remote_sw = p_min_group->remote_hca_or_sw.p_sw;

	/* Four possible cases:
	 *
	 *  1. is_main_path == TRUE:
	 *      - going UP(TRUE,TRUE) on selected min_group and min_port
	 *         + promoting port counter
	 *         + setting path in remote switch fwd tbl
	 *         + setting hops in remote switch on all the ports of selected group
	 *      - going UP(TRUE,FALSE) on rest of the groups, each time on port 0
	 *         + NOT promoting port counter
	 *         + setting path in remote switch fwd tbl if it hasn't been set yet
	 *         + setting hops in remote switch on all the ports of each group
	 *           if it hasn't been set yet
	 *
	 *  2. is_main_path == FALSE:
	 *      - going UP(TRUE,FALSE) on ALL the groups, each time on port 0,
	 *        but only if the remote (upper) switch hasn't been already
	 *        configured for this target LID
	 *         + NOT promoting port counter
	 *         + setting path in remote switch fwd tbl if it hasn't been set yet
	 *         + setting hops in remote switch on all the ports of each group
	 *           if it hasn't been set yet
	 */

	/* covering first half of case 1, and case 3 */
	if (is_main_path) {
		if (p_sw->is_leaf) {
			OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_DEBUG,
				" - Routing MAIN path for %s CA LID %u: %s --> %s\n",
				(target_lid != 0) ? "real" : "DUMMY",
				target_lid,
				tuple_to_str(p_sw->tuple),
				tuple_to_str(p_remote_sw->tuple));
		}
		/* The number of downgoing routes is tracked in the
		   p_group->counter_down p_port->counter_down counters of the
		   group and port that belong to the lower side of the link
		   (on switch with higher rank) */
		p_min_group->counter_down++;
		p_min_port->counter_down++;
		if (p_min_group->counter_down ==
		    (p_min_group->remote_hca_or_sw.p_sw->min_counter_down +
		     1)) {
			recalculate_min_counter_down
			    (p_min_group->remote_hca_or_sw.p_sw);
		}

		/* This LID may already be in the LFT in the reverse_hop feature is used */
		/* We update the LFT only if this LID isn't already present. */

		/* skip if target lid has been already set on remote switch fwd tbl (with a bigger hop count) */
		if ((p_remote_sw->p_osm_sw->new_lft[target_lid] == OSM_NO_PATH)
		    ||
		    ((p_remote_sw->p_osm_sw->new_lft[target_lid] != OSM_NO_PATH)
			     &&
		      (current_hops + 1 <
		       sw_get_least_hops(p_remote_sw, target_lid)))) {

			p_remote_sw->p_osm_sw->new_lft[target_lid] =
				p_min_port->remote_port_num;
			OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_DEBUG,
					"Switch %s: set path to CA LID %u through port %u\n",
					tuple_to_str(p_remote_sw->tuple),
					target_lid,
					p_min_port->remote_port_num);

			/* On the remote switch that is pointed by the min_group,
			set hops for ALL the ports in the remote group. */

			set_hops_on_remote_sw(p_min_group, target_lid,
						      current_hops + 1,
						      is_target_a_sw);
		}
	/* Recursion step: Assign downgoing ports by stepping up, starting on REMOTE switch. */
	created_route |= fabric_route_downgoing_by_going_up(p_ftree,
							    p_remote_sw,	/* remote switch - used as a route-downgoing alg. next step point */
							    p_sw,		/* this switch - prev. position switch for the function */
							    target_lid,		/* LID that we're routing to */
							    is_main_path,	/* whether this is path to HCA that should by tracked by counters */
							    is_target_a_sw,	/* Whether target lid is a switch or not */
							    reverse_hop_credit,	/* Remaining reverse_hops allowed */
							    reverse_hops,	/* Number of reverse_hops done up to this point */
							    current_hops + 1);
	}

	/* What's left to do at this point:
	 *
	 *  1. is_main_path == TRUE:
	 *      - going UP(TRUE,FALSE) on rest of the groups, each time on port 0,
	 *        but only if the remote (upper) switch hasn't been already
	 *        configured for this target LID
	 *         + NOT promoting port counter
	 *         + setting path in remote switch fwd tbl if it hasn't been set yet
	 *         + setting hops in remote switch on all the ports of each group
	 *           if it hasn't been set yet
	 *
	 *  2. is_main_path == FALSE:
	 *      - going UP(TRUE,FALSE) on ALL the groups, each time on port 0,
	 *        but only if the remote (upper) switch hasn't been already
	 *        configured for this target LID
	 *         + NOT promoting port counter
	 *         + setting path in remote switch fwd tbl if it hasn't been set yet
	 *         + setting hops in remote switch on all the ports of each group
	 *           if it hasn't been set yet
	 *
	 *  These two rules can be rephrased this way:
	 *   - foreach UP port group
	 *      + if remote switch has been set with the target LID
	 *         - skip this port group
	 *      + else
	 *         - select port 0
	 *         - do NOT promote port counter
	 *         - set path in remote switch fwd tbl
	 *         - set hops in remote switch on all the ports of this group
	 *         - go UP(TRUE,FALSE) to the remote switch
	 */

	for (i = is_main_path ? 1 : 0; i < p_sw->up_port_groups_num; i++) {
		p_group = p_sw->up_port_groups[i];
		p_remote_sw = p_group->remote_hca_or_sw.p_sw;

		/* skip if target lid has been already set on remote switch fwd tbl (with a bigger hop count) */
		if (p_remote_sw->p_osm_sw->new_lft[target_lid] != OSM_NO_PATH)
			if (current_hops + 1 >=
			    sw_get_least_hops(p_remote_sw, target_lid))
				continue;

		if (p_sw->is_leaf) {
			OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_DEBUG,
				" - Routing SECONDARY path for LID %u: %s --> %s\n",
				target_lid,
				tuple_to_str(p_sw->tuple),
				tuple_to_str(p_remote_sw->tuple));
		}

		/* Routing REAL lids on SECONDARY path means routing
		   switch-to-switch or switch-to-CA paths.
		   We can safely assume that switch will initiate very
		   few traffic, so there's no point wasting runtime on
		   trying to balance these routes - always pick port 0. */
		p_min_port = NULL;
		ports_num = (uint16_t) cl_ptr_vector_get_size(&p_group->ports);
		if(ports_num == 0)
			continue;
		for (j = 0; j < ports_num; j++) {
			cl_ptr_vector_at(&p_group->ports, j, (void *)&p_port);
			if (!p_min_port) {
				/* first port that we're checking - use
				   it as a port with the lowest load */
				p_min_port = p_port;
			} else if (p_port->counter_down <
				   p_min_port->counter_down) {
				/* this port is less loaded - use it as min */
				p_min_port = p_port;
			}
		}

		p_port = p_min_port;
		p_remote_sw->p_osm_sw->new_lft[target_lid] =
		    p_port->remote_port_num;

		/* On the remote switch that is pointed by the p_group,
		   set hops for ALL the ports in the remote group. */

		set_hops_on_remote_sw(p_group, target_lid,
				      current_hops + 1, is_target_a_sw);

		/* Recursion step:
		   Assign downgoing ports by stepping up, starting on REMOTE switch. */
		routed = fabric_route_downgoing_by_going_up(p_ftree, p_remote_sw,	/* remote switch - used as a route-downgoing alg. next step point */
							    p_sw,	/* this switch - prev. position switch for the function */
							    target_lid,	/* LID that we're routing to */
							    FALSE,	/* whether this is path to HCA that should by tracked by counters */
							    is_target_a_sw,	/* Whether target lid is a switch or not */
							    reverse_hop_credit,	/* Remaining reverse_hops allowed */
							    reverse_hops,	/* Number of reverse_hops done up to this point */
							    current_hops + 1);
		created_route |= routed;
	}

	/* Now doing the same thing with horizontal links */
	if (p_sw->sibling_port_groups_num > 0)
		bubble_sort_down(p_sw->sibling_port_groups,
				 p_sw->sibling_port_groups_num);

	for (i = 0; i < p_sw->sibling_port_groups_num; i++) {
		p_group = p_sw->sibling_port_groups[i];
		p_remote_sw = p_group->remote_hca_or_sw.p_sw;

		/* skip if target lid has been already set on remote switch fwd tbl (with a bigger hop count) */
		if (p_remote_sw->p_osm_sw->new_lft[target_lid] != OSM_NO_PATH)
			if (current_hops + 1 >=
			    sw_get_least_hops(p_remote_sw, target_lid))
				continue;

		if (p_sw->is_leaf) {
			OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_DEBUG,
				" - Routing SECONDARY path for LID %u: %s --> %s\n",
				target_lid,
				tuple_to_str(p_sw->tuple),
				tuple_to_str(p_remote_sw->tuple));
		}

		/* Routing REAL lids on SECONDARY path means routing
		   switch-to-switch or switch-to-CA paths.
		   We can safely assume that switch will initiate very
		   few traffic, so there's no point wasting runtime on
		   trying to balance these routes - always pick port 0. */

		p_min_port = NULL;
		ports_num = (uint16_t) cl_ptr_vector_get_size(&p_group->ports);
		for (j = 0; j < ports_num; j++) {
			cl_ptr_vector_at(&p_group->ports, j, (void *)&p_port);
			if (!p_min_port) {
				/* first port that we're checking - use
				   it as a port with the lowest load */
				p_min_port = p_port;
			} else if (p_port->counter_down <
				   p_min_port->counter_down) {
				/* this port is less loaded - use it as min */
				p_min_port = p_port;
			}
		}

		p_port = p_min_port;
		p_remote_sw->p_osm_sw->new_lft[target_lid] =
		    p_port->remote_port_num;

		/* On the remote switch that is pointed by the p_group,
		   set hops for ALL the ports in the remote group. */

		set_hops_on_remote_sw(p_group, target_lid,
				      current_hops + 1, is_target_a_sw);

		/* Recursion step:
		   Assign downgoing ports by stepping up, starting on REMOTE switch. */
		routed = fabric_route_downgoing_by_going_up(p_ftree, p_remote_sw,	/* remote switch - used as a route-downgoing alg. next step point */
							    p_sw,	/* this switch - prev. position switch for the function */
							    target_lid,	/* LID that we're routing to */
							    FALSE,	/* whether this is path to HCA that should by tracked by counters */
							    is_target_a_sw,	/* Whether target lid is a switch or not */
							    reverse_hop_credit,	/* Remaining reverse_hops allowed */
							    reverse_hops,	/* Number of reverse_hops done up to this point */
							    current_hops + 1);
		created_route |= routed;
		if (routed) {
			p_min_group->counter_down++;
			p_min_port->counter_down++;
		}
	}

	/* If we don't have any reverse hop credits, we are done */
	if (reverse_hop_credit == 0)
		return created_route;

	if (p_sw->is_leaf)
		return created_route;

	/* We explore all the down group ports */
	/* We try to reverse jump for each of them */
	/* They already have a route to us from the upgoing_by_going_down started earlier */
	/* This is only so it'll continue exploring up, after this step backwards */
	for (i = 0; i < p_sw->down_port_groups_num; i++) {
		p_group = p_sw->down_port_groups[i];
		p_remote_sw = p_group->remote_hca_or_sw.p_sw;

		/* Skip this port group unless it points to a switch */
		if (p_group->remote_node_type != IB_NODE_TYPE_SWITCH)
			continue;

		/* Recursion step:
		   Assign downgoing ports by stepping up, fter doing one step down starting on REMOTE switch. */
		created_route |= fabric_route_downgoing_by_going_up(p_ftree, p_remote_sw,	/* remote switch - used as a route-downgoing alg. next step point */
								    p_sw,	/* this switch - prev. position switch for the function */
								    target_lid,	/* LID that we're routing to */
								    TRUE,	/* whether this is path to HCA that should by tracked by counters */
								    is_target_a_sw,	/* Whether target lid is a switch or not */
								    reverse_hop_credit - 1,	/* Remaining reverse_hops allowed */
								    reverse_hops + 1,	/* Number of reverse_hops done up to this point */
								    current_hops
								    + 1);
	}
	return created_route;

}				/* ftree_fabric_route_downgoing_by_going_up() */