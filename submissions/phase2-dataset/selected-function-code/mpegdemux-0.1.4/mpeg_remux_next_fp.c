static
int mpeg_remux_next_fp (mpeg_demux_t *mpeg)
{
	char *fname;
	FILE *fp;

	fp = (FILE *) mpeg->ext;
	if (fp != NULL) {
		fclose (fp);
		mpeg->ext = NULL;
	}

	fname = mpeg_get_name (par_demux_name, sequence);
	if (fname == NULL) {
		return (1);
	}

	sequence += 1;

	fp = fopen (fname, "wb");

	free (fname);

	if (fp == NULL) {
		return (1);
	}

	mpeg->ext = fp;

	return (0);
}
