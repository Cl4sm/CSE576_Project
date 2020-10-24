static
int mpegd_parse_packet (mpeg_demux_t *mpeg)
{
	unsigned           i;
	unsigned           sid, ssid;
	unsigned long long ofs;

	mpeg->packet.type = 0;

	sid = mpegd_get_bits (mpeg, 24, 8);
	ssid = 0;

	mpeg->packet.sid = sid;
	mpeg->packet.ssid = ssid;

	mpeg->packet.size = mpegd_get_bits (mpeg, 32, 16) + 6;
	mpeg->packet.offset = 6;

	mpeg->packet.have_pts = 0;
	mpeg->packet.pts = 0;

	mpeg->packet.have_dts = 0;
	mpeg->packet.dts = 0;

	i = 48;

	if (((sid >= 0xc0) && (sid < 0xf0)) || (sid == 0xbd)) {
		while (mpegd_get_bits (mpeg, i, 8) == 0xff) {
			if (i > (48 + 16 * 8)) {
				break;
			}
			i += 8;
		}

		if (mpegd_get_bits (mpeg, i, 2) == 0x02) {
			if (mpegd_parse_packet2 (mpeg, i)) {
				return (1);
			}
		}
		else {
			if (mpegd_parse_packet1 (mpeg, i)) {
				return (1);
			}
		}
	}
	else if (sid == 0xbe) {
		mpeg->packet.type = 1;
	}

	if (sid == 0xbd) {
		ssid = mpegd_get_bits (mpeg, 8 * mpeg->packet.offset, 8);
		mpeg->packet.ssid = ssid;
	}

	if ((mpeg->mpeg_packet_check != NULL) && mpeg->mpeg_packet_check (mpeg)) {
		if (mpegd_skip (mpeg, 1)) {
			return (1);
		}
	}
	else {
		mpeg->packet_cnt += 1;
		mpeg->streams[sid].packet_cnt += 1;
		mpeg->streams[sid].size += mpeg->packet.size - mpeg->packet.offset;

		if (sid == 0xbd) {
			mpeg->substreams[ssid].packet_cnt += 1;
			mpeg->substreams[ssid].size += mpeg->packet.size - mpeg->packet.offset;
		}

		ofs = mpeg->ofs + mpeg->packet.size;

		if (mpeg->mpeg_packet != NULL) {
			if (mpeg->mpeg_packet (mpeg)) {
				return (1);
			}
		}

		mpegd_set_offset (mpeg, ofs);
	}

	return (0);
}
