{
	unsigned long curr = offset + 4 * ((size + PAGE_CACHE_SIZE - 1) / PAGE_CACHE_SIZE);

	do {
		unsigned long out = PAGE_CACHE_SIZE;
		unsigned long next = *(u32 *) romfs_read(offset);

		if (next > end_data) {
			end_data = next;
		}

		offset += 4;
		if (curr == next) {
			if (opt_verbose > 1) {
				printf("  hole at %ld (%d)\n", curr, PAGE_CACHE_SIZE);
			}
			if (size < PAGE_CACHE_SIZE)
				out = size;
			memset(outbuffer, 0x00, out);
		}
		else {
			if (opt_verbose > 1) {
				printf("  uncompressing block at %ld to %ld (%ld)\n", curr, next, next - curr);
			}
			out = uncompress_block(romfs_read(curr), next - curr);
		}
		if (size >= PAGE_CACHE_SIZE) {
			if (out != PAGE_CACHE_SIZE) {
				die(FSCK_UNCORRECTED, 0, "non-block (%ld) bytes", out);
			}
		} else {
			if (out != size) {
				die(FSCK_UNCORRECTED, 0, "non-size (%ld vs %ld) bytes", out, size);
			}
		}
		size -= out;
		if (opt_extract) {
			if (write(fd, outbuffer, out) < 0) {
				die(FSCK_ERROR, 1, "write failed: %s", path);
			}
		}
		curr = next;
	} while (size);
}
