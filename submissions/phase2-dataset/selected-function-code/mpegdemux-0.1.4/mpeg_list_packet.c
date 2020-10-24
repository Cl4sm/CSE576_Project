static
int mpeg_list_packet (mpeg_demux_t *mpeg)
{
	FILE     *fp;
	unsigned sid, ssid;

	if (par_no_packet) {
		return (0);
	}

	sid = mpeg->packet.sid;
	ssid = mpeg->packet.ssid;

	if (mpeg_stream_excl (sid, ssid)) {
		return (0);
	}

	fp = (FILE *) mpeg->ext;

	mpeg_list_print_skip (fp);

	fprintf (fp,
		"%08" PRIxMAX ": packet[%lu]: sid=%02x",
		(uintmax_t) mpeg->ofs,
		mpeg->streams[sid].packet_cnt - 1,
		sid
	);

	if (sid == 0xbd) {
		fprintf (fp, "[%02x]", ssid);
	}
	else {
		fputs ("    ", fp);
	}

	if (mpeg->packet.type == 1) {
		fputs (" MPEG1", fp);
	}
	else if (mpeg->packet.type == 2) {
		fputs (" MPEG2", fp);
	}
	else {
		fputs (" UNKWN", fp);
	}

	fprintf (fp, " size=%u", mpeg->packet.size);

	if (mpeg->packet.have_pts || mpeg->packet.have_dts) {
		fprintf (fp,
			" pts=%" PRIuMAX "[%.4f] dts=%" PRIuMAX "[%.4f]",
			(uintmax_t) mpeg->packet.pts,
			(double) mpeg->packet.pts / 90000.0,
			(uintmax_t) mpeg->packet.dts,
			(double) mpeg->packet.dts / 90000.0
		);
	}

	fputs ("\n", fp);

	return (0);
}
