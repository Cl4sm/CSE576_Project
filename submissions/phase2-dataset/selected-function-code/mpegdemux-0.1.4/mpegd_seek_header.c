int mpegd_seek_header (mpeg_demux_t *mpeg)
{
	unsigned long long ofs;

	while (mpegd_get_bits (mpeg, 0, 24) != 1) {
		ofs = mpeg->ofs + 1;

		if (mpeg->mpeg_skip != NULL) {
			if (mpeg->mpeg_skip (mpeg)) {
				return (1);
			}
		}

		if (mpegd_set_offset (mpeg, ofs)) {
			return (1);
		}

		mpeg->skip_cnt += 1;
	}

	return (0);
}
