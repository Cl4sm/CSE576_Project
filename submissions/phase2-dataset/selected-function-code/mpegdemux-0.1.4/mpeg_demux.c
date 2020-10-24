int mpeg_demux (FILE *inp, FILE *out)
{
	unsigned     i;
	int          r;
	mpeg_demux_t *mpeg;

	for (i = 0; i < 512; i++) {
		fp[i] = NULL;
	}

	mpeg = mpegd_open_fp (NULL, inp, 0);
	if (mpeg == NULL) {
		return (1);
	}

	mpeg->mpeg_system_header = &mpeg_demux_system_header;
	mpeg->mpeg_pack = &mpeg_demux_pack;
	mpeg->mpeg_packet = &mpeg_demux_packet;
	mpeg->mpeg_packet_check = &mpeg_packet_check;
	mpeg->mpeg_end = &mpeg_demux_end;

	mpeg->ext = out;

	r = mpegd_parse (mpeg);

	mpegd_close (mpeg);

	for (i = 0; i < 512; i++) {
		if ((fp[i] != NULL) && (fp[i] != out)) {
			fclose (fp[i]);
		}
	}

	return (r);
}
