int mpeg_scan_packet (mpeg_demux_t *mpeg)
{
	FILE               *fp;
	int                skip;
	unsigned           sid, ssid;
	unsigned long long ofs;

	sid = mpeg->packet.sid;
	ssid = mpeg->packet.ssid;

	if (mpeg_stream_excl (sid, ssid)) {
		return (0);
	}

	fp = (FILE *) mpeg->ext;

	ofs = mpeg->ofs;

	if (mpegd_set_offset (mpeg, ofs + mpeg->packet.size)) {
		fprintf (fp,
			"%08" PRIxMAX ": sid=%02x ssid=%02x incomplete packet\n",
			(uintmax_t) ofs, sid, ssid
		);
	}

	skip = 0;

	if (sid == 0xbd) {
		if (mpeg->substreams[ssid].packet_cnt > 1) {
			if (!par_first_pts) {
				return (0);
			}

			if (!mpeg->packet.have_pts) {
				return (0);
			}

			if (mpeg->packet.pts >= pts2[ssid]) {
				return (0);
			}
		}

		if (mpeg->packet.pts < pts2[ssid]) {
			pts2[ssid] = mpeg->packet.pts;
		}
	}
	else {
		if (mpeg->streams[sid].packet_cnt > 1) {
			if (!par_first_pts) {
				return (0);
			}

			if (!mpeg->packet.have_pts) {
				return (0);
			}

			if (mpeg->packet.pts >= pts1[sid]) {
				return (0);
			}
		}

		if (mpeg->packet.pts < pts1[sid]) {
			pts1[sid] = mpeg->packet.pts;
		}
	}

	fprintf (fp, "%08" PRIxMAX ": sid=%02x", (uintmax_t) ofs, sid);

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

	if (mpeg->packet.have_pts) {
		fprintf (fp, " pts=%" PRIuMAX "[%.4f]",
			(uintmax_t) mpeg->packet.pts,
			(double) mpeg->packet.pts / 90000.0
		);
	}

	fputs ("\n", fp);

	fflush (fp);

	return (0);
}
