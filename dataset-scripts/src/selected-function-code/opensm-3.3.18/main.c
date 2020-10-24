int main(int argc, char *argv[])
{
	static osmtest_t osm_test;
	osmtest_opt_t opt = { 0 };
	ib_net64_t guid = 0;
	uint16_t max_lid = 100;
	ib_api_status_t status;
	uint32_t log_flags = OSM_LOG_ERROR | OSM_LOG_INFO;
	int32_t vendor_debug = 0;
	char flow_name[64];
	uint32_t next_option;
	const char *const short_option = "f:l:m:M:d:g:s:t:i:pcvGVh";

	/*
	 * In the array below, the 2nd parameter specified the number
	 * of arguments as follows:
	 * 0: no arguments
	 * 1: argument
	 * 2: optional
	 */
	const struct option long_option[] = {
		{"create", 0, NULL, 'c'},
		{"debug", 1, NULL, 'd'},
		{"flow", 1, NULL, 'f'},
		{"wait", 1, NULL, 'w'},
		{"inventory", 1, NULL, 'i'},
		{"max_lid", 1, NULL, 'm'},
		{"guid", 1, NULL, 'g'},
		{"port", 0, NULL, 'p'},
		{"help", 0, NULL, 'h'},
		{"stress", 1, NULL, 's'},
		{"grh", 0, NULL, 'G'},
		{"Multicast_Mode", 1, NULL, 'M'},
		{"timeout", 1, NULL, 't'},
		{"verbose", 0, NULL, 'v'},
		{"log_file", 1, NULL, 'l'},
		{"vf", 1, NULL, 'x'},
		{"V", 0, NULL, 'V'},

		{NULL, 0, NULL, 0}	/* Required at end of array */
	};

	/* Make sure that the opensm, complib and osmtest were compiled using
	   same modes (debug/free) */
	if (osm_is_debug() != cl_is_debug() || osm_is_debug() != osmt_is_debug()
	    || osmt_is_debug() != cl_is_debug()) {
		fprintf(stderr,
			"-E- OpenSM, Complib and OsmTest were compiled using different modes\n");
		fprintf(stderr,
			"-E- OpenSM debug:%d Complib debug:%d OsmTest debug:%d \n",
			osm_is_debug(), cl_is_debug(), osmt_is_debug());
		exit(1);
	}

	opt.transaction_timeout = OSMT_DEFAULT_TRANS_TIMEOUT_MILLISEC;
	opt.wait_time = OSMT_DEFAULT_TRAP_WAIT_TIMEOUT_SEC;
	opt.retry_count = OSMT_DEFAULT_RETRY_COUNT;
	opt.force_log_flush = FALSE;
	opt.stress = 0;
	opt.log_file = NULL;
	opt.create = FALSE;
	opt.mmode = 1;
	opt.ignore_path_records = FALSE;	/*  Do path Records too */
	opt.full_world_path_recs = FALSE;
	opt.flow = OSMT_FLOW_ALL;	/*  run all validation tests */
	opt.with_grh = FALSE;
	strcpy(flow_name, "All Validations");
	strcpy(opt.file_name, "osmtest.dat");

	printf("\nCommand Line Arguments\n");
	do {
		next_option = getopt_long_only(argc, argv, short_option,
					       long_option, NULL);
		switch (next_option) {
		case 'c':
			/*
			 * Create the inventory file.
			 */
			opt.create = TRUE;
			printf("\tCreating inventory file\n");
			break;

		case 'i':
			/*
			 * Specifies inventory file name.
			 */
			if (strlen(optarg) > OSMTEST_FILE_PATH_MAX)
				printf
				    ("\nError: path name too long (ignored)\n");
			else
				strcpy(opt.file_name, optarg);

			printf("\tFile = %s\n", opt.file_name);
			break;

		case 'f':
			/*
			 * Specifies Flow.
			 */
			if (strlen(optarg) > OSMTEST_FILE_PATH_MAX)
				printf
				    ("\nError: path name too long (ignored)\n");
			else
				strcpy(flow_name, optarg);

			if (!strcmp("c", optarg)) {
				strcpy(flow_name, "Create Inventory");
				opt.flow = OSMT_FLOW_CREATE_INVENTORY;
			} else if (!strcmp("v", optarg)) {
				strcpy(flow_name, "Validate Inventory");
				opt.flow = OSMT_FLOW_VALIDATE_INVENTORY;
			} else if (!strcmp("s", optarg)) {
				strcpy(flow_name, "Services Registration");
				opt.flow = OSMT_FLOW_SERVICE_REGISTRATION;
			} else if (!strcmp("e", optarg)) {
				strcpy(flow_name, "Event Forwarding");
				opt.flow = OSMT_FLOW_EVENT_FORWARDING;
			} else if (!strcmp("f", optarg)) {
				strcpy(flow_name, "Stress SA");
				opt.flow = OSMT_FLOW_STRESS_SA;
			} else if (!strcmp("m", optarg)) {
				strcpy(flow_name, "Multicast");
				opt.flow = OSMT_FLOW_MULTICAST;
			} else if (!strcmp("q", optarg)) {
				strcpy(flow_name, "QoS: VLArb and SLtoVL");
				opt.flow = OSMT_FLOW_QOS;
			} else if (!strcmp("t", optarg)) {
				strcpy(flow_name, "Trap 64/65");
				opt.flow = OSMT_FLOW_TRAP;
			} else if (!strcmp("a", optarg)) {
				strcpy(flow_name, "All Validations");
				opt.flow = OSMT_FLOW_ALL;
			} else {
				printf("\nError: unknown flow %s\n", flow_name);
				exit(2);
			}
			break;

		case 'w':
			/*
			 * Specifies trap 64/65 wait time
			 */
			CL_ASSERT(strtol(optarg, NULL, 0) < 0x100);
			opt.wait_time = (uint8_t) strtol(optarg, NULL, 0);
			printf("\tTrap 64/65 wait time = %d\n", opt.wait_time);
			break;

		case 'm':
			/*
			 * Specifies the max LID to search for during exploration.
			 */
			max_lid = (uint16_t) atoi(optarg);
			printf("\tMAX-LID %u\n", max_lid);
			break;

		case 'g':
			/*
			 * Specifies port guid with which to bind.
			 */
			guid = cl_hton64(strtoull(optarg, NULL, 16));
			printf(" Guid <0x%" PRIx64 ">\n", cl_hton64(guid));
			break;

		case 'p':
			/*
			 * Display current port guids
			 */
			guid = INVALID_GUID;
			break;

		case 't':
			/*
			 * Specifies transaction timeout.
			 */
			opt.transaction_timeout = strtol(optarg, NULL, 0);
			printf("\tTransaction timeout = %d\n",
			       opt.transaction_timeout);
			break;

		case 'l':
			opt.log_file = optarg;
			printf("\tLog File:%s\n", opt.log_file);
			break;

		case 'v':
			/*
			 * Increases log verbosity.
			 */
			log_flags = (log_flags << 1) | 1;
			printf("\tVerbose option -v (log flags = 0x%X)\n",
			       log_flags);
			break;

		case 'V':
			/*
			 * Specifies maximum log verbosity.
			 */
			log_flags = 0xFFFFFFFF;
			opt.force_log_flush = TRUE;
			printf("\tEnabling maximum log verbosity\n");
			break;

		case 's':
			/*
			 * Perform stress test.
			 */
			opt.stress = strtol(optarg, NULL, 0);
			printf("\tStress test enabled: ");
			switch (opt.stress) {
			case 1:
				printf("Small SA queries\n");
				break;
			case 2:
				printf("Large SA queries\n");
				break;
			case 3:
				printf("Large Path Record SA queries\n");
				break;
			case 4:
				printf("SA Get Path Record queries\n");
				break;
			default:
				printf("Unknown value %u (ignored)\n",
				       opt.stress);
				opt.stress = 0;
				break;
			}
			break;

		case 'M':
			/*
			 * Perform multicast test.
			 */
			opt.mmode = strtol(optarg, NULL, 0);
			printf("\tMulticast test enabled: ");
			switch (opt.mmode) {
			case 1:
				printf
				    ("Short MC Flow - single mode (default)\n");
				break;
			case 2:
				printf("Short MC Flow - multiple mode\n");
				break;
			case 3:
				printf("Long MC Flow - single mode\n");
				break;
			case 4:
				printf("Long MC Flow - multiple mode\n");
				break;
			default:
				printf("Unknown value %u (ignored)\n",
				       opt.stress);
				opt.mmode = 0;
				break;
			}
			break;

		case 'd':
			/*
			 * Debug Options
			 */
			printf("\tDebug Option: ");
			switch (strtol(optarg, NULL, 0)) {
			case 1:
				printf("Ignore Path Records\n");
				opt.ignore_path_records = TRUE;
				break;
			case 2:
				printf("Force Log Flush\n");
				opt.force_log_flush = TRUE;
				break;
			case 4:
				printf("Use Full World Path Record Queries\n");
				opt.full_world_path_recs = TRUE;
				break;
			case 3:
				/* Used to be memory tracking */
			default:
				printf("Unknown value %ld (ignored)\n",
				       strtol(optarg, NULL, 0));
				break;
			}
			break;

		case 'h':
			show_usage();
			return 0;

		case 'x':
			log_flags = strtol(optarg, NULL, 0);
			printf("\tVerbose option -vf (log flags = 0x%X)\n",
			       log_flags);
			break;
		case 'G':
			opt.with_grh = TRUE;
			break;
		case -1:
			printf("Done with args\n");
			break;

		default:	/* something wrong */
			abort();
		}

	}
	while (next_option != -1);

	printf("\tFlow = %s\n", flow_name);

	if (vendor_debug)
		osm_vendor_set_debug(osm_test.p_vendor, vendor_debug);

	complib_init();

	status = osmtest_init(&osm_test, &opt, (osm_log_level_t) log_flags);
	if (status != IB_SUCCESS) {
		printf("\nError from osmtest_init: %s\n",
		       ib_get_err_str(status));
		goto Exit;
	}
	if (cl_hton64(guid) == cl_hton64(INVALID_GUID)) {
		print_all_guids(&osm_test);
		complib_exit();
		return (status);
	}

	/*
	   If the user didn't specify a GUID on the command line,
	   then get a port GUID value with which to bind.
	 */
	if (guid == 0 && !(guid = get_port_guid(&osm_test, guid))) {
		printf("\nError: port guid 0x%" PRIx64 " not found\n", guid);
		goto Exit;
	}

	/*
	 * Guid may be zero going into this function if the user
	 * hasn't specified a binding port on the command line.
	 */
	status = osmtest_bind(&osm_test, max_lid, guid);
	if (status != IB_SUCCESS)
		exit(status);

	status = osmtest_run(&osm_test);
	if (status != IB_SUCCESS) {
		printf("OSMTEST: TEST \"%s\" FAIL\n", flow_name);
	} else {
		printf("OSMTEST: TEST \"%s\" PASS\n", flow_name);
	}
	osmtest_destroy(&osm_test);

	complib_exit();

Exit:
	return (status);
}