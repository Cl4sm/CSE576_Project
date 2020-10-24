{
	int          r;
	unsigned     i;
	mpeg_demux_t *mpeg;

	for (i = 0; i < 256; i++) {
		pts1[i] = ~(unsigned long long) 0;
		pts2[i] = ~(unsigned long long) 0;
	}

	mpeg = mpegd_open_fp (NULL, inp, 0);
	if (mpeg == NULL) {
		return (1);
	}

	mpeg->ext = out;

	mpeg->mpeg_system_header = &mpeg_scan_system_header;
	mpeg->mpeg_pack = &mpeg_scan_pack;
	mpeg->mpeg_packet = &mpeg_scan_packet;
	mpeg->mpeg_packet_check = &mpeg_packet_check;
	mpeg->mpeg_end = &mpeg_scan_end;

	r = mpegd_parse (mpeg);

	mpeg_print_stats (mpeg, out);

	mpegd_close (mpeg);

	return (r);
}
