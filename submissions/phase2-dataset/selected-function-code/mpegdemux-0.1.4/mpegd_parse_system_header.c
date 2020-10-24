int mpegd_parse_system_header (mpeg_demux_t *mpeg)
{
	unsigned long long ofs;

	mpeg->shdr.size = mpegd_get_bits (mpeg, 32, 16) + 6;

	mpeg->shdr.fixed = mpegd_get_bits (mpeg, 78, 1);
	mpeg->shdr.csps = mpegd_get_bits (mpeg, 79, 1);

	mpeg->shdr_cnt += 1;

	ofs = mpeg->ofs + mpeg->shdr.size;

	if (mpeg->mpeg_system_header != NULL) {
		if (mpeg->mpeg_system_header (mpeg)) {
			return (1);
		}
	}

	mpegd_set_offset (mpeg, ofs);

	return (0);
}
