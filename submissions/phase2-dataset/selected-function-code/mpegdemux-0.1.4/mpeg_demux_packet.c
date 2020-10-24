int mpeg_demux_packet (mpeg_demux_t *mpeg)
{
	unsigned sid, ssid;
	unsigned fpi;
	unsigned cnt;
	int      r;

	sid = mpeg->packet.sid;
	ssid = mpeg->packet.ssid;

	if (mpeg_stream_excl (sid, ssid)) {
		return (0);
	}

	cnt = mpeg->packet.offset;

	fpi = sid;

	/* select substream in private stream 1 (AC3 audio) */
	if (sid == 0xbd) {
		fpi = 256 + ssid;
		cnt += 1;

		if (par_dvdac3) {
			cnt += 3;
		}
	}

	if (cnt > mpeg->packet.size) {
		prt_msg ("demux: AC3 packet too small (sid=%02x size=%u)\n",
			sid, mpeg->packet.size
		);

		return (1);
	}

	if (fp[fpi] == NULL) {
		fp[fpi] = mpeg_demux_open (mpeg, sid, ssid);
		if (fp[fpi] == NULL) {
			return (1);
		}
	}

	if (cnt > 0) {
		mpegd_skip (mpeg, cnt);
	}

	cnt = mpeg->packet.size - cnt;

	if ((sid == 0xbd) && par_dvdsub) {
		return (mpeg_demux_copy_spu (mpeg, fp[fpi], cnt));
	}

	r = 0;

	if (mpeg_buf_read (&packet, mpeg, cnt)) {
		prt_msg ("demux: incomplete packet (sid=%02x size=%u/%u)\n",
			sid, packet.cnt, cnt
		);

		if (par_drop) {
			mpeg_buf_clear (&packet);
			return (1);
		}

		r = 1;
	}

	if (mpeg_buf_write_clear (&packet, fp[fpi])) {
		r = 1;
	}

	return (r);
}
