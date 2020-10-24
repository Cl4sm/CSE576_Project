FILE *mpeg_demux_open (mpeg_demux_t *mpeg, unsigned sid, unsigned ssid)
{
	FILE     *fp;
	char     *name;
	unsigned seq;

	if (par_demux_name == NULL) {
		fp = (FILE *) mpeg->ext;
	}
	else {
		seq = (sid == 0xbd) ? ((sid << 8) + ssid) : sid;

		name = mpeg_get_name (par_demux_name, seq);

		fp = fopen (name, "wb");
		if (fp == NULL) {
			prt_err ("can't open stream file (%s)\n", name);

			if (sid == 0xbd) {
				par_substream[ssid] &= ~PAR_STREAM_SELECT;
			}
			else {
				par_stream[sid] &= ~PAR_STREAM_SELECT;
			}

			free (name);

			return (NULL);
		}

		free (name);
	}

	if ((sid == 0xbd) && par_dvdsub) {
		if (fwrite ("SPU ", 1, 4, fp) != 4) {
			fclose (fp);
			return (NULL);
		}
	}

	return (fp);
}
