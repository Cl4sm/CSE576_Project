ib_api_status_t
osmtest_bind(IN osmtest_t * p_osmt,
	     IN uint16_t max_lid, IN ib_net64_t guid OPTIONAL)
{
	uint32_t port_index;
	ib_api_status_t status;
	uint32_t num_ports = MAX_LOCAL_IBPORTS;
	ib_port_attr_t attr_array[MAX_LOCAL_IBPORTS] = { {0} };
	ib_gid_t gid[MAX_LOCAL_IBPORTS];
	int i;

	OSM_LOG_ENTER(&p_osmt->log);

	for (i = 0; i < MAX_LOCAL_IBPORTS; i++) {
		attr_array[i].num_gids = 1;
		attr_array[i].p_gid_table = &gid[i];
	}

	/*
	 * Call the transport layer for a list of local port
	 * GUID values.
	 */
	status = osm_vendor_get_all_port_attr(p_osmt->p_vendor,
					      attr_array, &num_ports);
	if (status != IB_SUCCESS) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0134: "
			"Failure getting local port attributes (%s)\n",
			ib_get_err_str(status));
		goto Exit;
	}

	if (guid == 0) {
		/*
		 * User needs prompting for the local port GUID with which
		 * to bind.
		 */
		port_index =
		    osmtest_get_user_port(p_osmt, attr_array, num_ports);

		if (num_ports == 0) {
			OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0135: "
				"No local ports.  Unable to proceed\n");
			goto Exit;
		}
		guid = attr_array[port_index].port_guid;
	} else {
		for (port_index = 0; port_index < num_ports; port_index++) {
			if (attr_array[port_index].port_guid == guid)
				break;
		}

		if (port_index == num_ports) {
			OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0136: "
				"No local port with guid 0x%016" PRIx64 "\n",
				cl_ntoh64(guid));
			status = IB_NOT_FOUND;
			goto Exit;
		}
	}

	/*
	 * Copy the port info for the selected port.
	 */
	memcpy(&p_osmt->local_port, &attr_array[port_index],
	       sizeof(p_osmt->local_port) - sizeof(p_osmt->local_port.p_gid_table));
	if (p_osmt->local_port.num_gids) {
		p_osmt->local_port_gid.unicast.prefix = p_osmt->local_port.p_gid_table[0].unicast.prefix;
		p_osmt->local_port_gid.unicast.interface_id = p_osmt->local_port.p_gid_table[0].unicast.interface_id;
	} else {
		p_osmt->local_port_gid.unicast.prefix = IB_DEFAULT_SUBNET_PREFIX_HO;
		p_osmt->local_port_gid.unicast.interface_id = attr_array[port_index].port_guid;
		p_osmt->local_port.p_gid_table = NULL;
	}
	p_osmt->local_port.num_gids = 1;
	p_osmt->local_port.p_gid_table = &p_osmt->local_port_gid;

	/* bind to the SA */
	OSM_LOG(&p_osmt->log, OSM_LOG_DEBUG,
		"Using port with SM LID:0x%04X\n", p_osmt->local_port.sm_lid);
	p_osmt->max_lid = max_lid;

	p_osmt->h_bind =
	    osmv_bind_sa(p_osmt->p_vendor, &p_osmt->mad_pool, guid);

	if (p_osmt->h_bind == OSM_BIND_INVALID_HANDLE) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0137: "
			"Unable to bind to SA\n");
		status = IB_ERROR;
		goto Exit;
	}

Exit:
	OSM_LOG_EXIT(&p_osmt->log);
	return (status);
}