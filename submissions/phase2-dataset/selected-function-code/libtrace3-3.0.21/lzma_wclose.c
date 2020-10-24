static void lzma_wclose(iow_t *iow)
{
	lzma_ret res;
	while (1) {
		res = lzma_code(&DATA(iow)->strm, LZMA_FINISH);

		if (res == LZMA_STREAM_END)
			break;
		if (res != LZMA_OK) {
			fprintf(stderr, "Z_STREAM_ERROR while closing output\n");
			break;
		}

		wandio_wwrite(DATA(iow)->child,
				(char*)DATA(iow)->outbuff,
				sizeof(DATA(iow)->outbuff)-DATA(iow)->strm.avail_out);
		DATA(iow)->strm.next_out = DATA(iow)->outbuff;
		DATA(iow)->strm.avail_out = sizeof(DATA(iow)->outbuff);
	}

	wandio_wwrite(DATA(iow)->child,
			(char *)DATA(iow)->outbuff,
			sizeof(DATA(iow)->outbuff)-DATA(iow)->strm.avail_out);
	lzma_end(&DATA(iow)->strm);
	wandio_wdestroy(DATA(iow)->child);
	free(iow->data);
	free(iow);
}