int main (int argc, char **argv)
{
	unsigned i;
	int      r;
	unsigned id1, id2;
	char     **optarg;

	for (i = 0; i < 256; i++) {
		par_stream[i] = 0;
		par_substream[i] = 0;
		par_stream_map[i] = i;
		par_substream_map[i] = i;
	}

	while (1) {
		r = mpegd_getopt (argc, argv, &optarg, opts);

		if (r == GETOPT_DONE) {
			break;
		}

		if (r < 0) {
			return (1);
		}

		switch (r) {
		case '?':
			print_help();
			return (0);

		case 'a':
			par_dvdac3 = 1;
			break;

		case 'b':
			if (par_demux_name != NULL) {
				free (par_demux_name);
			}
			par_demux_name = str_clone (optarg[0]);
			break;

		case 'c':
			par_mode = PAR_MODE_SCAN;

			for (i = 0; i < 256; i++) {
				par_stream[i] |= PAR_STREAM_SELECT;
				par_substream[i] |= PAR_STREAM_SELECT;
			}
			break;

		case 'd':
			par_mode = PAR_MODE_DEMUX;
			break;

		case 'D':
			par_drop = 0;
			break;

		case 'e':
			par_no_end = 1;
			break;

		case 'E':
			par_empty_pack = 1;
			break;

		case 'F':
			par_first_pts = 1;
			break;

		case 'h':
			par_no_shdr = 1;
			break;

		case 'i':
			if (strcmp (optarg[0], "-") == 0) {
				for (i = 0; i < 256; i++) {
					if (par_stream[i] & PAR_STREAM_SELECT) {
						par_stream[i] &= ~PAR_STREAM_INVALID;
					}
					else {
						par_stream[i] |= PAR_STREAM_INVALID;
					}
				}
			}
			else {
				if (str_get_streams (optarg[0], par_stream, PAR_STREAM_INVALID)) {
					prt_err ("%s: bad stream id (%s)\n", argv[0], optarg[0]);
					return (1);
				}
			}
			break;

		case 'k':
			par_no_pack = 1;
			break;

		case 'K':
			par_remux_skipped = 1;
			break;

		case 'l':
			par_mode = PAR_MODE_LIST;
			break;

		case 'm':
			par_packet_max = (unsigned) strtoul (optarg[0], NULL, 0);
			break;

		case 'p':
			if (str_get_streams (optarg[0], par_substream, PAR_STREAM_SELECT)) {
				prt_err ("%s: bad substream id (%s)\n", argv[0], optarg[0]);
				return (1);
			}
			break;

		case 'P':
			id1 = (unsigned) strtoul (optarg[0], NULL, 0);
			id2 = (unsigned) strtoul (optarg[1], NULL, 0);
			par_substream_map[id1 & 0xff] = id2 & 0xff;
			break;

		case 'r':
			par_mode = PAR_MODE_REMUX;
			break;

		case 's':
			if (str_get_streams (optarg[0], par_stream, PAR_STREAM_SELECT)) {
				prt_err ("%s: bad stream id (%s)\n", argv[0], optarg[0]);
				return (1);
			}
			break;

		case 'S':
			id1 = (unsigned) strtoul (optarg[0], NULL, 0);
			id2 = (unsigned) strtoul (optarg[1], NULL, 0);
			par_stream_map[id1 & 0xff] = id2 & 0xff;
			break;

		case 't':
			par_no_packet = 1;
			break;

		case 'u':
			par_dvdsub = 1;
			break;

		case 'V':
			print_version();
			return (0);

		case 'x':
			par_split = 1;
			break;

		case 0:
			if (par_inp == NULL) {
				if (strcmp (optarg[0], "-") == 0) {
					par_inp = stdin;
				}
				else {
					par_inp = fopen (optarg[0], "rb");
				}
				if (par_inp == NULL) {
					prt_err (
						"%s: can't open input file (%s)\n",
						argv[0], optarg[0]
					);
					return (1);
				}
			}
			else if (par_out == NULL) {
				if (strcmp (optarg[0], "-") == 0) {
					par_out = stdout;
				}
				else {
					par_out = fopen (optarg[0], "wb");
				}
				if (par_out == NULL) {
					prt_err (
						"%s: can't open output file (%s)\n",
						argv[0], optarg[0]
					);
					return (1);
				}
			}
			else {
				prt_err ("%s: too many files (%s)\n",
					argv[0], optarg[0]
				);
				return (1);
			}
			break;

		default:
			return (1);
		}
	}

	if (par_inp == NULL) {
		par_inp = stdin;
	}

	if (par_out == NULL) {
		par_out = stdout;
	}

	switch (par_mode) {
	case PAR_MODE_SCAN:
		r = mpeg_scan (par_inp, par_out);
		break;

	case PAR_MODE_LIST:
		r = mpeg_list (par_inp, par_out);
		break;

	case PAR_MODE_REMUX:
		r = mpeg_remux (par_inp, par_out);
		break;

	case PAR_MODE_DEMUX:
		r = mpeg_demux (par_inp, par_out);
		break;

	default:
		r = 1;
		break;
	}

	if (r) {
		return (1);
	}

	return (0);
}
