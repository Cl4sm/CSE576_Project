io_t *bz_open(io_t *parent)
{
	io_t *io;
	if (!parent)
		return NULL;
	io = malloc(sizeof(io_t));
	io->source = &bz_source;
	io->data = malloc(sizeof(struct bz_t));

	DATA(io)->parent = parent;

	DATA(io)->strm.next_in = NULL;
	DATA(io)->strm.avail_in = 0;
	DATA(io)->strm.next_out = NULL;
	DATA(io)->strm.avail_out = 0;
	DATA(io)->strm.bzalloc = NULL;
	DATA(io)->strm.bzfree = NULL;
	DATA(io)->strm.opaque = NULL;
	DATA(io)->err = ERR_OK;

	BZ2_bzDecompressInit(&DATA(io)->strm, 
		0, 	/* Verbosity */
		0);	/* small */

	return io;
}