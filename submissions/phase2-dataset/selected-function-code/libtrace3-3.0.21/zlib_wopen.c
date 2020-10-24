iow_t *zlib_wopen(iow_t *child, int compress_level)
{
	iow_t *iow;
	if (!child)
		return NULL;
	iow = malloc(sizeof(iow_t));
	iow->source = &zlib_wsource;
	iow->data = malloc(sizeof(struct zlibw_t));

	DATA(iow)->child = child;

	DATA(iow)->strm.next_in = NULL;
	DATA(iow)->strm.avail_in = 0;
	DATA(iow)->strm.next_out = DATA(iow)->outbuff;
	DATA(iow)->strm.avail_out = sizeof(DATA(iow)->outbuff);
	DATA(iow)->strm.zalloc = Z_NULL;
	DATA(iow)->strm.zfree = Z_NULL;
	DATA(iow)->strm.opaque = NULL;
	DATA(iow)->err = ERR_OK;

	deflateInit2(&DATA(iow)->strm, 
			compress_level,	/* Level */
			Z_DEFLATED, 	/* Method */
			15 | 16, 	/* 15 bits of windowsize, 16 == use gzip header */
			9,		/* Use maximum (fastest) amount of memory usage */
			Z_DEFAULT_STRATEGY
		);

	return iow;
}