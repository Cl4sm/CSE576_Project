static void fabric_route_to_switches(IN ftree_fabric_t * p_ftree)
{
	ftree_sw_t *p_sw;
	ftree_sw_t *p_next_sw;

	OSM_LOG_ENTER(&p_ftree->p_osm->log);

	p_next_sw = (ftree_sw_t *) cl_qmap_head(&p_ftree->sw_tbl);
	while (p_next_sw != (ftree_sw_t *) cl_qmap_end(&p_ftree->sw_tbl)) {
		p_sw = p_next_sw;
		p_next_sw = (ftree_sw_t *) cl_qmap_next(&p_sw->map_item);

		/* set local LFT(LID) to 0 (route to itself) */
		p_sw->p_osm_sw->new_lft[p_sw->base_lid] = 0;

		OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_DEBUG,
			"Switch %s (LID %u): routing switch-to-switch paths\n",
			tuple_to_str(p_sw->tuple), p_sw->base_lid);

		/* set min hop table of the switch to itself */
		sw_set_hops(p_sw, p_sw->base_lid, 0,	/* port_num */
			    0, TRUE);	/* hops     */

		fabric_route_downgoing_by_going_up(p_ftree, p_sw,	/* local switch - used as a route-downgoing alg. start point */
						   NULL,	/* prev. position switch */
						   p_sw->base_lid,	/* LID that we're routing to */
						   FALSE,	/* whether this path to HCA should by tracked by counters */
						   TRUE,	/* Whether the target LID is a switch or not */
						   0,	/* Number of reverse hops allowed */
						   0,	/* Number of reverse hops done yet */
						   0);	/* Number of hops done yet */
	}

	OSM_LOG_EXIT(&p_ftree->p_osm->log);
}				/* fabric_route_to_switches() */