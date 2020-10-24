ib_api_status_t osmt_run_service_records_flow(IN osmtest_t * const p_osmt)
{
	ib_service_record_t srv_rec;
	ib_api_status_t status;
	uint8_t instance, i;
	uint8_t service_data8[16], service_key[16];
	ib_net16_t service_data16[8];
	ib_net32_t service_data32[4];
	ib_net64_t service_data64[2];
	uint64_t pid = getpid();
	uint64_t id[7];
	/* We use up to seven service names - we use the extra for bad flow */
	ib_svc_name_t service_name[7];
#ifdef VENDOR_RMPP_SUPPORT
	/* This array contain only the valid names after registering vs SM */
	ib_svc_name_t service_valid_names[3];
	uint32_t num_recs = 0;
#endif

	OSM_LOG_ENTER(&p_osmt->log);

	/* Init Service names */
	for (i = 0; i < 7; i++) {
#ifdef __WIN__
		uint64_t rand_val = rand() - (uint64_t) i;
#else
		uint64_t rand_val = random() - (uint64_t) i;
#endif
		id[i] = abs((int)(pid - rand_val));
		/* Just to be unique any place on any host */
		sprintf((char *)(service_name[i]),
			"osmt.srvc.%" PRIu64 ".%" PRIu64, rand_val, pid);
		/*printf("-I- Service Name is : %s, ID is : 0x%" PRIx64 "\n",service_name[i],id[i]); */
	}

	status = osmt_register_service(p_osmt, cl_ntoh64(id[0]),	/*  IN ib_net64_t      service_id, */
				       IB_DEFAULT_PKEY,	/*  IN ib_net16_t      service_pkey, */
				       0xFFFFFFFF,	/*  IN ib_net32_t      service_lease, */
				       11,	/*  IN uint8_t         service_key_lsb, */
				       (char *)service_name[0]	/*  IN char            *service_name */
	    );
	if (status != IB_SUCCESS) {
		goto Exit;
	}

	status = osmt_register_service(p_osmt, cl_ntoh64(id[1]),	/*  IN ib_net64_t      service_id, */
				       IB_DEFAULT_PKEY,	/*  IN ib_net16_t      service_pkey, */
				       cl_hton32(0x00000004),	/*  IN ib_net32_t     service_lease, */
				       11,	/*  IN uint8_t         service_key_lsb, */
				       (char *)service_name[1]	/*  IN char            *service_name */
	    );
	if (status != IB_SUCCESS) {
		goto Exit;
	}

	status = osmt_register_service(p_osmt, cl_ntoh64(id[2]),	/*  IN ib_net64_t      service_id, */
				       0,	/*  IN ib_net16_t      service_pkey, */
				       0xFFFFFFFF,	/*  IN ib_net32_t      service_lease, */
				       11,	/* Remove Service Record IN uint8_t service_key_lsb, */
				       (char *)service_name[2]	/*  IN char            *service_name */
	    );

	if (status != IB_SUCCESS) {
		goto Exit;
	}

	/*  Generate 2 instances of service record with consecutive data */
	for (instance = 0; instance < 2; instance++) {
		/*  First, clear all arrays */
		memset(service_data8, 0, 16 * sizeof(uint8_t));
		memset(service_data16, 0, 8 * sizeof(uint16_t));
		memset(service_data32, 0, 4 * sizeof(uint32_t));
		memset(service_data64, 0, 2 * sizeof(uint64_t));
		service_data8[instance] = instance + 1;
		service_data16[instance] = cl_hton16(instance + 2);
		service_data32[instance] = cl_hton32(instance + 3);
		service_data64[instance] = cl_hton64(instance + 4);
		status = osmt_register_service_with_data(p_osmt, cl_ntoh64(id[3]),	/*  IN ib_net64_t      service_id, */
							 IB_DEFAULT_PKEY,	/*  IN ib_net16_t      service_pkey, */
							 cl_ntoh32(10),	/*  IN ib_net32_t      service_lease, */
							 12,	/*  IN uint8_t         service_key_lsb, */
							 service_data8, service_data16, service_data32, service_data64,	/* service data structures */
							 (char *)service_name[3]	/*  IN char            *service_name */
		    );

		if (status != IB_SUCCESS) {
			goto Exit;
		}

	}

	/*  Trying to create service with zero key */
	memset(service_key, 0, 16 * sizeof(uint8_t));
	status = osmt_register_service_with_full_key(p_osmt, cl_ntoh64(id[5]),	/*  IN ib_net64_t      service_id, */
						     0,	/*  IN ib_net16_t      service_pkey, */
						     0xFFFFFFFF,	/*  IN ib_net32_t      service_lease, */
						     service_key,	/*  full service_key, */
						     (char *)service_name[5]	/*  IN char            *service_name */
	    );

	if (status != IB_SUCCESS) {
		goto Exit;
	}

	/*  Now update it with Unique key and different service name */
	for (i = 0; i <= 15; i++) {
		service_key[i] = i + 1;
	}
	status = osmt_register_service_with_full_key(p_osmt, cl_ntoh64(id[5]),	/*  IN ib_net64_t      service_id, */
						     0,	/*  IN ib_net16_t      service_pkey, */
						     0xFFFFFFFF,	/*  IN ib_net32_t      service_lease, */
						     service_key,	/* full service_key, */
						     (char *)service_name[6]	/*  IN char            *service_name */
	    );
	if (status != IB_SUCCESS) {
		goto Exit;
	}

	/* Let OpenSM handle it */
	usleep(100);

	/* Make sure service_name[0] exists */
	status = osmt_get_service_by_name(p_osmt,
					  (char *)service_name[0], 1, &srv_rec);
	if (status != IB_SUCCESS) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 4A1A: "
			"Fail to find service: name: %s\n",
			(char *)service_name[0]);
		status = IB_ERROR;
		goto Exit;
	}

	/* Make sure service_name[1] exists */
	status = osmt_get_service_by_name(p_osmt,
					  (char *)service_name[1], 1, &srv_rec);
	if (status != IB_SUCCESS) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 4A1B: "
			"Fail to find service: name: %s\n",
			(char *)service_name[1]);
		status = IB_ERROR;
		goto Exit;
	}

	/* Make sure service_name[2] exists */
	status = osmt_get_service_by_name(p_osmt,
					  (char *)service_name[2], 1, &srv_rec);
	if (status != IB_SUCCESS) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 4A1C: "
			"Fail to find service: name: %s\n",
			(char *)service_name[2]);
		status = IB_ERROR;
		goto Exit;
	}

	/* Make sure service_name[3] exists. */
	/* After 10 seconds the service should not exist: service_lease = 10 */
	status = osmt_get_service_by_name(p_osmt,
					  (char *)service_name[3], 1, &srv_rec);
	if (status != IB_SUCCESS) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 4A1D: "
			"Fail to find service: name: %s\n",
			(char *)service_name[3]);
		status = IB_ERROR;
		goto Exit;
	}

	sleep(10);

	status = osmt_get_service_by_name(p_osmt,
					  (char *)service_name[3], 0, &srv_rec);
	if (status != IB_SUCCESS) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 4A1E: "
			"Found service: name: %s that should have been "
			"deleted due to service lease expiring\n",
			(char *)service_name[3]);
		status = IB_ERROR;
		goto Exit;
	}

	/*  Check that for service: id[5] only one record exists */
	status = osmt_get_service_by_id(p_osmt, 1, cl_ntoh64(id[5]), &srv_rec);
	if (status != IB_SUCCESS) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 4A1F: "
			"Found number of records != 1 for "
			"service: id: 0x%016" PRIx64 "\n", id[5]);
		status = IB_ERROR;
		goto Exit;
	}

	/*  Bad Flow of Get with invalid Service ID: id[6] */
	status = osmt_get_service_by_id(p_osmt, 0, cl_ntoh64(id[6]), &srv_rec);
	if (status != IB_SUCCESS) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 4A20: "
			"Found service: id: 0x%016" PRIx64 " "
			"that is invalid\n", id[6]);
		status = IB_ERROR;
		goto Exit;
	}

	/*  Check by both id and service name: id[0], service_name[0] */
	status = osmt_get_service_by_id_and_name(p_osmt, 1, cl_ntoh64(id[0]),
						 (char *)service_name[0],
						 &srv_rec);
	if (status != IB_SUCCESS) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 4A21: "
			"Fail to find service: id: 0x%016" PRIx64 " "
			"name: %s\n", id[0], (char *)service_name[0]);
		status = IB_ERROR;
		goto Exit;
	}

	/*  Check by both id and service name: id[5], service_name[6] */
	status = osmt_get_service_by_id_and_name(p_osmt, 1, cl_ntoh64(id[5]),
						 (char *)service_name[6],
						 &srv_rec);
	if (status != IB_SUCCESS) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 4A22: "
			"Fail to find service: id: 0x%016" PRIx64 " "
			"name: %s\n", id[5], (char *)service_name[6]);
		status = IB_ERROR;
		goto Exit;
	}

	/* Bad Flow of Get with invalid name(service_name[3]) and valid ID(id[0]) */
	status = osmt_get_service_by_id_and_name(p_osmt, 0, cl_ntoh64(id[0]),
						 (char *)service_name[3],
						 &srv_rec);
	if (status != IB_SUCCESS) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 4A23: "
			"Found service: id: 0x%016" PRIx64
			"name: %s which is an invalid service\n",
			id[0], (char *)service_name[3]);
		status = IB_ERROR;
		goto Exit;
	}

	/*  Bad Flow of Get with unmatched name(service_name[5]) and id(id[3]) (both valid) */
	status = osmt_get_service_by_id_and_name(p_osmt, 0, cl_ntoh64(id[3]),
						 (char *)service_name[5],
						 &srv_rec);
	if (status != IB_SUCCESS) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 4A24: "
			"Found service: id: 0x%016" PRIx64
			"name: %s which is an invalid service\n",
			id[3], (char *)service_name[5]);
		status = IB_ERROR;
		goto Exit;
	}

	/* Bad Flow of Get with service name that doesn't exist (service_name[4]) */
	status = osmt_get_service_by_name(p_osmt,
					  (char *)service_name[4], 0, &srv_rec);
	if (status != IB_SUCCESS) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 4A25: "
			"Found service: name: %s that shouldn't exist\n",
			(char *)service_name[4]);
		status = IB_ERROR;
		goto Exit;
	}

	/*  Bad Flow : Check that getting service_name[5] brings no records since another service
	   has been updated with the same ID (service_name[6] */
	status = osmt_get_service_by_name(p_osmt,
					  (char *)service_name[5], 0, &srv_rec);
	if (status != IB_SUCCESS) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 4A26: "
			"Found service: name: %s which is an "
			"invalid service\n", (char *)service_name[5]);
		status = IB_ERROR;
		goto Exit;
	}

	/*  Check that getting service_name[6] by name ONLY is valid,
	   since we do not support key&name association, also trusted queries */
	status = osmt_get_service_by_name(p_osmt,
					  (char *)service_name[6], 1, &srv_rec);
	if (status != IB_SUCCESS) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 4A27: "
			"Fail to find service: name: %s\n",
			(char *)service_name[6]);
		status = IB_ERROR;
		goto Exit;
	}

	/*  Test Service Key */
	memset(service_key, 0, 16 * sizeof(uint8_t));

	/* Check for service_name[5] with service_key=0 - the service shouldn't
	   exist with this name. */
	status = osmt_get_service_by_name_and_key(p_osmt,
						  (char *)service_name[5],
						  0, service_key, &srv_rec);
	if (status != IB_SUCCESS) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 4A28: "
			"Found service: name: %s key:0 which is an "
			"invalid service (wrong name)\n",
			(char *)service_name[5]);
		status = IB_ERROR;
		goto Exit;
	}

	/* Check for service_name[6] with service_key=0 - the service should
	   exist with different key. */
	status = osmt_get_service_by_name_and_key(p_osmt,
						  (char *)service_name[6],
						  0, service_key, &srv_rec);
	if (status != IB_SUCCESS) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 4A29: "
			"Found service: name: %s key: 0 which is an "
			"invalid service (wrong service_key)\n",
			(char *)service_name[6]);
		status = IB_ERROR;
		goto Exit;
	}

	/* check for service_name[6] with the correct service_key */
	for (i = 0; i <= 15; i++)
		service_key[i] = i + 1;
	status = osmt_get_service_by_name_and_key(p_osmt,
						  (char *)service_name[6],
						  1, service_key, &srv_rec);
	if (status != IB_SUCCESS) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 4A2A: "
			"Fail to find service: name: %s with "
			"correct service key\n", (char *)service_name[6]);
		status = IB_ERROR;
		goto Exit;
	}
#ifdef VENDOR_RMPP_SUPPORT
	/* These ar the only service_names which are valid */
	memcpy(&service_valid_names[0], &service_name[0], sizeof(uint8_t) * 64);
	memcpy(&service_valid_names[1], &service_name[2], sizeof(uint8_t) * 64);
	memcpy(&service_valid_names[2], &service_name[6], sizeof(uint8_t) * 64);

	status =
	    osmt_get_all_services_and_check_names(p_osmt, service_valid_names,
						  3, &num_recs);
	if (status != IB_SUCCESS) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 4A2B: "
			"Fail to find all services that should exist\n");
		status = IB_ERROR;
		goto Exit;
	}
#endif

	/* Delete service_name[0] */
	status = osmt_delete_service_by_name(p_osmt, 1,
					     (char *)service_name[0], 1);
	if (status != IB_SUCCESS) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 4A2C: "
			"Fail to delete service: name: %s\n",
			(char *)service_name[0]);
		status = IB_ERROR;
		goto Exit;
	}

	/* Make sure deletion of service_name[0] succeeded */
	status = osmt_get_service_by_name(p_osmt,
					  (char *)service_name[0], 0, &srv_rec);
	if (status != IB_SUCCESS) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 4A2D: "
			"Found service: name: %s that was deleted\n",
			(char *)service_name[0]);
		status = IB_ERROR;
		goto Exit;
	}

	/* Make sure service_name[1] doesn't exist (expired service lease) */
	status = osmt_get_service_by_name(p_osmt,
					  (char *)service_name[1], 0, &srv_rec);
	if (status != IB_SUCCESS) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 4A2E: "
			"Found service: name: %s that should have expired\n",
			(char *)service_name[1]);
		status = IB_ERROR;
		goto Exit;
	}

	/* Make sure service_name[2] exists */
	status = osmt_get_service_by_name(p_osmt,
					  (char *)service_name[2], 1, &srv_rec);
	if (status != IB_SUCCESS) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 4A2F: "
			"Fail to find service: name: %s\n",
			(char *)service_name[2]);
		status = IB_ERROR;
		goto Exit;
	}

	/*  Bad Flow - try to delete non-existent service_name[5] */
	status = osmt_delete_service_by_name(p_osmt, 0,
					     (char *)service_name[5], 0);
	if (status != IB_SUCCESS) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 4A30: "
			"Succeed to delete non-existent service: name: %s\n",
			(char *)service_name[5]);
		status = IB_ERROR;
		goto Exit;
	}

	/* Delete service_name[2] */
	status = osmt_delete_service_by_name(p_osmt, 1,
					     (char *)service_name[2], 1);
	if (status != IB_SUCCESS) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 4A31: "
			"Fail to delete service: name: %s\n",
			(char *)service_name[2]);
		status = IB_ERROR;
		goto Exit;
	}

	/* Delete service_name[6] */
	status = osmt_delete_service_by_name(p_osmt, 1,
					     (char *)service_name[6], 1);
	if (status != IB_SUCCESS) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 4A32: "
			"Failed to delete service name: %s\n",
			(char *)service_name[6]);
		status = IB_ERROR;
		goto Exit;
	}

Exit:
	OSM_LOG_EXIT(&p_osmt->log);
	return status;
}