mpeg_demux_t *mpegd_open_fp (mpeg_demux_t *mpeg, FILE *fp, int close)
{
	if (mpeg == NULL) {
		mpeg = malloc (sizeof (mpeg_demux_t));
		if (mpeg == NULL) {
			return (NULL);
		}
		mpeg->free = 1;
	}
	else {
		mpeg->free = 0;
	}

	mpeg->fp = fp;
	mpeg->close = close;

	mpeg->ofs = 0;

	mpeg->buf_i = 0;
	mpeg->buf_n = 0;

	mpeg->ext = NULL;

	mpeg->mpeg_skip = NULL;
	mpeg->mpeg_system_header = NULL;
	mpeg->mpeg_packet = NULL;
	mpeg->mpeg_packet_check = NULL;
	mpeg->mpeg_pack = NULL;
	mpeg->mpeg_end = NULL;

	mpegd_reset_stats (mpeg);

	return (mpeg);
}
