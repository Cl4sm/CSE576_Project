int mpeg_demux_copy_spu (mpeg_demux_t *mpeg, FILE *fp, unsigned cnt)
{
	static unsigned    spucnt = 0;
	static int         half = 0;
	unsigned           i, n;
	unsigned char      buf[8];
	unsigned long long pts;

	if (half) {
		mpegd_read (mpeg, buf, 1);

		if (fwrite (buf, 1, 1, fp) != 1) {
			return (1);
		}

		spucnt = (spucnt << 8) + buf[0];
		half = 0;

		spucnt -= 2;
		cnt -= 1;
	}

	while (cnt > 0) {
		if (spucnt == 0) {
			pts = mpeg->packet.pts;
			for (i = 0; i < 8; i++) {
				buf[7 - i] = pts & 0xff;
				pts = pts >> 8;
			}

			if (fwrite (buf, 1, 8, fp) != 8) {
				return (1);
			}

			if (cnt == 1) {
				mpegd_read (mpeg, buf, 1);

				if (fwrite (buf, 1, 1, fp) != 1) {
					return (1);
				}

				spucnt = buf[0];
				half = 1;

				return (0);
			}

			mpegd_read (mpeg, buf, 2);

			if (fwrite (buf, 1, 2, fp) != 2) {
				return (1);
			}

			spucnt = (buf[0] << 8) + buf[1];

			if (spucnt < 2) {
				return (1);
			}

			spucnt -= 2;
			cnt -= 2;
		}

		n = (cnt < spucnt) ? cnt : spucnt;

		mpeg_copy (mpeg, fp, n);
		cnt -= n;
		spucnt -= n;
	}

	return (0);
}
