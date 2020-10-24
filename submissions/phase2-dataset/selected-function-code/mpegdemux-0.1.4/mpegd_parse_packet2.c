int mpegd_parse_packet2 (mpeg_demux_t *mpeg, unsigned i)
{
	unsigned           pts_dts_flag;
	unsigned           cnt;
	unsigned long long tmp;

	mpeg->packet.type = 2;

	pts_dts_flag = mpegd_get_bits (mpeg, i + 8, 2);
	cnt = mpegd_get_bits (mpeg, i + 16, 8);

	if (pts_dts_flag == 0x02) {
		if (mpegd_get_bits (mpeg, i + 24, 4) == 0x02) {
			tmp = mpegd_get_bits (mpeg, i + 28, 3);
			tmp = (tmp << 15) | mpegd_get_bits (mpeg, i + 32, 15);
			tmp = (tmp << 15) | mpegd_get_bits (mpeg, i + 48, 15);

			mpeg->packet.have_pts = 1;
			mpeg->packet.pts = tmp;
		}
	}
	else if ((pts_dts_flag & 0x03) == 0x03) {
		if (mpegd_get_bits (mpeg, i + 24, 4) == 0x03) {
			tmp = mpegd_get_bits (mpeg, i + 28, 3);
			tmp = (tmp << 15) | mpegd_get_bits (mpeg, i + 32, 15);
			tmp = (tmp << 15) | mpegd_get_bits (mpeg, i + 48, 15);

			mpeg->packet.have_pts = 1;
			mpeg->packet.pts = tmp;
		}

		if (mpegd_get_bits (mpeg, i + 64, 4) == 0x01) {
			tmp = mpegd_get_bits (mpeg, i + 68, 3);
			tmp = (tmp << 15) | mpegd_get_bits (mpeg, i + 72, 15);
			tmp = (tmp << 15) | mpegd_get_bits (mpeg, i + 88, 15);

			mpeg->packet.have_dts = 1;
			mpeg->packet.dts = tmp;
		}
	}

	i += 8 * (cnt + 3);

	mpeg->packet.offset = i / 8;

	return (0);
}
