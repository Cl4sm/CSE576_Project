ib_api_status_t
osmt_run_slvl_and_vlarb_records_flow(IN osmtest_t * const p_osmt)
{
	ib_api_status_t status;
	FILE *fh;
	ib_net16_t test_lid;
	uint8_t lmc;

	OSM_LOG_ENTER(&p_osmt->log);

	fh = fopen("qos.txt", "w");
	if (!fh) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0474: "
			"Failed to open file qos.txt for writing\n");
		status = IB_ERROR;
		goto Exit;
	}

	/* go over all ports in the subnet */
	status = osmt_query_all_ports_vl_arb(p_osmt, fh);
	if (status != IB_SUCCESS) {
		goto Exit;
	}

	status = osmt_query_all_ports_slvl_map(p_osmt, fh);
	if (status != IB_SUCCESS) {
		goto Exit;
	}

	/* If LMC > 0, test non base LID SA QoS Record requests */
	status =
	    osmtest_get_local_port_lmc(p_osmt, p_osmt->local_port.lid, &lmc);
	if (status != IB_SUCCESS)
		goto Exit;

	if (lmc != 0) {
		test_lid = cl_ntoh16(p_osmt->local_port.lid + 1);

		status = osmt_query_vl_arb(p_osmt, test_lid, 0, 1, NULL);
		if (status != IB_SUCCESS)
			goto Exit;

		status = osmt_query_slvl_map(p_osmt, test_lid, 0, 0, NULL);
		if (status != IB_SUCCESS)
			goto Exit;
	}

Exit:
	if (fh)
		fclose(fh);
	OSM_LOG_EXIT(&p_osmt->log);
	return status;
}