void get_options(int argc, char **argv, struct options *options)
{
	int optidx = 0;
	int o;

#define OPTIONS "a:b:CcD:d:Ff:hI:i:LlM:mNo:P:p:qRrS:s:T:t:uVvZz:"
	while ((o = getopt_long(argc, argv, OPTIONS, lopt, &optidx)) != -1) {
		switch (o) {
		case 'a':
			options->amplify = atoi(optarg);
			break;
		case 'b':
			if (atoi(optarg) == 8) {
				options->format |= XMP_FORMAT_8BIT;
			}
			break;
		case 'C':
			options->show_comment = 1;
			break;
		case 'c':
			options->driver_id = "file";
			options->out_file = "-";
			break;
		case 'D':
			if (options->dparm < MAX_DRV_PARM)
				options->driver_parm[options->dparm++] = optarg;
			break;
		case 'd':
			options->driver_id = optarg;
			break;
		case 'F':
			options->dsp &= ~XMP_DSP_LOWPASS;
			break;
		case 'f':
			options->rate = strtoul(optarg, NULL, 0);
			break;
		case OPT_FIXLOOP:
			options->fixloop = 1;
			break;
		case 'I':
			options->ins_path = optarg;
			break;
		case 'i':
			if (!strcmp(optarg, "nearest")) {
				options->interp = XMP_INTERP_NEAREST;
			} else if (!strcmp(optarg, "linear")) {
				options->interp = XMP_INTERP_LINEAR;
			} else if (!strcmp(optarg, "spline")) {
				options->interp = XMP_INTERP_SPLINE;
			} else {
				options->interp = -1;
			}
			break;
		case OPT_LOADONLY:
			options->info = 1;
			options->silent = 1;
			break;
		case 'L': {
			char **list;
			int i;
			list = xmp_get_format_list();
			for (i = 0; list[i] != NULL; i++) {
				printf("%d:%s\n", i + 1, list[i]);
			}
			exit(EXIT_SUCCESS);
			break; }
		case 'l':
			options->loop = 1;
			break;
		case OPT_LOOPALL:
			options->loop = 2;
			break;
		case 'm':
			options->format |= XMP_FORMAT_MONO;
			break;
		case 'N':
			options->silent = 1;
			break;
		case OPT_NOCMD:
			options->nocmd = 1;
			break;
		case OPT_NORC:
			options->norc = 1;
			break;
		case 'o':
			options->out_file = optarg;
			if (strlen(optarg) >= 4 &&
			    !strcasecmp(optarg + strlen(optarg) - 4, ".wav")) {
				options->driver_id = "wav";
			} else {
				options->driver_id = "file";
			}
			break;
		case OPT_FX9BUG:
			options->fx9bug = 1;
			break;
		case 'P':
			options->mix = strtoul(optarg, NULL, 0);
			if (options->mix < 0)
				options->mix = 0;
			if (options->mix > 100)
				options->mix = 100;
			break;
		case 'p':
			options->defpan = strtoul(optarg, NULL, 0);
			if (options->defpan < 0)
				options->defpan = 0;
			if (options->defpan > 100)
				options->defpan = 100;
			break;
		case OPT_PROBEONLY:
			options->probeonly = 1;
			break;
		case 'q':
			options->verbose = 0;
			break;
		case 'R':
			options->random = 1;
			break;
		case 'r':
			options->reverse = 1;
			break;
		case 'M':
		case 'S':
			if (o == 'S') {
				memset(options->mute, 1, XMP_MAX_CHANNELS);
			}
			token = strtok(optarg, ",");
			while (token) {
				int a, b;
				char buf[40];
				memset(buf, 0, 40);
				if (strchr(token, '-')) {
					b = strcspn(token, "-");
					strncpy(buf, token, b);
					a = atoi(buf);
					strncpy(buf, token + b + 1,
						strlen(token) - b - 1);
					b = atoi(buf);
				} else {
					a = b = atoi(token);
				}
				for (; b >= a; b--) {
					if (b < XMP_MAX_CHANNELS)
						options->mute[b] = (o == 'M');
				}
				token = strtok(NULL, ",");
			}
			break;
		case 's':
			options->start = strtoul(optarg, NULL, 0);
			break;
		case 't':
			options->max_time = strtoul(optarg, NULL, 0) * 1000;
			break;
		case 'u':
			options->format |= XMP_FORMAT_UNSIGNED;
			break;
		case OPT_VBLANK:
			options->vblank = 1;
			break;
		case 'V':
			puts("Extended Module Player " VERSION);
			exit(0);
		case 'v':
			options->verbose++;
			break;
		case 'Z':
			options->explore = 1;
			break;
		case 'z':
			options->sequence = strtoul(optarg, NULL, 0);
			break;
		case 'h':
			usage(argv[0], options);
		default:
			exit(-1);
		}
	}

	/* Set limits */
	if (options->rate < 1000)
		options->rate = 1000;	/* Min. rate 1 kHz */
	if (options->rate > 48000)
		options->rate = 48000;	/* Max. rate 48 kHz */
}