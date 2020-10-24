int mpeg_list_pack (mpeg_demux_t *mpeg)
{
	FILE *fp;

	if (par_no_pack) {
		return (0);
	}

	fp = (FILE *) mpeg->ext;

	mpeg_list_print_skip (fp);

	fprintf (fp,
		"%08" PRIxMAX ": pack[%lu]: "
		"type=%u scr=%" PRIuMAX "[%.4f] mux=%lu[%.2f] stuff=%u\n",
		(uintmax_t) mpeg->ofs,
		mpeg->pack_cnt - 1,
		mpeg->pack.type,
		(uintmax_t) mpeg->pack.scr,
		(double) mpeg->pack.scr / 90000.0,
		mpeg->pack.mux_rate, 50.0 * mpeg->pack.mux_rate,
		mpeg->pack.stuff
	);

	fflush (fp);

	return (0);
}
