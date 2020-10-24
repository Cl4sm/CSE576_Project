{
	unsigned long original_size = size;
	unsigned long original_offset = offset;
	unsigned long new_size;
	unsigned long blocks = (size - 1) / blksize + 1;
	unsigned long curr = offset + 4 * blocks;
	int change;

	total_blocks += blocks;

	do {
		unsigned long len = 2 * blksize;
		unsigned int input = size;
		int err;

		if (input > blksize)
			input = blksize;
		size -= input;
		if (!(opt_holes && is_zero (uncompressed, input))) {
			err = compress2(base + curr, &len, uncompressed, input, Z_BEST_COMPRESSION);
			if (err != Z_OK) {
				die(MKFS_ERROR, 0, "compression error: %s", zError(err));
			}
			curr += len;
		}
		uncompressed += input;

		if (len > blksize*2) {
			/* (I don't think this can happen with zlib.) */
			die(MKFS_ERROR, 0, "AIEEE: block \"compressed\" to > 2*blocklength (%ld)", len);
		}

		*(u32 *) (base + offset) = curr;
		offset += 4;
	} while (size);

	curr = (curr + 3) & ~3;
	new_size = curr - original_offset;
	/* TODO: Arguably, original_size in these 2 lines should be
	   st_blocks * 512.  But if you say that then perhaps
	   administrative data should also be included in both. */
	change = new_size - original_size;
	if (opt_verbose > 1) {
		printf("%6.2f%% (%+d bytes)\t%s\n",
		       (change * 100) / (double) original_size, change, name);
	}

	return curr;
}
