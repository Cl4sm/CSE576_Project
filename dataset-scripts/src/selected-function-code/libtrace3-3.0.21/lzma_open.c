io_t *lzma_open(io_t *parent)
{
	io_t *io;
	if (!parent)
		return NULL;
	io = malloc(sizeof(io_t));
	io->source = &lzma_source;
	io->data = malloc(sizeof(struct lzma_t));

	DATA(io)->parent = parent;

        memset(&DATA(io)->strm, 0, sizeof(DATA(io)->strm));
	DATA(io)->err = ERR_OK;

        if (lzma_auto_decoder(&DATA(io)->strm, UINT64_MAX, 0) != LZMA_OK) {
            free(io->data);
            free(io);
            fprintf(stderr, "auto decoder failed\n");
            return NULL;
        }

	return io;
}