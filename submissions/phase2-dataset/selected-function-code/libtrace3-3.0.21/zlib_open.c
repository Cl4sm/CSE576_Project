io_t *zlib_open(io_t *parent)
{
	io_t *io;
	if (!parent)
		return NULL;
	io = malloc(sizeof(io_t));
	io->source = &zlib_source;
	io->data = malloc(sizeof(struct zlib_t));

	DATA(io)->parent = parent;

	DATA(io)->strm.next_in = NULL;
	DATA(io)->strm.avail_in = 0;
	DATA(io)->strm.next_out = NULL;
	DATA(io)->strm.avail_out = 0;
	DATA(io)->strm.zalloc = Z_NULL;
	DATA(io)->strm.zfree = Z_NULL;
	DATA(io)->strm.opaque = NULL;
	DATA(io)->err = ERR_OK;

	inflateInit2(&DATA(io)->strm, 15 | 32);

	return io;
}