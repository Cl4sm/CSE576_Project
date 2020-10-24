int mpeg_list (FILE *inp, FILE *out)
{
	int          r;
	mpeg_demux_t *mpeg;

	mpeg = mpegd_open_fp (NULL, inp, 0);
	if (mpeg == NULL) {
		return (1);
	}

	skip_cnt = 0;
	skip_ofs = 0;

	mpeg->ext = out;

	mpeg->mpeg_skip = &mpeg_list_skip;
	mpeg->mpeg_system_header = &mpeg_list_system_header;
	mpeg->mpeg_pack = &mpeg_list_pack;
	mpeg->mpeg_packet = &mpeg_list_packet;
	mpeg->mpeg_packet_check = &mpeg_packet_check;
	mpeg->mpeg_end = &mpeg_list_end;

	r = mpegd_parse (mpeg);

	mpeg_list_print_skip (out);

	mpeg_print_stats (mpeg, out);

	mpegd_close (mpeg);

	return (r);
}
