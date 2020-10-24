int parse_init(int argc, const char *const *argv, void **context)
{
	struct parse_context *ctxt;
	char buf[MAX_ERR_BUF];
	char *noptstr, *def, *val, *macros, *gbl_options;
	const char *xopt;
	int optlen, len, offset;
	int i, bval;
	unsigned int append_options;

	/* Get processor information for predefined escapes */
	macro_lock();
	if (!macro_init_done) {
		macro_init_done = 1;
		macro_init();
	}
	macro_unlock();

	/* Set up context and escape chain */

	if (!(ctxt = (struct parse_context *) malloc(sizeof(struct parse_context)))) {
		char *estr = strerror_r(errno, buf, MAX_ERR_BUF);
		logerr(MODPREFIX "malloc: %s", estr);
		*context = NULL;
		return 1;
	}
	*context = (void *) ctxt;

	*ctxt = default_context;
	optlen = 0;

	/* Look for options and capture, and create new defines if we need to */

	for (i = 0; i < argc; i++) {
		if (argv[i][0] == '-' &&
		   (argv[i][1] == 'D' || argv[i][1] == '-') ) {
			switch (argv[i][1]) {
			case 'D':
				if (argv[i][2])
					def = strdup(argv[i] + 2);
				else if (++i < argc)
					def = strdup(argv[i]);
				else
					break;

				if (!def) {
					char *estr = strerror_r(errno, buf, MAX_ERR_BUF);
					logerr(MODPREFIX "strdup: %s", estr);
					break;
				}

				val = strchr(def, '=');
				if (val)
					*(val++) = '\0';
				else
					val = "";

				macro_lock();

				ctxt->subst = macro_addvar(ctxt->subst,
							def, strlen(def), val);

				macro_unlock();

				/* we use 5 for the "-D", "=", "," and the null */
				if (ctxt->macros) {
					len = strlen(ctxt->macros) + strlen(def) + strlen(val);
					macros = realloc(ctxt->macros, len + 5);
					if (!macros) {
						free(def);
						break;
					}
					strcat(macros, ",");
				} else { /* No comma, so only +4 */
					len = strlen(def) + strlen(val);
					macros = malloc(len + 4);
					if (!macros) {
						free(def);
						break;
					}
					*macros = '\0';
				}
				ctxt->macros = macros;

				strcat(ctxt->macros, "-D");
				strcat(ctxt->macros, def);
				strcat(ctxt->macros, "=");
				strcat(ctxt->macros, val);
				free(def);
				break;

			case '-':
				if (!strncmp(argv[i] + 2, "no-", 3)) {
					xopt = argv[i] + 5;
					bval = 0;
				} else {
					xopt = argv[i] + 2;
					bval = 1;
				}

				if (!strmcmp(xopt, "slashify-colons", 1))
					ctxt->slashify_colons = bval;
				else
					error(LOGOPT_ANY,
					      MODPREFIX "unknown option: %s",
					      argv[i]);
				break;

			default:
				error(LOGOPT_ANY,
				      MODPREFIX "unknown option: %s", argv[i]);
				break;
			}
		} else {
			offset = (argv[i][0] == '-' ? 1 : 0);
			len = strlen(argv[i] + offset);
			if (ctxt->optstr) {
				noptstr =
				    (char *) realloc(ctxt->optstr, optlen + len + 2);
				if (noptstr) {
					noptstr[optlen] = ',';
					strcpy(noptstr + optlen + 1, argv[i] + offset);
					optlen += len + 1;
				}
			} else {
				noptstr = (char *) malloc(len + 1);
				if (noptstr) {
					strcpy(noptstr, argv[i] + offset);
					optlen = len;
				}
			}
			if (!noptstr) {
				char *estr = strerror_r(errno, buf, MAX_ERR_BUF);
				kill_context(ctxt);
				logerr(MODPREFIX "%s", estr);
				*context = NULL;
				return 1;
			}
			ctxt->optstr = noptstr;
		}
	}

	gbl_options = NULL;
	if (global_options) {
		if (ctxt->optstr && strstr(ctxt->optstr, global_options))
			goto options_done;
		gbl_options = strdup(global_options);
	}

	if (gbl_options) {
		append_options = defaults_get_append_options();
		if (append_options) {
			char *tmp = concat_options(gbl_options, ctxt->optstr);
			if (!tmp) {
				char *estr = strerror_r(errno, buf, MAX_ERR_BUF);
				logerr(MODPREFIX "concat_options: %s", estr);
				free(gbl_options);
			} else
				ctxt->optstr = tmp;
		} else {
			if (!ctxt->optstr)
				ctxt->optstr = gbl_options;
			else
				free(gbl_options);
		}
	}
options_done:
	debug(LOGOPT_NONE,
	      MODPREFIX "init gathered global options: %s", ctxt->optstr);

	/* We only need this once.  NFS mounts are so common that we cache
	   this module. */
	instance_mutex_lock();
	if (mount_nfs)
		init_ctr++;
	else {
		if ((mount_nfs = open_mount("nfs", MODPREFIX))) {
			init_ctr++;
		} else {
			kill_context(ctxt);
			*context = NULL;
			instance_mutex_unlock();
			return 1;
		}
	}
	instance_mutex_unlock();
	return 0;
}