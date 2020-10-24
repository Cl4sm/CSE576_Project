void mpeg_print_stats (mpeg_demux_t *mpeg, FILE *fp)
{
	unsigned i;

	fprintf (fp,
		"System headers: %lu\n"
		"Packs:          %lu\n"
		"Packets:        %lu\n"
		"End codes:      %lu\n"
		"Skipped:        %lu bytes\n",
		mpeg->shdr_cnt, mpeg->pack_cnt, mpeg->packet_cnt, mpeg->end_cnt,
		mpeg->skip_cnt
	);

	for (i = 0; i < 256; i++) {
		if (mpeg->streams[i].packet_cnt > 0) {
			fprintf (fp,
			"Stream %02x:      "
			"%lu packets / %" PRIuMAX " bytes\n",
				i, mpeg->streams[i].packet_cnt,
				(uintmax_t) mpeg->streams[i].size
			);
		}
	}

	for (i = 0; i < 256; i++) {
		if (mpeg->substreams[i].packet_cnt > 0) {
			fprintf (fp,
				"Substream %02x:   "
				"%lu packets / %" PRIuMAX " bytes\n",
				i, mpeg->substreams[i].packet_cnt,
				(uintmax_t) mpeg->substreams[i].size
			);
		}
	}

	fflush (fp);
}
