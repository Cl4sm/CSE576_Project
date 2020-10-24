DLLEXPORT iow_t *wandio_wcreate(const char *filename, int compress_type, int compression_level, int flags)
{
	iow_t *iow;
	parse_env();

	assert ( compression_level >= 0 && compression_level <= 9 );
	assert (compress_type != WANDIO_COMPRESS_MASK);

	iow=stdio_wopen(filename, flags);
	if (!iow)
		return NULL;

	/* We prefer zlib if available, otherwise we'll use bzip. If neither
	 * are present, guess we'll just have to write uncompressed */
#if HAVE_LIBZ
	if (compression_level != 0 && 
	    compress_type == WANDIO_COMPRESS_ZLIB) {
		iow = zlib_wopen(iow,compression_level);
	}
#endif
#if HAVE_LIBLZO2
	else if (compression_level != 0 && 
	    compress_type == WANDIO_COMPRESS_LZO) {
		iow = lzo_wopen(iow,compression_level);
	}
#endif
#if HAVE_LIBBZ2
	else if (compression_level != 0 && 
	    compress_type == WANDIO_COMPRESS_BZ2) {
		iow = bz_wopen(iow,compression_level);
	}
#endif
#if HAVE_LIBLZMA
        else if (compression_level != 0 && 
            compress_type == WANDIO_COMPRESS_LZMA) {
                iow = lzma_wopen(iow,compression_level);
        }
#endif
	/* Open a threaded writer */
	if (use_threads)
		return thread_wopen(iow);
	else
		return iow;
}