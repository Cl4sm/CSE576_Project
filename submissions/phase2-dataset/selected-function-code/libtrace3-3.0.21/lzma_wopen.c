iow_t *lzma_wopen(iow_t *child, int compress_level)
{
	iow_t *iow;
	if (!child)
		return NULL;
	iow = malloc(sizeof(iow_t));
	iow->source = &lzma_wsource;
	iow->data = malloc(sizeof(struct lzmaw_t));

	DATA(iow)->child = child;

        memset(&DATA(iow)->strm, 0, sizeof(DATA(iow)->strm));
	DATA(iow)->strm.next_out = DATA(iow)->outbuff;
	DATA(iow)->strm.avail_out = sizeof(DATA(iow)->outbuff);
	DATA(iow)->err = ERR_OK;

        if (lzma_easy_encoder(&DATA(iow)->strm,
                    compress_level,
                    LZMA_CHECK_CRC64) != LZMA_OK) {
            free(iow->data);
            free(iow);
            return NULL;
        }

	return iow;
}