int mpeg_remux_packet (mpeg_demux_t *mpeg)
{
	int      r;
	unsigned sid, ssid;

	sid = mpeg->packet.sid;
	ssid = mpeg->packet.ssid;

	if (mpeg_stream_excl (sid, ssid)) {
		return (0);
	}

	r = 0;

	if (mpeg_buf_read (&packet, mpeg, mpeg->packet.size)) {
		prt_msg ("remux: incomplete packet (sid=%02x size=%u/%u)\n",
			sid, packet.cnt, mpeg->packet.size
		);

		if (par_drop) {
			mpeg_buf_clear (&packet);
			return (1);
		}

		r = 1;
	}

	if (packet.cnt >= 4) {
		packet.buf[3] = par_stream_map[sid];

		if ((sid == 0xbd) && (packet.cnt > mpeg->packet.offset)) {
			packet.buf[mpeg->packet.offset] = par_substream_map[ssid];
		}
	}

	if (mpeg_buf_write_clear (&pack, mpeg_ext_fp (mpeg))) {
		return (1);
	}

	if (mpeg_buf_write_clear (&packet, mpeg_ext_fp (mpeg))) {
		return (1);
	}

	return (r);
}
