static void state_mgr_report_new_ports(IN osm_sm_t * sm)
{
	ib_gid_t port_gid;
	ib_mad_notice_attr_t notice;
	ib_api_status_t status;
	ib_net64_t port_guid;
	cl_map_item_t *p_next;
	osm_port_t *p_port;
	uint16_t min_lid_ho;
	uint16_t max_lid_ho;

	OSM_LOG_ENTER(sm->p_log);

	CL_PLOCK_ACQUIRE(sm->p_lock);
	p_next = cl_qmap_head(&sm->p_subn->port_guid_tbl);
	while (p_next != cl_qmap_end(&sm->p_subn->port_guid_tbl)) {
		p_port = (osm_port_t *) p_next;
		p_next = cl_qmap_next(p_next);

		if (!p_port->is_new)
			continue;

		port_guid = osm_port_get_guid(p_port);
		/* issue a notice - trap 64 */

		/* details of the notice */
		notice.generic_type = 0x83;	/* is generic subn mgt type */
		ib_notice_set_prod_type_ho(&notice, 4);	/* A Class Manager generator */
		/* endport becomes reachable */
		notice.g_or_v.generic.trap_num = CL_HTON16(64);
		/* The sm_base_lid is saved in network order already. */
		notice.issuer_lid = sm->p_subn->sm_base_lid;
		/* following C14-72.1.1 and table 119 p739 */
		/* we need to provide the GID */
		port_gid.unicast.prefix = sm->p_subn->opt.subnet_prefix;
		port_gid.unicast.interface_id = port_guid;
		memcpy(&(notice.data_details.ntc_64_67.gid), &(port_gid),
		       sizeof(ib_gid_t));

		/* According to page 653 - the issuer gid in this case of trap
		 * is the SM gid, since the SM is the initiator of this trap. */
		notice.issuer_gid.unicast.prefix =
		    sm->p_subn->opt.subnet_prefix;
		notice.issuer_gid.unicast.interface_id =
		    sm->p_subn->sm_port_guid;

		status = osm_report_notice(sm->p_log, sm->p_subn, &notice);
		if (status != IB_SUCCESS)
			OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 3318: "
				"Error sending trap reports on GUID:0x%016"
				PRIx64 " (%s)\n", port_gid.unicast.interface_id,
				ib_get_err_str(status));
		osm_port_get_lid_range_ho(p_port, &min_lid_ho, &max_lid_ho);
		OSM_LOG(sm->p_log, OSM_LOG_INFO,
			"Discovered new port with GUID:0x%016" PRIx64
			" LID range [%u,%u] of node: %s\n",
			cl_ntoh64(port_gid.unicast.interface_id),
			min_lid_ho, max_lid_ho,
			p_port->p_node ? p_port->p_node->
			print_desc : "UNKNOWN");

		p_port->is_new = 0;
	}
	CL_PLOCK_RELEASE(sm->p_lock);

	OSM_LOG_EXIT(sm->p_log);
}