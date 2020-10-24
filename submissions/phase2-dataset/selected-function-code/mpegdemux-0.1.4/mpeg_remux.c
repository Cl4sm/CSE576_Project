{
	int          r;
	mpeg_demux_t *mpeg;

	mpeg = mpegd_open_fp (NULL, inp, 0);
	if (mpeg == NULL) {
		return (1);
	}

	if (par_split) {
		mpeg->ext = NULL;
		sequence = 0;

		if (mpeg_remux_next_fp (mpeg)) {
			return (1);
		}
	}
	else {
		mpeg->ext = out;
	}

	mpeg->mpeg_skip = mpeg_remux_skip;
	mpeg->mpeg_system_header = mpeg_remux_system_header;
	mpeg->mpeg_pack = mpeg_remux_pack;
	mpeg->mpeg_packet = mpeg_remux_packet;
	mpeg->mpeg_packet_check = mpeg_packet_check;
	mpeg->mpeg_end = mpeg_remux_end;

	mpeg_buf_init (&shdr);
	mpeg_buf_init (&pack);
	mpeg_buf_init (&packet);

	r = mpegd_parse (mpeg);

	if (par_no_end) {
		unsigned char buf[4];

		buf[0] = (MPEG_END_CODE >> 24) & 0xff;
		buf[1] = (MPEG_END_CODE >> 16) & 0xff;
		buf[2] = (MPEG_END_CODE >> 8) & 0xff;
		buf[3] = MPEG_END_CODE & 0xff;

		if (fwrite (buf, 1, 4, (FILE *) mpeg->ext) != 4) {
			r = 1;
		}
	}

	if (par_split) {
		fclose ((FILE *) mpeg->ext);
		mpeg->ext = NULL;
	}

	mpegd_close (mpeg);

	mpeg_buf_free (&shdr);
	mpeg_buf_free (&pack);
	mpeg_buf_free (&packet);

	return (r);
}
