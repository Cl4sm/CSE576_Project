static
int mpeg_remux_system_header (mpeg_demux_t *mpeg)
{
	if (par_no_shdr && (mpeg->shdr_cnt > 1)) {
		return (0);
	}

	if (mpeg_buf_write_clear (&pack, mpeg_ext_fp (mpeg))) {
		return (1);
	}

	if (mpeg_buf_read (&shdr, mpeg, mpeg->shdr.size)) {
		return (1);
	}

	if (mpeg_buf_write_clear (&shdr, mpeg_ext_fp (mpeg))) {
		return (1);
	}

	return (0);
}
