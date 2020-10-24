iow_t *bz_wopen(iow_t *child, int compress_level)
{
	iow_t *iow;
	if (!child)
		return NULL;
	iow = malloc(sizeof(iow_t));
	iow->source = &bz_wsource;
	iow->data = malloc(sizeof(struct bzw_t));

	DATA(iow)->child = child;

	DATA(iow)->strm.next_in = NULL;
	DATA(iow)->strm.avail_in = 0;
	DATA(iow)->strm.next_out = DATA(iow)->outbuff;
	DATA(iow)->strm.avail_out = sizeof(DATA(iow)->outbuff);
	DATA(iow)->strm.bzalloc = NULL;
	DATA(iow)->strm.bzfree = NULL;
	DATA(iow)->strm.opaque = NULL;
	DATA(iow)->err = ERR_OK;

	BZ2_bzCompressInit(&DATA(iow)->strm, 
			compress_level,	/* Block size */
			0,		/* Verbosity */
			30);		/* Work factor */

	return iow;
}