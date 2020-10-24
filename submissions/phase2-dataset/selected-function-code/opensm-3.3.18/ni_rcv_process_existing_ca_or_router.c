static void ni_rcv_process_existing_ca_or_router(IN osm_sm_t * sm,
						 IN osm_node_t * p_node,
						 IN const osm_madw_t * p_madw)
{
	ib_node_info_t *p_ni;
	ib_smp_t *p_smp;
	osm_port_t *p_port;
	osm_port_t *p_port_check;
	uint8_t port_num;
	osm_dr_path_t *p_dr_path;
	osm_alias_guid_t *p_alias_guid, *p_alias_guid_check;

	OSM_LOG_ENTER(sm->p_log);

	p_smp = osm_madw_get_smp_ptr(p_madw);
	p_ni = ib_smp_get_payload_ptr(p_smp);
	port_num = ib_node_info_get_local_port_num(p_ni);

	/*
	   Determine if we have encountered this node through a
	   previously undiscovered port.  If so, build the new
	   port object.
	 */
	p_port = osm_get_port_by_guid(sm->p_subn, p_ni->port_guid);
	if (!p_port) {
		OSM_LOG(sm->p_log, OSM_LOG_VERBOSE,
			"Creating new port object with GUID 0x%" PRIx64 "\n",
			cl_ntoh64(p_ni->port_guid));

		osm_node_init_physp(p_node, port_num, p_madw);

		p_port = osm_port_new(p_ni, p_node);
		if (PF(p_port == NULL)) {
			OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 0D04: "
				"Unable to create new port object\n");
			goto Exit;
		}

		/*
		   Add the new port object to the database.
		 */
		p_port_check =
		    (osm_port_t *) cl_qmap_insert(&sm->p_subn->port_guid_tbl,
						  p_ni->port_guid,
						  &p_port->map_item);
		if (PF(p_port_check != p_port)) {
			/*
			   We should never be here!
			   Somehow, this port GUID already exists in the table.
			 */
			OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 0D12: "
				"Port 0x%" PRIx64 " already in the database!\n",
				cl_ntoh64(p_ni->port_guid));

			osm_port_delete(&p_port);
			goto Exit;
		}

		p_alias_guid = osm_alias_guid_new(p_ni->port_guid,
						  p_port);
		if (PF(!p_alias_guid)) {
			OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 0D11: "
				"alias guid memory allocation failed"
				" for port GUID 0x%" PRIx64 "\n",
				cl_ntoh64(p_ni->port_guid));
			goto alias_done;
		}

		/* insert into alias guid table */
		p_alias_guid_check =
			(osm_alias_guid_t *) cl_qmap_insert(&sm->p_subn->alias_port_guid_tbl,
							    p_alias_guid->alias_guid,
							    &p_alias_guid->map_item);
		if (p_alias_guid_check != p_alias_guid) {
			/* alias GUID is a duplicate */
			OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 0D13: "
				"Duplicate alias port GUID 0x%" PRIx64 "\n",
				cl_ntoh64(p_ni->port_guid));
			osm_alias_guid_delete(&p_alias_guid);
			osm_port_delete(&p_port);
			goto Exit;
		}

alias_done:
		/* If we are a master, then this means the port is new on the subnet.
		   Mark it as new - need to send trap 64 for these ports.
		   The condition that we are master is true, since if we are in discovering
		   state (meaning we woke up from standby or we are just initializing),
		   then these ports may be new to us, but are not new on the subnet.
		   If we are master, then the subnet as we know it is the updated one,
		   and any new ports we encounter should cause trap 64. C14-72.1.1 */
		if (sm->p_subn->sm_state == IB_SMINFO_STATE_MASTER)
			p_port->is_new = 1;

	} else {
		osm_physp_t *p_physp = osm_node_get_physp_ptr(p_node, port_num);

		if (PF(p_physp == NULL)) {
			OSM_LOG(sm->p_log, OSM_LOG_ERROR, "ERR 0D1C: "
				"No physical port found for node GUID 0x%"
				PRIx64 " port %u. Might be duplicate port GUID\n",
				cl_ntoh64(p_node->node_info.node_guid),
				port_num);
			goto Exit;
		}

		/*
		   Update the DR Path to the port,
		   in case the old one is no longer available.
		 */
		p_dr_path = osm_physp_get_dr_path_ptr(p_physp);

		osm_dr_path_init(p_dr_path, p_smp->hop_count,
				 p_smp->initial_path);
	}

	ni_rcv_get_port_info(sm, p_node, p_madw);

Exit:
	OSM_LOG_EXIT(sm->p_log);
}