int mpeg_list_system_header (mpeg_demux_t *mpeg)
{
	FILE *fp;

	if (par_no_shdr) {
		return (0);
	}

	fp = (FILE *) mpeg->ext;

	mpeg_list_print_skip (fp);

	fprintf (fp, "%08" PRIxMAX ": system header[%lu]: "
		"size=%u fixed=%d csps=%d\n",
		(uintmax_t) mpeg->ofs,
		mpeg->shdr_cnt - 1,
		mpeg->shdr.size, mpeg->shdr.fixed, mpeg->shdr.csps
	);

	return (0);
}
