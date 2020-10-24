int mpegd_parse_packet1 (mpeg_demux_t *mpeg, unsigned i)
{
	unsigned           val;
	unsigned long long tmp;

	mpeg->packet.type = 1;

	if (mpegd_get_bits (mpeg, i, 2) == 0x01) {
		i += 16;
	}

	val = mpegd_get_bits (mpeg, i, 8);

	if ((val & 0xf0) == 0x20) {
		tmp = mpegd_get_bits (mpeg, i + 4, 3);
		tmp = (tmp << 15) | mpegd_get_bits (mpeg, i + 8, 15);
		tmp = (tmp << 15) | mpegd_get_bits (mpeg, i + 24, 15);

		mpeg->packet.have_pts = 1;
		mpeg->packet.pts = tmp;

		i += 40;
	}
	else if ((val & 0xf0) == 0x30) {
		tmp = mpegd_get_bits (mpeg, i + 4, 3);
		tmp = (tmp << 15) | mpegd_get_bits (mpeg, i + 8, 15);
		tmp = (tmp << 15) | mpegd_get_bits (mpeg, i + 24, 15);

		mpeg->packet.have_pts = 1;
		mpeg->packet.pts = tmp;

		tmp = mpegd_get_bits (mpeg, i + 44, 3);
		tmp = (tmp << 15) | mpegd_get_bits (mpeg, i + 48, 15);
		tmp = (tmp << 15) | mpegd_get_bits (mpeg, i + 64, 15);

		mpeg->packet.have_dts = 1;
		mpeg->packet.dts = tmp;

		i += 80;
	}
	else if (val == 0x0f) {
		i += 8;
	}

	mpeg->packet.offset = i / 8;

	return (0);
}
