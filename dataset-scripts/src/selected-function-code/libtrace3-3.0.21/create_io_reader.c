static io_t *create_io_reader(const char *filename, int autodetect)
{

	/* Use a peeking reader to look at the start of the trace file and
	 * determine what type of compression may have been used to write
	 * the file */

	DEBUG_PIPELINE("stdio");
	DEBUG_PIPELINE("peek");
	io_t *io = peek_open(stdio_open(filename));
	unsigned char buffer[1024];
	int len;
	if (!io)
		return NULL;
	len = wandio_peek(io, buffer, sizeof(buffer));
	/* Auto detect gzip compressed data -- if autodetect is false,
	 * instead we just assume uncompressed.
	 */

	if (autodetect) {
		if (len>=3 && buffer[0] == 0x1f && buffer[1] == 0x8b &&
				buffer[2] == 0x08) { 
#if HAVE_LIBZ
			DEBUG_PIPELINE("zlib");
			io = zlib_open(io);
#else
			fprintf(stderr, "File %s is gzip compressed but libtrace has not been built with zlib support!\n", filename);
			return NULL;
#endif
		}
		/* Auto detect compress(1) compressed data (gzip can read this) */
		if (len>=2 && buffer[0] == 0x1f && buffer[1] == 0x9d) {
#if HAVE_LIBZ
			DEBUG_PIPELINE("zlib");
			io = zlib_open(io);
#else
			fprintf(stderr, "File %s is compress(1) compressed but libtrace has not been built with zlib support!\n", filename);
			return NULL;
#endif
		}

		/* Auto detect bzip compressed data */
		if (len>=3 && buffer[0] == 'B' && buffer[1] == 'Z' && buffer[2] == 'h') { 
#if HAVE_LIBBZ2
			DEBUG_PIPELINE("bzip");
			io = bz_open(io);
#else
			fprintf(stderr, "File %s is bzip compressed but libtrace has not been built with bzip2 support!\n", filename);
			return NULL;
#endif
		}

                if (len >=5 && buffer[0] == 0xfd && buffer[1] == '7' && 
                                buffer[2] == 'z' && buffer[3] == 'X' &&
                                buffer[4] == 'Z') {
#if HAVE_LIBLZMA
                        DEBUG_PIPELINE("lzma");
                        io = lzma_open(io);
#else
                        fprintf(stderr, "File %s is lzma compressed but libtrace has not been built with lzma support!\n", filename);
                        return NULL;
#endif
                }
	}	
	/* Now open a threaded, peekable reader using the appropriate module
	 * to read the data */

	if (use_threads) {
		DEBUG_PIPELINE("thread");
		io = thread_open(io);
	}
	
	DEBUG_PIPELINE("peek");
	return peek_open(io);
}