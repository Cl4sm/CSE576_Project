int mpegd_parse (mpeg_demux_t *mpeg)
{
	unsigned long long ofs;

	while (1) {
		if (mpegd_seek_header (mpeg)) {
			return (0);
		}

		switch (mpegd_get_bits (mpeg, 0, 32)) {
		case MPEG_PACK_START:
			if (mpegd_parse_pack (mpeg)) {
				return (1);
			}
			break;

		case MPEG_END_CODE:
			mpeg->end_cnt += 1;

			ofs = mpeg->ofs + 4;

			if (mpeg->mpeg_end != NULL) {
				if (mpeg->mpeg_end (mpeg)) {
					return (1);
				}
			}

			if (mpegd_set_offset (mpeg, ofs)) {
				return (1);
			}
			break;

		default:
			ofs = mpeg->ofs + 1;

			if (mpeg->mpeg_skip != NULL) {
				if (mpeg->mpeg_skip (mpeg)) {
					return (1);
				}
			}

			if (mpegd_set_offset (mpeg, ofs)) {
				return (0);
			}

			break;
		}
	}

	return (0);
}
