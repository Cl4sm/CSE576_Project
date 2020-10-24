static void print_all_guids(IN osmtest_t * p_osmt)
{
	ib_api_status_t status;
	uint32_t num_ports = MAX_LOCAL_IBPORTS;
	ib_port_attr_t attr_array[MAX_LOCAL_IBPORTS] = { {0} };
	uint32_t i;

	/*
	   Call the transport layer for a list of local port
	   GUID values.
	 */
	status =
	    osm_vendor_get_all_port_attr(p_osmt->p_vendor, attr_array,
					 &num_ports);
	if (status != IB_SUCCESS) {
		printf("\nError from osm_vendor_get_all_port_attr (%x)\n",
		       status);
		return;
	}

	printf("\nListing GUIDs:\n");
	for (i = 0; i < num_ports; i++)
		printf("Port %i: 0x%" PRIx64 "\n", i,
		       cl_hton64(attr_array[i].port_guid));
}