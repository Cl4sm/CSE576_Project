static ib_api_status_t osmtest_validate_against_db(IN osmtest_t * const p_osmt)
{
	ib_api_status_t status = IB_SUCCESS;
	ib_gid_t portgid, mgid;
	osmtest_sm_info_rec_t sm_info_rec_opt;
	osmtest_inform_info_t inform_info_opt;
	osmtest_inform_info_rec_t inform_info_rec_opt;
#ifdef VENDOR_RMPP_SUPPORT
	ib_net64_t sm_key;
	ib_net16_t test_lid;
	uint8_t lmc;
	osmtest_req_context_t context;
#ifdef DUAL_SIDED_RMPP
	osmv_multipath_req_t request;
#endif
	uint8_t i;
#endif

	OSM_LOG_ENTER(&p_osmt->log);

#ifdef VENDOR_RMPP_SUPPORT
	status = osmtest_validate_all_node_recs(p_osmt);
	if (status != IB_SUCCESS)
		goto Exit;
#endif

	status = osmtest_validate_single_node_recs(p_osmt);
	if (status != IB_SUCCESS)
		goto Exit;

	/* Exercise SA PathRecord multicast destination code */
	memset(&context, 0, sizeof(context));
	ib_gid_set_default(&portgid, portguid);
	/* Set IPoIB broadcast MGID */
	mgid.unicast.prefix = CL_HTON64(0xff12401bffff0000ULL);
	mgid.unicast.interface_id = CL_HTON64(0x00000000ffffffffULL);
	/* Can't check status as don't know whether port is running IPoIB */
	osmtest_get_path_rec_by_gid_pair(p_osmt, portgid, mgid, &context);

	/* Other link local unicast PathRecord */
	memset(&context, 0, sizeof(context));
	ib_gid_set_default(&portgid, portguid);
	ib_gid_set_default(&mgid, portguid);
	mgid.raw[7] = 0xff;	/* not default GID prefix */
	/* Can't check status as don't know whether ??? */
	osmtest_get_path_rec_by_gid_pair(p_osmt, portgid, mgid, &context);

	/* Off subnet (site local) unicast PathRecord */
	memset(&context, 0, sizeof(context));
	ib_gid_set_default(&portgid, portguid);
	ib_gid_set_default(&mgid, portguid);
	mgid.raw[1] = 0xc0;	/* site local */
	/* Can't check status as don't know whether ??? */
	osmtest_get_path_rec_by_gid_pair(p_osmt, portgid, mgid, &context);

	/* More than link local scope multicast PathRecord */
	memset(&context, 0, sizeof(context));
	ib_gid_set_default(&portgid, portguid);
	/* Set IPoIB broadcast MGID */
	mgid.unicast.prefix = CL_HTON64(0xff15401bffff0000ULL);	/* site local */
	mgid.unicast.interface_id = CL_HTON64(0x00000000ffffffffULL);
	/* Can't check status as don't know whether port is running IPoIB */
	osmtest_get_path_rec_by_gid_pair(p_osmt, portgid, mgid, &context);

#if defined (VENDOR_RMPP_SUPPORT) && defined (DUAL_SIDED_RMPP)
	memset(&context, 0, sizeof(context));
	memset(&request, 0, sizeof(request));
	request.comp_mask =
	    IB_MPR_COMPMASK_SGIDCOUNT | IB_MPR_COMPMASK_DGIDCOUNT;
	request.sgid_count = 1;
	request.dgid_count = 1;
	ib_gid_set_default(&request.gids[0], portguid);
	ib_gid_set_default(&request.gids[1], portguid);
	status = osmtest_get_multipath_rec(p_osmt, &request, &context);
	if (status != IB_SUCCESS)
		goto Exit;

	memset(&context, 0, sizeof(context));
	memset(&request, 0, sizeof(request));

	OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, EXPECTING_ERRORS_START "\n");
	status = osmtest_get_multipath_rec(p_osmt, &request, &context);
	if (status != IB_SUCCESS) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR,
			"Got error %s\n", ib_get_err_str(status));
	}
	OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, EXPECTING_ERRORS_END "\n");

	if (status == IB_SUCCESS) {
		status = IB_ERROR;
		goto Exit;
	}

	memset(&context, 0, sizeof(context));
	memset(&request, 0, sizeof(request));
	request.comp_mask = IB_MPR_COMPMASK_SGIDCOUNT;
	request.sgid_count = 1;
	ib_gid_set_default(&request.gids[0], portguid);

	OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, EXPECTING_ERRORS_START "\n");
	status = osmtest_get_multipath_rec(p_osmt, &request, &context);
	if (status != IB_SUCCESS) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR,
			"Got error %s\n", ib_get_err_str(status));
	}
	OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, EXPECTING_ERRORS_END "\n");

	if (status == IB_SUCCESS) {
		status = IB_ERROR;
		goto Exit;
	}

	memset(&context, 0, sizeof(context));
	memset(&request, 0, sizeof(request));
	request.comp_mask =
	    IB_MPR_COMPMASK_SGIDCOUNT | IB_MPR_COMPMASK_DGIDCOUNT;
	request.sgid_count = 1;
	request.dgid_count = 1;
	ib_gid_set_default(&request.gids[0], portguid);
	/* Set IPoIB broadcast MGID as DGID */
	request.gids[1].unicast.prefix = CL_HTON64(0xff12401bffff0000ULL);
	request.gids[1].unicast.interface_id = CL_HTON64(0x00000000ffffffffULL);

	OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, EXPECTING_ERRORS_START "\n");
	status = osmtest_get_multipath_rec(p_osmt, &request, &context);
	if (status != IB_SUCCESS) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR,
			"Got error %s\n", ib_get_err_str(status));
	}
	OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, EXPECTING_ERRORS_END "\n");

	if (status == IB_SUCCESS) {
		status = IB_ERROR;
		goto Exit;
	}

	memset(&context, 0, sizeof(context));
	request.comp_mask =
	    IB_MPR_COMPMASK_SGIDCOUNT | IB_MPR_COMPMASK_DGIDCOUNT;
	request.sgid_count = 1;
	request.dgid_count = 1;
	/* Set IPoIB broadcast MGID as SGID */
	request.gids[0].unicast.prefix = CL_HTON64(0xff12401bffff0000ULL);
	request.gids[0].unicast.interface_id = CL_HTON64(0x00000000ffffffffULL);
	ib_gid_set_default(&request.gids[1], portguid);

	OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, EXPECTING_ERRORS_START "\n");
	status = osmtest_get_multipath_rec(p_osmt, &request, &context);
	if (status != IB_SUCCESS) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR,
			"Got error %s\n", ib_get_err_str(status));
	}
	OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, EXPECTING_ERRORS_END "\n");

	if (status == IB_SUCCESS) {
		status = IB_ERROR;
		goto Exit;
	}

	memset(&context, 0, sizeof(context));
	memset(&request, 0, sizeof(request));
	request.comp_mask =
	    IB_MPR_COMPMASK_SGIDCOUNT | IB_MPR_COMPMASK_DGIDCOUNT |
	    IB_MPR_COMPMASK_NUMBPATH;
	request.sgid_count = 2;
	request.dgid_count = 2;
	request.num_path = 2;
	ib_gid_set_default(&request.gids[0], portguid);
	ib_gid_set_default(&request.gids[1], portguid);
	ib_gid_set_default(&request.gids[2], portguid);
	ib_gid_set_default(&request.gids[3], portguid);
	status = osmtest_get_multipath_rec(p_osmt, &request, &context);
	if (status != IB_SUCCESS)
		goto Exit;
#endif

#ifdef VENDOR_RMPP_SUPPORT
	/* GUIDInfoRecords */
	status = osmtest_validate_all_guidinfo_recs(p_osmt);
	if (status != IB_SUCCESS)
		goto Exit;

	/* If LMC > 0, test non base LID SA PortInfoRecord request */
	status =
	    osmtest_get_local_port_lmc(p_osmt, p_osmt->local_port.lid, &lmc);
	if (status != IB_SUCCESS)
		goto Exit;

	if (lmc != 0) {
		status =
		    osmtest_get_local_port_lmc(p_osmt,
					       p_osmt->local_port.lid + 1,
					       NULL);
		if (status != IB_SUCCESS)
			goto Exit;
	}

	status = osmtest_get_local_port_lmc(p_osmt, 0xffff, NULL);
	if (status != IB_SUCCESS)
		goto Exit;

	test_lid = cl_ntoh16(p_osmt->local_port.lid);

	/* More GUIDInfo Record tests */
	memset(&context, 0, sizeof(context));
	status = osmtest_get_guidinfo_rec_by_lid(p_osmt, test_lid, &context);
	if (status != IB_SUCCESS)
		goto Exit;

	memset(&context, 0, sizeof(context));
	status = osmtest_get_guidinfo_rec_by_lid(p_osmt, 0xffff, &context);
	if (status != IB_SUCCESS)
		goto Exit;

	/* Some PKeyTable Record tests */
	sm_key = OSM_DEFAULT_SM_KEY;
	memset(&context, 0, sizeof(context));
	status =
	    osmtest_get_pkeytbl_rec_by_lid(p_osmt, test_lid, sm_key, &context);
	if (status != IB_SUCCESS)
		goto Exit;

	memset(&context, 0, sizeof(context));

	OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, EXPECTING_ERRORS_START "\n");
	status = osmtest_get_pkeytbl_rec_by_lid(p_osmt, test_lid, 0, &context);
	if (status != IB_SUCCESS) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR,
			"Got error %s\n", ib_get_err_str(status));
	}
	OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, EXPECTING_ERRORS_END "\n");

	if (status == IB_SUCCESS) {
		status = IB_ERROR;
		goto Exit;
	}

	memset(&context, 0, sizeof(context));
	status =
	    osmtest_get_pkeytbl_rec_by_lid(p_osmt, 0xffff, sm_key, &context);
	if (status != IB_SUCCESS)
		goto Exit;

	/* SwitchInfo Record tests */
	memset(&context, 0, sizeof(context));
	status = osmtest_get_sw_info_rec_by_lid(p_osmt, 0, &context);
	if (status != IB_SUCCESS)
		goto Exit;

	memset(&context, 0, sizeof(context));
	status = osmtest_get_sw_info_rec_by_lid(p_osmt, test_lid, &context);
	if (status != IB_SUCCESS)
		goto Exit;

	/* LFT Record tests */
	memset(&context, 0, sizeof(context));
	status = osmtest_get_lft_rec_by_lid(p_osmt, 0, &context);
	if (status != IB_SUCCESS)
		goto Exit;

	memset(&context, 0, sizeof(context));
	status = osmtest_get_lft_rec_by_lid(p_osmt, test_lid, &context);
	if (status != IB_SUCCESS)
		goto Exit;

	/* MFT Record tests */
	memset(&context, 0, sizeof(context));
	status = osmtest_get_mft_rec_by_lid(p_osmt, 0, &context);
	if (status != IB_SUCCESS)
		goto Exit;

	memset(&context, 0, sizeof(context));
	status = osmtest_get_mft_rec_by_lid(p_osmt, test_lid, &context);
	if (status != IB_SUCCESS)
		goto Exit;

	/* Some LinkRecord tests */
	/* FromLID */
	memset(&context, 0, sizeof(context));
	status = osmtest_get_link_rec_by_lid(p_osmt, test_lid, 0, &context);
	if (status != IB_SUCCESS)
		goto Exit;

	/* ToLID */
	memset(&context, 0, sizeof(context));
	status = osmtest_get_link_rec_by_lid(p_osmt, 0, test_lid, &context);
	if (status != IB_SUCCESS)
		goto Exit;

	/* FromLID & ToLID */
	memset(&context, 0, sizeof(context));
	status =
	    osmtest_get_link_rec_by_lid(p_osmt, test_lid, test_lid, &context);
	if (status != IB_SUCCESS)
		goto Exit;

	/* NodeRecord test */
	memset(&context, 0, sizeof(context));
	status = osmtest_get_node_rec_by_lid(p_osmt, 0xffff, &context);
	if (status != IB_SUCCESS)
		goto Exit;

	/* SMInfoRecord tests */
	memset(&sm_info_rec_opt, 0, sizeof(sm_info_rec_opt));
	memset(&context, 0, sizeof(context));
	status = osmtest_sminfo_record_request(p_osmt, IB_MAD_METHOD_SET,
					       &sm_info_rec_opt, &context);
	if (status == IB_SUCCESS) {
		status = IB_ERROR;
		goto Exit;
	} else {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "IS EXPECTED ERROR ^^^^\n");
	}

	memset(&sm_info_rec_opt, 0, sizeof(sm_info_rec_opt));
	memset(&context, 0, sizeof(context));
	status = osmtest_sminfo_record_request(p_osmt, IB_MAD_METHOD_GETTABLE,
					       &sm_info_rec_opt, &context);
	if (status != IB_SUCCESS)
		goto Exit;

	memset(&sm_info_rec_opt, 0, sizeof(sm_info_rec_opt));
	sm_info_rec_opt.lid = test_lid;	/* local LID */
	memset(&context, 0, sizeof(context));
	status = osmtest_sminfo_record_request(p_osmt, IB_MAD_METHOD_GETTABLE,
					       &sm_info_rec_opt, &context);
	if (status != IB_SUCCESS)
		goto Exit;

	if (portguid != 0) {
		memset(&sm_info_rec_opt, 0, sizeof(sm_info_rec_opt));
		sm_info_rec_opt.sm_guid = portguid;	/* local GUID */
		memset(&context, 0, sizeof(context));
		status =
		    osmtest_sminfo_record_request(p_osmt,
						  IB_MAD_METHOD_GETTABLE,
						  &sm_info_rec_opt, &context);
		if (status != IB_SUCCESS)
			goto Exit;
	}

	for (i = 1; i < 16; i++) {
		memset(&sm_info_rec_opt, 0, sizeof(sm_info_rec_opt));
		sm_info_rec_opt.priority = i;
		memset(&context, 0, sizeof(context));
		status =
		    osmtest_sminfo_record_request(p_osmt,
						  IB_MAD_METHOD_GETTABLE,
						  &sm_info_rec_opt, &context);
		if (status != IB_SUCCESS)
			goto Exit;
	}

	for (i = 1; i < 4; i++) {
		memset(&sm_info_rec_opt, 0, sizeof(sm_info_rec_opt));
		sm_info_rec_opt.sm_state = i;
		memset(&context, 0, sizeof(context));
		status =
		    osmtest_sminfo_record_request(p_osmt,
						  IB_MAD_METHOD_GETTABLE,
						  &sm_info_rec_opt, &context);
		if (status != IB_SUCCESS)
			goto Exit;
	}

	/* InformInfoRecord tests */
	OSM_LOG(&p_osmt->log, OSM_LOG_VERBOSE, "InformInfoRecord "
		"Sending a BAD - Set Unsubscribe request\n");
	memset(&inform_info_opt, 0, sizeof(inform_info_opt));
	memset(&inform_info_rec_opt, 0, sizeof(inform_info_rec_opt));
	memset(&context, 0, sizeof(context));
	status =
	    osmtest_informinfo_request(p_osmt, IB_MAD_ATTR_INFORM_INFO_RECORD,
				       IB_MAD_METHOD_SET, &inform_info_rec_opt,
				       &context);
	if (status == IB_SUCCESS) {
		status = IB_ERROR;
		goto Exit;
	} else {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "InformInfoRecord "
			"IS EXPECTED ERROR ^^^^\n");
	}

	OSM_LOG(&p_osmt->log, OSM_LOG_VERBOSE, "InformInfoRecord "
		"Sending a Good - Empty GetTable request\n");
	memset(&context, 0, sizeof(context));
	status =
	    osmtest_informinfo_request(p_osmt, IB_MAD_ATTR_INFORM_INFO_RECORD,
				       IB_MAD_METHOD_GETTABLE,
				       &inform_info_rec_opt, &context);
	if (status != IB_SUCCESS)
		goto Exit;

	/* InformInfo tests */
	OSM_LOG(&p_osmt->log, OSM_LOG_VERBOSE, "InformInfo "
		"Sending a BAD - Empty Get request "
		"(should fail with NO_RECORDS)\n");
	memset(&context, 0, sizeof(context));
	status = osmtest_informinfo_request(p_osmt, IB_MAD_ATTR_INFORM_INFO,
					    IB_MAD_METHOD_GET, &inform_info_opt,
					    &context);
	if (status == IB_SUCCESS) {
		status = IB_ERROR;
		goto Exit;
	} else {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "InformInfo "
			"IS EXPECTED ERROR ^^^^\n");
	}

	OSM_LOG(&p_osmt->log, OSM_LOG_VERBOSE, "InformInfo "
		"Sending a BAD - Set Unsubscribe request\n");
	memset(&context, 0, sizeof(context));
	status = osmtest_informinfo_request(p_osmt, IB_MAD_ATTR_INFORM_INFO,
					    IB_MAD_METHOD_SET, &inform_info_opt,
					    &context);
	if (status == IB_SUCCESS) {
		status = IB_ERROR;
		goto Exit;
	} else {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "InformInfo UnSubscribe "
			"IS EXPECTED ERROR ^^^^\n");
	}

	/* Now subscribe */
	OSM_LOG(&p_osmt->log, OSM_LOG_VERBOSE, "InformInfo "
		"Sending a Good - Set Subscribe request\n");
	inform_info_opt.subscribe = TRUE;
	memset(&context, 0, sizeof(context));
	status = osmtest_informinfo_request(p_osmt, IB_MAD_ATTR_INFORM_INFO,
					    IB_MAD_METHOD_SET, &inform_info_opt,
					    &context);
	if (status != IB_SUCCESS)
		goto Exit;

	/* Now unsubscribe (QPN needs to be 1 to work) */
	OSM_LOG(&p_osmt->log, OSM_LOG_VERBOSE, "InformInfo "
		"Sending a Good - Set Unsubscribe request\n");
	inform_info_opt.subscribe = FALSE;
	inform_info_opt.qpn = 1;
	memset(&context, 0, sizeof(context));
	status = osmtest_informinfo_request(p_osmt, IB_MAD_ATTR_INFORM_INFO,
					    IB_MAD_METHOD_SET, &inform_info_opt,
					    &context);
	if (status != IB_SUCCESS)
		goto Exit;

	/* Now subscribe again */
	OSM_LOG(&p_osmt->log, OSM_LOG_VERBOSE, "InformInfo "
		"Sending a Good - Set Subscribe request\n");
	inform_info_opt.subscribe = TRUE;
	inform_info_opt.qpn = 1;
	memset(&context, 0, sizeof(context));
	status = osmtest_informinfo_request(p_osmt, IB_MAD_ATTR_INFORM_INFO,
					    IB_MAD_METHOD_SET, &inform_info_opt,
					    &context);
	if (status != IB_SUCCESS)
		goto Exit;

	/* Subscribe over existing subscription */
	OSM_LOG(&p_osmt->log, OSM_LOG_VERBOSE, "InformInfo "
		"Sending a Good - Set Subscribe (again) request\n");
	inform_info_opt.qpn = 0;
	memset(&context, 0, sizeof(context));
	status = osmtest_informinfo_request(p_osmt, IB_MAD_ATTR_INFORM_INFO,
					    IB_MAD_METHOD_SET, &inform_info_opt,
					    &context);
	if (status != IB_SUCCESS)
		goto Exit;

	/* More InformInfoRecord tests */
	/* RID lookup (with currently invalid enum) */
	OSM_LOG(&p_osmt->log, OSM_LOG_VERBOSE, "InformInfoRecord "
		"Sending a Good - GetTable by GID\n");
	ib_gid_set_default(&inform_info_rec_opt.subscriber_gid,
			   p_osmt->local_port.port_guid);
	inform_info_rec_opt.subscriber_enum = 1;
	memset(&context, 0, sizeof(context));
	status =
	    osmtest_informinfo_request(p_osmt, IB_MAD_ATTR_INFORM_INFO_RECORD,
				       IB_MAD_METHOD_GETTABLE,
				       &inform_info_rec_opt, &context);
	if (status != IB_SUCCESS)
		goto Exit;

	/* Enum lookup */
	OSM_LOG(&p_osmt->log, OSM_LOG_VERBOSE, "InformInfoRecord "
		"Sending a Good - GetTable (subscriber_enum == 0) request\n");
	inform_info_rec_opt.subscriber_enum = 0;
	memset(&context, 0, sizeof(context));
	status =
	    osmtest_informinfo_request(p_osmt, IB_MAD_ATTR_INFORM_INFO_RECORD,
				       IB_MAD_METHOD_GETTABLE,
				       &inform_info_rec_opt, &context);
	if (status != IB_SUCCESS)
		goto Exit;

	/* Get all InformInfoRecords */
	OSM_LOG(&p_osmt->log, OSM_LOG_VERBOSE, "InformInfoRecord "
		"Sending a Good - GetTable (ALL records) request\n");
	memset(&inform_info_rec_opt, 0, sizeof(inform_info_rec_opt));
	memset(&context, 0, sizeof(context));
	status =
	    osmtest_informinfo_request(p_osmt, IB_MAD_ATTR_INFORM_INFO_RECORD,
				       IB_MAD_METHOD_GETTABLE,
				       &inform_info_rec_opt, &context);
	if (status != IB_SUCCESS)
		goto Exit;

	/* Another subscription */
	OSM_LOG(&p_osmt->log, OSM_LOG_VERBOSE, "InformInfo "
		"Sending another Good - Set Subscribe (again) request\n");
	inform_info_opt.qpn = 0;
	inform_info_opt.trap = 0x1234;
	memset(&context, 0, sizeof(context));
	status = osmtest_informinfo_request(p_osmt, IB_MAD_ATTR_INFORM_INFO,
					    IB_MAD_METHOD_SET, &inform_info_opt,
					    &context);
	if (status != IB_SUCCESS)
		goto Exit;

	/* Get all InformInfoRecords again */
	OSM_LOG(&p_osmt->log, OSM_LOG_VERBOSE, "InformInfoRecord "
		"Sending a Good - GetTable (ALL records) request\n");
	memset(&inform_info_rec_opt, 0, sizeof(inform_info_rec_opt));
	memset(&context, 0, sizeof(context));
	status =
	    osmtest_informinfo_request(p_osmt, IB_MAD_ATTR_INFORM_INFO_RECORD,
				       IB_MAD_METHOD_GETTABLE,
				       &inform_info_rec_opt, &context);
	if (status != IB_SUCCESS)
		goto Exit;

	/* Cleanup subscriptions before further testing */
	/* Does order of deletion matter ? Test this !!! */
	OSM_LOG(&p_osmt->log, OSM_LOG_VERBOSE, "InformInfo "
		"Sending a Good - Set (cleanup) request\n");
	inform_info_opt.subscribe = FALSE;
	inform_info_opt.qpn = 1;
	memset(&context, 0, sizeof(context));
	status = osmtest_informinfo_request(p_osmt, IB_MAD_ATTR_INFORM_INFO,
					    IB_MAD_METHOD_SET,
					    &inform_info_opt, &context);
	if (status != IB_SUCCESS)
		goto Exit;

	/* Get all InformInfoRecords again */
	OSM_LOG(&p_osmt->log, OSM_LOG_VERBOSE, "InformInfoRecord "
		"Sending a Good - GetTable (ALL records) request\n");
	memset(&inform_info_rec_opt, 0, sizeof(inform_info_rec_opt));
	memset(&context, 0, sizeof(context));
	status =
	    osmtest_informinfo_request(p_osmt, IB_MAD_ATTR_INFORM_INFO_RECORD,
				       IB_MAD_METHOD_GETTABLE,
				       &inform_info_rec_opt, &context);
	if (status != IB_SUCCESS)
		goto Exit;

	OSM_LOG(&p_osmt->log, OSM_LOG_VERBOSE, "InformInfo"
		"Sending a Good - Set (cleanup) request\n");
	inform_info_opt.subscribe = FALSE;
	inform_info_opt.qpn = 1;
	inform_info_opt.trap = 0;
	memset(&context, 0, sizeof(context));
	status = osmtest_informinfo_request(p_osmt, IB_MAD_ATTR_INFORM_INFO,
					    IB_MAD_METHOD_SET,
					    &inform_info_opt, &context);
	if (status != IB_SUCCESS)
		goto Exit;

	/* Get all InformInfoRecords a final time */
	OSM_LOG(&p_osmt->log, OSM_LOG_VERBOSE, "InformInfoRecord "
		"Sending a Good - GetTable (ALL records) request\n");
	memset(&inform_info_rec_opt, 0, sizeof(inform_info_rec_opt));
	memset(&context, 0, sizeof(context));
	status =
	    osmtest_informinfo_request(p_osmt, IB_MAD_ATTR_INFORM_INFO_RECORD,
				       IB_MAD_METHOD_GETTABLE,
				       &inform_info_rec_opt, &context);
	if (status != IB_SUCCESS)
		goto Exit;

	if (lmc != 0) {
		test_lid = cl_ntoh16(p_osmt->local_port.lid + 1);

		/* Another GUIDInfo Record test */
		memset(&context, 0, sizeof(context));
		status =
		    osmtest_get_guidinfo_rec_by_lid(p_osmt, test_lid, &context);
		if (status != IB_SUCCESS)
			goto Exit;

		/* Another PKeyTable Record test */
		memset(&context, 0, sizeof(context));
		status =
		    osmtest_get_pkeytbl_rec_by_lid(p_osmt, test_lid, sm_key,
						   &context);
		if (status != IB_SUCCESS)
			goto Exit;

		/* Another SwitchInfo Record test */
		memset(&context, 0, sizeof(context));
		status =
		    osmtest_get_sw_info_rec_by_lid(p_osmt, test_lid, &context);
		if (status != IB_SUCCESS)
			goto Exit;

		/* Another LFT Record test */
		memset(&context, 0, sizeof(context));
		status = osmtest_get_lft_rec_by_lid(p_osmt, test_lid, &context);
		if (status != IB_SUCCESS)
			goto Exit;

		/* Another MFT Record test */
		memset(&context, 0, sizeof(context));
		status = osmtest_get_mft_rec_by_lid(p_osmt, test_lid, &context);
		if (status != IB_SUCCESS)
			goto Exit;

		/* More LinkRecord tests */
		/* FromLID */
		memset(&context, 0, sizeof(context));
		status =
		    osmtest_get_link_rec_by_lid(p_osmt, test_lid, 0, &context);
		if (status != IB_SUCCESS)
			goto Exit;

		/* ToLID */
		memset(&context, 0, sizeof(context));
		status =
		    osmtest_get_link_rec_by_lid(p_osmt, 0, test_lid, &context);
		if (status != IB_SUCCESS)
			goto Exit;

		/* Another NodeRecord test */
		memset(&context, 0, sizeof(context));
		status =
		    osmtest_get_node_rec_by_lid(p_osmt, test_lid, &context);
		if (status != IB_SUCCESS)
			goto Exit;
	}

	/* PathRecords */
	if (!p_osmt->opt.ignore_path_records) {
		status = osmtest_validate_all_path_recs(p_osmt);
		if (status != IB_SUCCESS)
			goto Exit;

		if (lmc != 0) {
			memset(&context, 0, sizeof(context));
			status =
			    osmtest_get_path_rec_by_lid_pair(p_osmt, test_lid,
							     test_lid,
							     &context);
			if (status != IB_SUCCESS)
				goto Exit;

			memset(&context, 0, sizeof(context));
			OSM_LOG(&p_osmt->log, OSM_LOG_ERROR,
				EXPECTING_ERRORS_START "\n");
			status =
			    osmtest_get_path_rec_by_lid_pair(p_osmt, 0xffff,
							     0xffff, &context);
			if (status != IB_SUCCESS) {
				OSM_LOG(&p_osmt->log, OSM_LOG_ERROR,
					"Got error %s\n",
					ib_get_err_str(status));
			}
			OSM_LOG(&p_osmt->log, OSM_LOG_ERROR,
				EXPECTING_ERRORS_END "\n");

			if (status == IB_SUCCESS) {
				status = IB_ERROR;
				goto Exit;
			}

			OSM_LOG(&p_osmt->log, OSM_LOG_ERROR,
				EXPECTING_ERRORS_START "\n");

			status =
			    osmtest_get_path_rec_by_lid_pair(p_osmt, test_lid,
							     0xffff, &context);
			if (status != IB_SUCCESS) {
				OSM_LOG(&p_osmt->log, OSM_LOG_ERROR,
					"Got error %s\n",
					ib_get_err_str(status));
			}
			OSM_LOG(&p_osmt->log, OSM_LOG_ERROR,
				EXPECTING_ERRORS_END "\n");

			if (status == IB_SUCCESS) {
				status = IB_ERROR;
				goto Exit;
			}
		}
	}
#endif

	status = osmtest_validate_single_port_recs(p_osmt);
	if (status != IB_SUCCESS)
		goto Exit;

	if (!p_osmt->opt.ignore_path_records) {
		status = osmtest_validate_single_path_recs(p_osmt);
		if (status != IB_SUCCESS)
			goto Exit;
	}

Exit:
	OSM_LOG_EXIT(&p_osmt->log);
	return (status);
}