int main(int argc, char **argv)
{
	xmp_context xc;
	struct xmp_module_info mi;
	struct xmp_frame_info fi;
	struct options opt, save_opt;
	struct control control;
	int i;
	int first;
	int skipprev;
	FILE *f = NULL;
	int val, lf_flag;
	int flags;
	int played;
#ifndef WIN32
	struct timeval tv;
	struct timezone tz;

	gettimeofday(&tv, &tz);
	srand(tv.tv_usec);
#else
	srand(GetTickCount());
#endif

	init_sound_drivers();

	memset(&opt, 0, sizeof (struct options));
	memset(&control, 0, sizeof (struct control));

	/* set defaults */
	opt.verbose = 1;
	opt.rate = 44100;
	opt.mix = -1;
	opt.defpan = 50;
	opt.driver_id = NULL;
	opt.interp = XMP_INTERP_SPLINE;
	opt.dsp = XMP_DSP_LOWPASS;

	/* read configuration file */
	if (!opt.norc) {
		read_config(&opt);
	}

	get_options(argc, argv, &opt);

	if (!opt.probeonly && optind >= argc) {
		fprintf(stderr, "%s: no modules to play\n"
			"Use `%s --help' for more information.\n",
			argv[0], argv[0]);
		exit(EXIT_FAILURE);
	}

	if (opt.interp < 0) {
		fprintf(stderr, "%s: unknown interpolation type\n"
			"Use `%s --help' for more information.\n",
			argv[0], argv[0]);
		exit(EXIT_FAILURE);
	}

	if (opt.silent) {
		opt.driver_id = "null";
	}

	sound = select_sound_driver(&opt);

	if (sound == NULL) {
		fprintf(stderr, "%s: can't initialize sound", argv[0]);
		if (opt.driver_id != NULL) {
			fprintf(stderr, " (driver = %s)", opt.driver_id);
		}
		fprintf(stderr, "\n");

		if (f != NULL) {
			fclose(f);
		}
		exit(EXIT_FAILURE);
	}

	if (opt.verbose > 0) {
		report("Extended Module Player " VERSION "\n"
			"Copyright (C) 1996-2014 Claudio Matsuoka and Hipolito Carraro Jr\n");

		report("Using %s\n", sound->description);

		report("Mixer set to %d Hz, %dbit, %s%s%s\n", opt.rate,
		    opt.format & XMP_FORMAT_8BIT ? 8 : 16,
		    opt.interp == XMP_INTERP_LINEAR ? "linear interpolated " :
		    opt.interp == XMP_INTERP_SPLINE ? "cubic spline interpolated " : "",
		    opt.format & XMP_FORMAT_MONO ? "mono" : "stereo",
		    opt.dsp & XMP_DSP_LOWPASS ? "" : " (no filter)");
	}

	if (opt.probeonly) {
		exit(EXIT_SUCCESS);
	}

#ifdef HAVE_SIGNAL_H
	signal(SIGTERM, cleanup);
	signal(SIGINT, cleanup);
	signal(SIGFPE, cleanup);
	signal(SIGSEGV, cleanup);
#ifdef SIGQUIT
	signal(SIGQUIT, cleanup);
#endif
#ifdef SIGTSTP
	signal(SIGCONT, sigcont_handler);
	signal(SIGTSTP, sigtstp_handler);
	sigcont_handler(0);
#endif
#endif

	xc = xmp_create_context();
	if (xc == NULL) {
		perror(argv[0]);
		exit(EXIT_FAILURE);
	}

	skipprev = 0;

	if (opt.ins_path) {
		xmp_set_instrument_path(xc, opt.ins_path);
	}

	lf_flag = 0;
	memcpy(&save_opt, &opt, sizeof (struct options));

	control.loop = opt.loop;
	control.explore = opt.explore;
	first = optind;

    play_all:
	if (opt.random) {
		shuffle(argc - optind + 1, &argv[optind - 1]);
	}

	for (played = 0; optind < argc; optind++) {
		memcpy(&opt, &save_opt, sizeof (struct options));

		if (opt.verbose > 0) {
			if (lf_flag)
				report("\n");
			lf_flag = 1;
			report("Loading %s (%d of %d)\n",
				argv[optind], optind - first + 1, argc - first);
		}

		xmp_set_player(xc, XMP_PLAYER_DEFPAN, opt.defpan);

		/* load module */

		val = xmp_load_module(xc, argv[optind]);

		if (val < 0) {
			load_error(argv[0], argv[optind], val);

			if (skipprev) {
		        	optind -= 2;
				if (optind < (first - 1)) {
					optind += 2;
					skipprev = 0;
				}
			}
			continue;
		}

		xmp_get_module_info(xc, &mi);
		if (!opt.norc) {
			read_modconf(&opt, mi.md5);
		}

		/* check sequence */

		if (opt.sequence) {
			if (opt.sequence < mi.num_sequences) {
				if (mi.seq_data[opt.sequence].duration > 0) {
					opt.start = mi.seq_data[opt.sequence].entry_point;
				}
			} else {
				fprintf(stderr, "%s: sequence %d doesn't exist in this module\n",
					argv[0], opt.sequence);
				if (skipprev) {
					optind -= 2;
					if (optind < first) {
						optind += 2;
					}
				}
				continue;
			}
		}
		control.sequence = opt.sequence;

		skipprev = 0;
		control.time = 0.0;

		/* Play module */

		if (xmp_start_player(xc, opt.rate, opt.format) == 0) {
			xmp_set_player(xc, XMP_PLAYER_INTERP, opt.interp);
			xmp_set_player(xc, XMP_PLAYER_DSP, opt.dsp);

			played = 1;

			xmp_set_player(xc, XMP_PLAYER_AMP, opt.amplify);

			if (opt.mix >= 0) {
				xmp_set_player(xc, XMP_PLAYER_MIX, opt.mix);
			}

			if (opt.reverse) {
				int mix;
				mix = xmp_get_player(xc, XMP_PLAYER_MIX);
				xmp_set_player(xc, XMP_PLAYER_MIX, -mix);
			}

			xmp_set_position(xc, opt.start);

			/* Mute channels */

			for (i = 0; i < XMP_MAX_CHANNELS; i++) {
				xmp_channel_mute(xc, i, opt.mute[i]);
			}

			/* Set player flags */

#define set_flag(x,y,z) do { \
  if ((y) > 0) (x) |= (z); \
  else if ((y) < 0) (x) &= ~ (z); \
} while (0)

			flags = xmp_get_player(xc, XMP_PLAYER_CFLAGS);
			set_flag(flags, opt.vblank, XMP_FLAGS_VBLANK);
			set_flag(flags, opt.fx9bug, XMP_FLAGS_FX9BUG);
			set_flag(flags, opt.fixloop, XMP_FLAGS_FIXLOOP);
			xmp_set_player(xc, XMP_PLAYER_CFLAGS, flags);

			/* Show module data */

			if (opt.verbose > 0) {
				info_mod(&mi);
			}

			if (opt.verbose > 1) {
				info_instruments(&mi);
			}

			if (opt.show_comment) {
				if (mi.comment != NULL) {
					report("\n");
					info_comment(&mi);
				}
			}
	
			/* Play sequence */

			refresh_status = 1;
			info_frame_init();
			fi.loop_count = 0;
    play_sequence:
			while (!opt.info && xmp_play_frame(xc) == 0) {
				int old_loop = fi.loop_count;
				
				xmp_get_frame_info(xc, &fi);

				/* Check loop */

				if (old_loop != fi.loop_count) {
					if (control.loop == 1) {
						info_message("Loop sequence %d", control.sequence);
					} else {
						break;
					}
				}

#ifdef SIGTSTP
				sigcont_handler(0);
#else
				foreground_in = foreground_out = 1;
#endif
				if (foreground_out && opt.verbose > 0) {
					info_frame(&mi, &fi, &control, refresh_status);
					refresh_status = 0;
				}

				control.time += 1.0 * fi.frame_time / 1000;

				sound->play(fi.buffer, fi.buffer_size);

				if (foreground_in && !opt.nocmd) {
					read_command(xc, &mi, &control);

					if (control.display) {
						show_info(control.display, &mi);
						control.display = 0;
						refresh_status = 1;
					}

					if (control.cur_seq) {
						info_message("Current sequence: %d (start at position %02X)", control.sequence, mi.seq_data[control.sequence].entry_point);
						control.cur_seq = 0;
					}
				}

				/* Check maximum replay time */

				if (opt.max_time > 0 &&
				    control.time > opt.max_time) {
					break;
				}

				check_pause(xc, &control, &mi, &fi,
							opt.verbose);

				opt.start = 0;
			}

			/* Subsong explorer */

			if (control.explore && control.skip == 0) {
				control.sequence++;
				if (control.sequence < mi.num_sequences) {
					xmp_set_position(xc, mi.seq_data
						[control.sequence].entry_point);
					info_message("Playing sequence %d",
						control.sequence);
					goto play_sequence;
				}
			}

			xmp_end_player(xc);
		}

		xmp_release_module(xc);

		if (!opt.info) {
			report("\n");
		}

		if (control.skip == -1) {
			optind -= optind > first ? 2 : 1;
			skipprev = 1;
		} else if (control.skip == -2) {
			goto end;
		}
		control.skip = 0;
	}

	if (control.loop == 2 && played) {
		info_message("Restart module list");
		optind = first;
		goto play_all;
	}

	sound->flush();

    end:
	xmp_free_context(xc);

	if (foreground_in)
		reset_tty();

	sound->deinit();

	exit(EXIT_SUCCESS);
}