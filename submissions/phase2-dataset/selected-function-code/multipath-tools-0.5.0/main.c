{
	int fd, c, res;
	const char *device_name = NULL;
	int num_prin_sa = 0;
	int num_prout_sa = 0;
	int num_prout_param = 0;
	int prin_flag = 0;
	int prout_flag = 0;
	int ret = 0;
	int hex = 0;
	uint64_t param_sark = 0;
	unsigned int prout_type = 0;
	int param_alltgpt = 0;
	int param_aptpl = 0;
	uint64_t param_rk = 0;
	unsigned int param_rtp = 0;
	int num_transportids = 0;
	struct transportid transportids[MPATH_MX_TIDS];
	int prout = 1;
	int prin = 1;
	int prin_sa = -1;
	int prout_sa = -1;
	int verbose = 0;
	int loglevel = 0;
	int noisy = 0;
	int num_transport =0;
	void *resp = NULL;
	struct transportid * tmp;
	struct udev *udev = NULL;

	if (optind == argc)
	{

		fprintf (stderr, "No parameter used\n");
		usage ();
		exit (1);
	}

	if (getuid () != 0)
	{
		fprintf (stderr, "need to be root\n");
		exit (1);
	}

	udev = udev_new();
	mpath_lib_init(udev);
	memset(transportids,0,MPATH_MX_TIDS);

	while (1)
	{
		int option_index = 0;

		c = getopt_long (argc, argv, "v:Cd:hHioZK:S:PAT:skrGILcRX:l:",
				long_options, &option_index);
		if (c == -1)
			break;

		switch (c)
		{
			case 'v':
				if (1 != sscanf (optarg, "%d", &loglevel))
				{
					fprintf (stderr, "bad argument to '--verbose'\n");
					return MPATH_PR_SYNTAX_ERROR;
				}
				break;

			case 'C':
				prout_sa = MPATH_PROUT_CLEAR_SA;
				++num_prout_sa;
				break;

			case 'd':
				device_name = optarg;
				break;

			case 'h':
				usage ();
				return 0;

			case 'H':
				hex=1;
				break;

			case 'i':
				prin_flag = 1;
				break;

			case 'o':
				prout_flag = 1;
				break;

			case 'Z':
				param_aptpl = 1;
				++num_prout_param;
				break;
			case 'K':
				if (1 != sscanf (optarg, "%" SCNx64 "", &param_rk))
				{
					fprintf (stderr, "bad argument to '--param-rk'\n");
					return MPATH_PR_SYNTAX_ERROR;
				}
				++num_prout_param;
				break;

			case 'S':
				if (1 != sscanf (optarg, "%" SCNx64 "", &param_sark))
				{
					fprintf (stderr, "bad argument to '--param-sark'\n");
					return MPATH_PR_SYNTAX_ERROR;
				}
				++num_prout_param;
				break;

			case 'P':
				prout_sa = MPATH_PROUT_PREE_SA;
				++num_prout_sa;
				break;

			case 'A':
				prout_sa = MPATH_PROUT_PREE_AB_SA;
				++num_prout_sa;
				break;

			case 'T':
				if (1 != sscanf (optarg, "%x", &prout_type))
				{
					fprintf (stderr, "bad argument to '--prout-type'\n");
					return MPATH_PR_SYNTAX_ERROR;
				}
				++num_prout_param;
				break;

			case 's':
				prin_sa = MPATH_PRIN_RFSTAT_SA;
				++num_prin_sa;
				break;

			case 'k':
				prin_sa = MPATH_PRIN_RKEY_SA;
				++num_prin_sa;
				break;

			case 'r':
				prin_sa = MPATH_PRIN_RRES_SA;
				++num_prin_sa;
				break;

			case 'G':
				prout_sa = MPATH_PROUT_REG_SA;
				++num_prout_sa;
				break;

			case 'I':
				prout_sa = MPATH_PROUT_REG_IGN_SA;
				++num_prout_sa;
				break;

			case 'L':
				prout_sa = MPATH_PROUT_REL_SA;
				++num_prout_sa;
				break;

			case 'c':
				prin_sa = MPATH_PRIN_RCAP_SA;
				++num_prin_sa;
				break;

			case 'R':
				prout_sa = MPATH_PROUT_RES_SA;
				++num_prout_sa;
				break;

			case 'X':
				if (0 != construct_transportid(optarg, transportids, num_transport)) {
					fprintf(stderr, "bad argument to '--transport-id'\n");
					return MPATH_PR_SYNTAX_ERROR;
				}

				++num_transport;
				break;

			case 'l':
                                if (1 != sscanf(optarg, "%u", &mpath_mx_alloc_len)) {
                                        fprintf(stderr, "bad argument to '--alloc-length'\n");
                                        return MPATH_PR_SYNTAX_ERROR;
                                } else if (MPATH_MAX_PARAM_LEN < mpath_mx_alloc_len) {
                                        fprintf(stderr, "'--alloc-length' argument exceeds maximum"
                                                        " limit(%d)\n", MPATH_MAX_PARAM_LEN);
                                        return MPATH_PR_SYNTAX_ERROR;
                                }
                                break;

			default:
				fprintf(stderr, "unrecognised switch " "code 0x%x ??\n", c);	
				usage ();
				ret = MPATH_PR_SYNTAX_ERROR;
				goto out;
		}
	}

	if (optind < argc)
	{

		if (NULL == device_name)
		{
			device_name = argv[optind];
			++optind;
		}
		if (optind < argc)
		{
			for (; optind < argc; ++optind)
				fprintf (stderr, "Unexpected extra argument: %s\n", argv[optind]);
			usage ();
			ret = MPATH_PR_SYNTAX_ERROR;
			goto out;
		}
	}

	/* set verbosity */
	noisy = (loglevel >= 3) ? 1 : hex;
	verbose	= (loglevel >= 3)? 3: loglevel;

	if ((prout_flag + prin_flag) == 0)
	{
		fprintf (stderr, "choose either '--in' or '--out' \n");
		usage ();
		ret = MPATH_PR_SYNTAX_ERROR;
		goto out;
	}
	if ((prout_flag + prin_flag) > 1)
	{
		fprintf (stderr, "choose either '--in' or '--out' \n");
		usage ();
		ret = MPATH_PR_SYNTAX_ERROR;
		goto out;
	}
	else if (prout_flag)
	{				/* syntax check on PROUT arguments */
		prin = 0;
		if ((1 != num_prout_sa) || (0 != num_prin_sa))
		{
			fprintf (stderr, " For Persistent Reserve Out only one "
					"appropriate\n service action must be "
					"chosen \n");
			ret = MPATH_PR_SYNTAX_ERROR;
			goto out;
		}
	}
	else if (prin_flag)
	{				/* syntax check on PRIN arguments */
		prout = 0;
		if (num_prout_sa > 0)
		{
			fprintf (stderr, " When a service action for Persistent "
					"Reserve Out is chosen the\n"
					" '--out' option must be given \n");
			ret = MPATH_PR_SYNTAX_ERROR;
			goto out;
		}
		if (0 == num_prin_sa)
		{
			fprintf (stderr,
					" No service action given for Persistent Reserve IN\n");
			usage();
			ret = MPATH_PR_SYNTAX_ERROR;
		}
		else if (num_prin_sa > 1)
		{
			fprintf (stderr, " Too many service actions given; choose "
					"one only\n");
			usage();
			ret = MPATH_PR_SYNTAX_ERROR;
		}
	}
	else
	{
		usage ();
		ret = MPATH_PR_SYNTAX_ERROR;
		goto out;
	}

	if ((param_rtp) && (MPATH_PROUT_REG_MOV_SA != prout_sa))
	{
		fprintf (stderr, " --relative-target-port"
				" only useful with --register-move\n");
		usage ();
		ret = MPATH_PR_SYNTAX_ERROR;
		goto out;
	}

	if (((MPATH_PROUT_RES_SA == prout_sa) ||
				(MPATH_PROUT_REL_SA == prout_sa) ||
				(MPATH_PROUT_PREE_SA == prout_sa) ||
				(MPATH_PROUT_PREE_AB_SA == prout_sa)) &&
			(0 == prout_type)) {
		fprintf(stderr, "Warning: --prout-type probably needs to be "
				"given\n");
	}
	if ((verbose > 2) && num_transportids)
	{
		fprintf (stderr, "number of tranport-ids decoded from "
				"command line : %d\n", num_transportids);
	}

	if (device_name == NULL)
	{
		fprintf (stderr, "No device name given \n");
		usage ();
		ret = MPATH_PR_SYNTAX_ERROR;
		goto out;
	}

	/* open device */
	if ((fd = open (device_name, O_WRONLY)) < 0)
	{
		fprintf (stderr, "%s: error opening file (rw) fd=%d\n",
				device_name, fd);
		ret = MPATH_PR_FILE_ERROR;
		goto out;
	}


	if (prin)
	{
		resp = mpath_alloc_prin_response(prin_sa);
		if (!resp)
		{
			fprintf (stderr, "failed to allocate PRIN response buffer\n");
			ret = MPATH_PR_OTHER;
			goto out;
		}

		ret = mpath_persistent_reserve_in (fd, prin_sa, resp, noisy, verbose);
		if (ret != MPATH_PR_SUCCESS )
		{
			fprintf (stderr, "Persistent Reserve IN command failed\n");
			goto out;	
		}

		switch(prin_sa)
		{			
			case MPATH_PRIN_RKEY_SA: 
				mpath_print_buf_readkeys(resp);		
				break;
			case MPATH_PRIN_RRES_SA: 
				mpath_print_buf_readresv(resp);
				break;
			case MPATH_PRIN_RCAP_SA:
				mpath_print_buf_readcap(resp);		
				break;
			case MPATH_PRIN_RFSTAT_SA:
				mpath_print_buf_readfullstat(resp);		
				break;
		}
		free(resp);
	}
	else if (prout)
	{
		int j; 
		struct prout_param_descriptor *paramp;

		paramp= malloc(sizeof(struct prout_param_descriptor) + (sizeof(struct transportid *)*(MPATH_MX_TIDS )));
		
		memset(paramp, 0, sizeof(struct prout_param_descriptor) + (sizeof(struct transportid *)*(MPATH_MX_TIDS)));

		for (j = 7; j >= 0; --j) {
			paramp->key[j] = (param_rk & 0xff);
			param_rk >>= 8;
		}

		for (j = 7; j >= 0; --j) {
			paramp->sa_key[j] = (param_sark & 0xff);
			param_sark >>= 8;
		}

		if (param_alltgpt)
			paramp->sa_flags |= 0x4;
		if (param_aptpl)
			paramp->sa_flags |= 0x1;

		if (num_transport) 
		{
			paramp->sa_flags |= MPATH_F_SPEC_I_PT_MASK;
			paramp->num_transportid = num_transport;
			for (j = 0 ; j < num_transport; j++)
			{
				paramp->trnptid_list[j] = (struct transportid *)malloc(sizeof(struct transportid));
				memcpy(paramp->trnptid_list[j], &transportids[j],sizeof(struct transportid));
			}
		}

		/* PROUT commands other than 'register and move' */
		ret = mpath_persistent_reserve_out (fd, prout_sa, 0, prout_type,
				paramp, noisy, verbose);
		for (j = 0 ; j < num_transport; j++)
		{
			tmp = paramp->trnptid_list[j];
			free(tmp);
		}
		free(paramp);
	}

	if (ret != MPATH_PR_SUCCESS)
	{
		switch(ret)
		{
			case MPATH_PR_SENSE_UNIT_ATTENTION:
				printf("persistent reserve out: scsi status: Unit Attention\n");
				break;
			case MPATH_PR_RESERV_CONFLICT:
				printf("persistent reserve out: scsi status: Reservation Conflict\n");
				break;
		}
		printf("PR out: command failed\n");
	}

	res = close (fd);
	if (res < 0)
	{
		mpath_lib_exit();
		udev_unref(udev);
		return MPATH_PR_FILE_ERROR;
	}

out :
	mpath_lib_exit();
	udev_unref(udev);
	return (ret >= 0) ? ret : MPATH_PR_OTHER;
}
