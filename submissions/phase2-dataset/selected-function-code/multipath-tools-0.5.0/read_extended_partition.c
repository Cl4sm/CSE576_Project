read_extended_partition(int fd, struct partition *ep, int en,
			struct slice *sp, int ns)
{
	struct partition p;
	unsigned long start, here, next;
	unsigned char *bp;
	int loopct = 0;
	int moretodo = 1;
	int i, n=0;

	int sector_size_mul = get_sector_size(fd)/512;

	next = start = sector_size_mul * le32_to_cpu(ep->start_sect);

	while (moretodo) {
		here = next;
		moretodo = 0;
		if (++loopct > 100)
			return n;

		bp = (unsigned char *)getblock(fd, here);
		if (bp == NULL)
			return n;

		if (bp[510] != 0x55 || bp[511] != 0xaa)
			return n;

		for (i=0; i<2; i++) {
			memcpy(&p, bp + 0x1be + i * sizeof (p), sizeof (p));
			if (is_extended(p.sys_type)) {
				if (p.nr_sects && !moretodo) {
					next = start + sector_size_mul * le32_to_cpu(p.start_sect);
					moretodo = 1;
				}
				continue;
			}
			if (n < ns) {
				sp[n].start = here + sector_size_mul * le32_to_cpu(p.start_sect);
				sp[n].size = sector_size_mul * le32_to_cpu(p.nr_sects);
				sp[n].container = en + 1;
				n++;
			} else {
				fprintf(stderr,
				    "dos_extd_partition: too many slices\n");
				return n;
			}
			loopct = 0;
		}
	}
	return n;
}
