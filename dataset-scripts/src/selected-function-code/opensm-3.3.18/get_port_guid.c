ib_net64_t get_port_guid(IN osmtest_t * p_osmt, uint64_t port_guid)
{
	ib_api_status_t status;
	uint32_t num_ports = MAX_LOCAL_IBPORTS;
	ib_port_attr_t attr_array[MAX_LOCAL_IBPORTS] = { {0} };
	uint32_t i;

	/*
	   Call the transport layer for a list of local port
	   GUID values.
	 */
/* "local ports" is(?) phys, shouldn't this exclude port 0 then ? */
	status =
	    osm_vendor_get_all_port_attr(p_osmt->p_vendor, attr_array,
					 &num_ports);
	if (status != IB_SUCCESS) {
		printf("\nError from osm_vendor_get_all_port_attr (%x)\n",
		       status);
		return (0);
	}

	if (num_ports == 1) {
		printf("using default guid 0x%" PRIx64 "\n",
		       cl_hton64(attr_array[0].port_guid));
		return (attr_array[0].port_guid);
	}

	for (i = 0; i < num_ports; i++) {
		if (attr_array[i].port_guid == port_guid ||
		    (!port_guid && attr_array[i].link_state > IB_LINK_DOWN))
			return attr_array[i].port_guid;
	}

	return 0;
}