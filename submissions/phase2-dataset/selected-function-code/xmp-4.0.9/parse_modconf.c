static void parse_modconf(struct options *o, char *confname, unsigned char *md5)
{
	FILE *rc;
	char *hash, *var, *val, line[256];
	int active = 0;

	if ((rc = fopen(confname, "r")) == NULL)
		return;

	while (!feof(rc)) {
		memset(line, 0, 256);
		fscanf(rc, "%255[^\n]", line);
		fgetc(rc);

		/* Delete comments */
		if ((hash = strchr(line, '#')))
			*hash = 0;

		if (line[0] == '[') {
			if (compare_md5(md5, line + 1) == 0) {
				active = 1;
			} else {
				active = 0;
			}
			continue;
		}

		if (!active)
			continue;

		delete_spaces(line);

		if (!(var = strtok(line, "=\n")))
			continue;

		val = strtok(NULL, " \t\n");

		getval_yn("8bit", o->format, XMP_FORMAT_8BIT);
		getval_yn("mono", o->format, XMP_FORMAT_MONO);
		getval_yn("filter", o->dsp, XMP_DSP_LOWPASS);
		getval_yn("loop", o->loop, XMP_DSP_LOWPASS);
		getval_yn("reverse", o->reverse, 1);
		getval_no("amplify", o->amplify);
		getval_no("mix", o->mix);

		getval_tristate("fixloop", o->fixloop);
		getval_tristate("fx9bug", o->fx9bug);
		getval_tristate("vblank", o->vblank);

		if (!strcmp(var, "interpolation")) {
			if (!strcmp(val, "nearest")) {
				o->interp = XMP_INTERP_NEAREST;
			} else if (!strcmp(val, "linear")) {
				o->interp = XMP_INTERP_LINEAR;
			} else if (!strcmp(val, "spline")) {
				o->interp = XMP_INTERP_SPLINE;
			} else {
				fprintf(stderr, "%s: unknown interpolation "
					"type \"%s\"\n", confname, val);
			}
		}
	}

	fclose(rc);
}