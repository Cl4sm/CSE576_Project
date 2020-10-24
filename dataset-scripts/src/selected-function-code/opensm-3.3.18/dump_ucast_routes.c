static void dump_ucast_routes(cl_map_item_t * item, FILE * file, void *cxt)
{
	const osm_node_t *p_node;
	osm_port_t *p_port;
	uint8_t port_num;
	uint8_t num_hops;
	uint8_t best_hops;
	uint8_t best_port;
	uint16_t max_lid_ho;
	uint16_t lid_ho, base_lid;
	boolean_t direct_route_exists = FALSE;
	boolean_t dor;
	osm_switch_t *p_sw = (osm_switch_t *) item;
	osm_opensm_t *p_osm = cxt;

	p_node = p_sw->p_node;

	max_lid_ho = p_sw->max_lid_ho;

	fprintf(file, "dump_ucast_routes: "
		"Switch 0x%016" PRIx64 "\nLID    : Port : Hops : Optimal\n",
		cl_ntoh64(osm_node_get_node_guid(p_node)));

	dor = (p_osm->routing_engine_used &&
	       p_osm->routing_engine_used->type == OSM_ROUTING_ENGINE_TYPE_DOR);

	for (lid_ho = 1; lid_ho <= max_lid_ho; lid_ho++) {
		fprintf(file, "0x%04X : ", lid_ho);

		p_port = osm_get_port_by_lid_ho(&p_osm->subn, lid_ho);
		if (!p_port) {
			fprintf(file, "UNREACHABLE\n");
			continue;
		}

		port_num = osm_switch_get_port_by_lid(p_sw, lid_ho);
		if (port_num == OSM_NO_PATH) {
			/*
			   This may occur if there are 'holes' in the existing
			   LID assignments.  Running SM with --reassign_lids
			   will reassign and compress the LID range.  The
			   subnet should work fine either way.
			 */
			fprintf(file, "UNREACHABLE\n");
			continue;
		}
		/*
		   Switches can lie about which port routes a given
		   lid due to a recent reconfiguration of the subnet.
		   Therefore, ensure that the hop count is better than
		   OSM_NO_PATH.
		 */
		if (p_port->p_node->sw) {
			/* Target LID is switch.
			   Get its base lid and check hop count for this base LID only. */
			base_lid = osm_node_get_base_lid(p_port->p_node, 0);
			base_lid = cl_ntoh16(base_lid);
			num_hops =
			    osm_switch_get_hop_count(p_sw, base_lid, port_num);
		} else {
			/* Target LID is not switch (CA or router).
			   Check if we have route to this target from current switch. */
			num_hops =
			    osm_switch_get_hop_count(p_sw, lid_ho, port_num);
			if (num_hops != OSM_NO_PATH) {
				direct_route_exists = TRUE;
				base_lid = lid_ho;
			} else {
				osm_physp_t *p_physp = p_port->p_physp;

				if (!p_physp || !p_physp->p_remote_physp ||
				    !p_physp->p_remote_physp->p_node->sw)
					num_hops = OSM_NO_PATH;
				else {
					base_lid =
					    osm_node_get_base_lid(p_physp->
								  p_remote_physp->
								  p_node, 0);
					base_lid = cl_ntoh16(base_lid);
					num_hops =
					    p_physp->p_remote_physp->p_node->
					    sw ==
					    p_sw ? 0 :
					    osm_switch_get_hop_count(p_sw,
								     base_lid,
								     port_num);
				}
			}
		}

		if (num_hops == OSM_NO_PATH) {
			fprintf(file, "%03u  : HOPS UNKNOWN\n", port_num);
			continue;
		}

		best_hops = osm_switch_get_least_hops(p_sw, base_lid);
		if (!p_port->p_node->sw && !direct_route_exists) {
			best_hops++;
			num_hops++;
		}

		fprintf(file, "%03u  : %02u   : ", port_num, num_hops);

		if (best_hops == num_hops)
			fprintf(file, "yes");
		else {
			/* No LMC Optimization */
			best_port = osm_switch_recommend_path(p_sw, p_port,
							      lid_ho, 1, TRUE,
							      FALSE, dor,
							      p_osm->subn.opt.port_shifting,
							      p_osm->subn.opt.scatter_ports);
			fprintf(file, "No %u hop path possible via port %u!",
				best_hops, best_port);
		}

		fprintf(file, "\n");
	}
}