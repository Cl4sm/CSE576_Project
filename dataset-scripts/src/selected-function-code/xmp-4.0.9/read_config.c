int read_config(struct options *o)
{
	FILE *rc;
	char myrc[PATH_MAX];
	char *hash, *var, *val, line[256];
	char cparm[512];

#if defined __EMX__
	char *home = getenv("HOME");

	snprintf(myrc, PATH_MAX, "%s\\.xmp\\xmp.conf", home);

	if ((rc = fopen(myrc, "r")) == NULL) {
		if ((rc = fopen("xmp.conf", "r")) == NULL) {
			return -1;
		}
	}
#elif defined __AMIGA__
	strncpy(myrc, "PROGDIR:xmp.conf", PATH_MAX);

	if ((rc = fopen(myrc, "r")) == NULL)
		return -1;
#elif defined WIN32
	const char *home = getenv("USERPROFILE");
	if (!home) home = "C:";

	snprintf(myrc, PATH_MAX, "%s/xmp.conf", home);

	if ((rc = fopen(myrc, "r")) == NULL)
		return -1;
#else
	char *home = getenv("HOME");

	snprintf(myrc, PATH_MAX, "%s/.xmp/xmp.conf", home);

	if ((rc = fopen(myrc, "r")) == NULL) {
		strncpy(myrc, SYSCONFDIR "/xmp.conf", PATH_MAX);
		if ((rc = fopen(myrc, "r")) == NULL) {
			return -1;
		}
	}
#endif

	while (!feof(rc)) {
		memset(line, 0, 256);
		fscanf(rc, "%255[^\n]", line);
		fgetc(rc);

		/* Delete comments */
		if ((hash = strchr(line, '#')))
			*hash = 0;

		delete_spaces(line);

		if (!(var = strtok(line, "=\n")))
			continue;

		val = strtok(NULL, " \t\n");

#define getval_yn(x,w,y) { \
	if (!strcmp(var,x)) { if (get_yesno (val)) w |= (y); \
	    else w &= ~(y); continue; } }

#define getval_tristate(x,w) { \
	if (!strcmp(var,x)) { if (get_yesno (val)) w = 1; \
	    else w = -1; continue; } }

#define getval_no(x,y) { \
	if (!strcmp(var,x)) { y = atoi (val); continue; } }

		getval_yn("8bit", o->format, XMP_FORMAT_8BIT);
		getval_yn("mono", o->format, XMP_FORMAT_MONO);
		getval_yn("filter", o->dsp, XMP_DSP_LOWPASS);
		getval_yn("loop", o->loop, 1);
		getval_yn("reverse", o->reverse, 1);
		getval_no("amplify", o->amplify);
		getval_no("mix", o->mix);
		getval_no("default_pan", o->defpan);
		/*getval_no("chorus", o->chorus);
		getval_no("reverb", o->reverb);*/
		getval_no("srate", o->rate);
		/*getval_no("time", o->time);
		getval_no("verbosity", o->verbosity);*/

		if (!strcmp(var, "driver")) {
			strncpy(driver, val, 31);
			o->driver_id = driver;
			continue;
		}

		if (!strcmp(var, "interpolation")) {
			if (!strcmp(val, "nearest")) {
				o->interp = XMP_INTERP_NEAREST;
			} else if (!strcmp(val, "linear")) {
				o->interp = XMP_INTERP_LINEAR;
			} else if (!strcmp(val, "spline")) {
				o->interp = XMP_INTERP_SPLINE;
			} else {
				fprintf(stderr, "%s: unknown interpolation "
						"type \"%s\"\n", myrc, val);
			}
		}

		if (!strcmp(var, "bits")) {
			if (atoi(val) == 8)
				o->format = XMP_FORMAT_8BIT;;
			continue;
		}

		if (!strcmp(var, "instrument_path")) {
			strncpy(instrument_path, val, 256);
			o->ins_path = instrument_path;
			continue;
		}

		/* If the line doesn't match any of the previous parameters,
		 * send it to the device driver
		 */
		if (o->dparm < MAX_DRV_PARM) {
			snprintf(cparm, 512, "%s=%s", var, val);
			o->driver_parm[o->dparm++] = strdup(cparm);
		}
	}

	fclose(rc);

	return 0;
}