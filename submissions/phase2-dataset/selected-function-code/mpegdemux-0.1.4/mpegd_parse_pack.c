int mpegd_parse_pack (mpeg_demux_t *mpeg)
{
	unsigned           sid;
	unsigned long long ofs;

	if (mpegd_get_bits (mpeg, 32, 4) == 0x02) {
		mpeg->pack.type = 1;
		mpeg->pack.scr = mpegd_get_bits (mpeg, 36, 3);
		mpeg->pack.scr = (mpeg->pack.scr << 15) | mpegd_get_bits (mpeg, 40, 15);
		mpeg->pack.scr = (mpeg->pack.scr << 15) | mpegd_get_bits (mpeg, 56, 15);
		mpeg->pack.mux_rate = mpegd_get_bits (mpeg, 73, 22);
		mpeg->pack.stuff = 0;
		mpeg->pack.size = 12;
	}
	else if (mpegd_get_bits (mpeg, 32, 2) == 0x01) {
		mpeg->pack.type = 2;
		mpeg->pack.scr = mpegd_get_bits (mpeg, 34, 3);
		mpeg->pack.scr = (mpeg->pack.scr << 15) | mpegd_get_bits (mpeg, 38, 15);
		mpeg->pack.scr = (mpeg->pack.scr << 15) | mpegd_get_bits (mpeg, 54, 15);
		mpeg->pack.mux_rate = mpegd_get_bits (mpeg, 80, 22);
		mpeg->pack.stuff = mpegd_get_bits (mpeg, 109, 3);
		mpeg->pack.size = 14 + mpeg->pack.stuff;
	}
	else {
		mpeg->pack.type = 0;
		mpeg->pack.scr = 0;
		mpeg->pack.mux_rate = 0;
		mpeg->pack.size = 4;
	}

	ofs = mpeg->ofs + mpeg->pack.size;

	mpeg->pack_cnt += 1;

	if (mpeg->mpeg_pack != NULL) {
		if (mpeg->mpeg_pack (mpeg)) {
			return (1);
		}
	}

	mpegd_set_offset (mpeg, ofs);

	mpegd_seek_header (mpeg);

	if (mpegd_get_bits (mpeg, 0, 32) == MPEG_SYSTEM_HEADER) {
		if (mpegd_parse_system_header (mpeg)) {
			return (1);
		}

		mpegd_seek_header (mpeg);
	}

	while (mpegd_get_bits (mpeg, 0, 24) == MPEG_PACKET_START) {
		sid = mpegd_get_bits (mpeg, 24, 8);

		if ((sid == 0xba) || (sid == 0xb9) || (sid == 0xbb)) {
			break;
		}
		else {
			mpegd_parse_packet (mpeg);
		}

		mpegd_seek_header (mpeg);
	}

	return (0);
}
