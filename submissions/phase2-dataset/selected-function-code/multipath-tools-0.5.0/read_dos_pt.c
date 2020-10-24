int
read_dos_pt(int fd, struct slice all, struct slice *sp, int ns) {
	struct partition p;
	unsigned long offset = all.start;
	int i, n=4;
	unsigned char *bp;
	int sector_size_mul = get_sector_size(fd)/512;

	bp = (unsigned char *)getblock(fd, offset);
	if (bp == NULL)
		return -1;

	if (bp[510] != 0x55 || bp[511] != 0xaa)
		return -1;

	for (i=0; i<4; i++) {
		memcpy(&p, bp + 0x1be + i * sizeof (p), sizeof (p));
		if (is_gpt(p.sys_type))
			return 0;
		if (i < ns) {
			sp[i].start =  sector_size_mul * le32_to_cpu(p.start_sect);
			sp[i].size = sector_size_mul * le32_to_cpu(p.nr_sects);
		} else {
			fprintf(stderr,
				"dos_partition: too many slices\n");
			break;
		}
		if (is_extended(p.sys_type)) {
			sp[i].size = sector_size_mul * 2; /* extended partitions only get two
					   sectors mapped for LILO to install */
			n += read_extended_partition(fd, &p, i, sp+n, ns-n);
		}
	}
	return n;
}
