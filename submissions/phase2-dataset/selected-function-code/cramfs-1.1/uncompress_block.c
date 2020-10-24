{
	int err;

	stream.next_in = src;
	stream.avail_in = len;

	stream.next_out = (unsigned char *) outbuffer;
	stream.avail_out = PAGE_CACHE_SIZE*2;

	inflateReset(&stream);

	if (len > PAGE_CACHE_SIZE*2) {
		die(FSCK_UNCORRECTED, 0, "data block too large");
	}
	err = inflate(&stream, Z_FINISH);
	if (err != Z_STREAM_END) {
		die(FSCK_UNCORRECTED, 0, "decompression error %p(%d): %s",
		    zError(err), src, len);
	}
	return stream.total_out;
}
