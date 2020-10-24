io_t *stdio_open(const char *filename)
{
	io_t *io = malloc(sizeof(io_t));
	io->data = malloc(sizeof(struct stdio_t));

	if (strcmp(filename,"-") == 0)
		DATA(io)->fd = 0; /* STDIN */
	else
		DATA(io)->fd = open(filename,
			O_RDONLY
#ifdef O_DIRECT
			|(force_directio_read?O_DIRECT:0)
#endif
			);
	io->source = &stdio_source;

	if (DATA(io)->fd == -1) {
		free(io);
		return NULL;
	}

	return io;
}