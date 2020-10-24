static ib_api_status_t
__osmv_get_lid_and_sm_lid_by_port_guid(IN osm_vendor_t * const p_vend,
				       IN ib_net64_t port_guid,
				       IN OUT uint64_t * p_lids_update_time_sec,
				       OUT uint16_t * lid,
				       OUT uint16_t * sm_lid)
{

	ib_api_status_t status;
	ib_port_attr_t *p_attr_array;
	uint32_t num_ports;
	uint32_t port_num;

	OSM_LOG_ENTER(p_vend->p_log);

	/* use prevous values if current time is close enough to previous query */
	if (cl_get_time_stamp_sec() <= *p_lids_update_time_sec + 30) {
		OSM_LOG(p_vend->p_log, OSM_LOG_DEBUG,
			"Using previously stored lid:0x%04x sm_lid:0x%04x\n",
			*lid, *sm_lid);
		status = IB_SUCCESS;
		goto Exit;
	}

	/* obtain the number of available ports */
	num_ports = 0;
	status = osm_vendor_get_all_port_attr(p_vend, NULL, &num_ports);
	if (status != IB_INSUFFICIENT_MEMORY) {
		OSM_LOG(p_vend->p_log, OSM_LOG_ERROR, "ERR 0503: "
			"Expected to get the IB_INSUFFICIENT_MEMORY but got: %s\n",
			ib_get_err_str(status));
		status = IB_ERROR;
		goto Exit;
	}

	OSM_LOG(p_vend->p_log, OSM_LOG_DEBUG,
		"Found total of %u ports. Looking for guid:0x%016" PRIx64 "\n",
		num_ports, cl_ntoh64(port_guid));

	/* allocate the attributes */
	p_attr_array =
	    (ib_port_attr_t *) malloc(sizeof(ib_port_attr_t) * num_ports);

	/* obtain the attributes */
	status = osm_vendor_get_all_port_attr(p_vend, p_attr_array, &num_ports);
	if (status != IB_SUCCESS) {
		OSM_LOG(p_vend->p_log, OSM_LOG_ERROR, "ERR 0504: "
			"Failed to get port attributes (error: %s)\n",
			ib_get_err_str(status));
		free(p_attr_array);
		goto Exit;
	}

	status = IB_ERROR;
	/* find the port requested in the list */
	for (port_num = 0; (port_num < num_ports) && (status == IB_ERROR);
	     port_num++) {
		if (p_attr_array[port_num].port_guid == port_guid) {
			*lid = p_attr_array[port_num].lid;
			*sm_lid = p_attr_array[port_num].sm_lid;
			*p_lids_update_time_sec = cl_get_time_stamp_sec();
			status = IB_SUCCESS;
			OSM_LOG(p_vend->p_log, OSM_LOG_DEBUG,
				"Found guid:0x%016" PRIx64 " with idx:%d\n",
				cl_ntoh64(port_guid), port_num);
		}
	}

	free(p_attr_array);

Exit:
	OSM_LOG_EXIT(p_vend->p_log);
	return (status);
}