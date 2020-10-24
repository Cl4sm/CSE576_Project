static void zlib_wclose(iow_t *iow)
{
	int res;
	
	while (1) {
		res = deflate(&DATA(iow)->strm, Z_FINISH);

		if (res == Z_STREAM_END)
			break;
		if (res == Z_STREAM_ERROR) {
			fprintf(stderr, "Z_STREAM_ERROR while closing output\n");
			break;
		}
	
		wandio_wwrite(DATA(iow)->child, 
				(char*)DATA(iow)->outbuff,
				sizeof(DATA(iow)->outbuff)-DATA(iow)->strm.avail_out);
		DATA(iow)->strm.next_out = DATA(iow)->outbuff;
		DATA(iow)->strm.avail_out = sizeof(DATA(iow)->outbuff);
	}

	deflateEnd(&DATA(iow)->strm);
	wandio_wwrite(DATA(iow)->child, 
			(char *)DATA(iow)->outbuff,
			sizeof(DATA(iow)->outbuff)-DATA(iow)->strm.avail_out);
	wandio_wdestroy(DATA(iow)->child);
	free(iow->data);
	free(iow);
}