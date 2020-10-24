static void mcast_mgr_subdivide(osm_sm_t * sm, uint16_t mlid_ho,
				osm_switch_t * p_sw, cl_qlist_t * p_list,
				cl_qlist_t * list_array, uint8_t array_size)
{
	uint8_t port_num;
	boolean_t ignore_existing;
	osm_mcast_work_obj_t *p_wobj;

	OSM_LOG_ENTER(sm->p_log);

	/*
	   For Multicast Groups, we don't want to count on previous
	   configurations - since we can easily generate a storm
	   by loops.
	 */
	ignore_existing = TRUE;

	/*
	   Subdivide the set of ports into non-overlapping subsets
	   that will be routed to other switches.
	 */
	while ((p_wobj =
		(osm_mcast_work_obj_t *) cl_qlist_remove_head(p_list)) !=
	       (osm_mcast_work_obj_t *) cl_qlist_end(p_list)) {
		port_num =
		    osm_switch_recommend_mcast_path(p_sw, p_wobj->p_port,
						    mlid_ho, ignore_existing);
		if (port_num == OSM_NO_PATH) {
			/*
			   This typically occurs if the switch does not support
			   multicast and the multicast tree must branch at this
			   switch.
			 */
			OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 0A03: "
				"Error routing MLID 0x%X through switch 0x%"
				PRIx64 " %s\n"
				"\t\t\t\tNo multicast paths from this switch "
				"for port with LID %u\n", mlid_ho,
				cl_ntoh64(osm_node_get_node_guid(p_sw->p_node)),
				p_sw->p_node->print_desc,
				cl_ntoh16(osm_port_get_base_lid
					  (p_wobj->p_port)));
			mcast_work_obj_delete(p_wobj);
			continue;
		}

		if (port_num >= array_size) {
			OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 0A04: "
				"Error routing MLID 0x%X through switch 0x%"
				PRIx64 " %s\n"
				"\t\t\t\tNo multicast paths from this switch "
				"to port with LID %u\n", mlid_ho,
				cl_ntoh64(osm_node_get_node_guid(p_sw->p_node)),
				p_sw->p_node->print_desc,
				cl_ntoh16(osm_port_get_base_lid
					  (p_wobj->p_port)));
			mcast_work_obj_delete(p_wobj);
			continue;
		}

		cl_qlist_insert_tail(&list_array[port_num], &p_wobj->list_item);
	}

	OSM_LOG_EXIT(sm->p_log);
}