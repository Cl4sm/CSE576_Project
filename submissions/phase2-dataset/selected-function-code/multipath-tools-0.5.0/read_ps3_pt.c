read_ps3_pt(int fd, struct slice all, struct slice *sp, int ns) {
	struct disklabel label;
        int n = 0;
	int i;

	if (!read_disklabel(fd, &label))
		return -1;

	if ((be64_to_cpu(label.d_magic1) != MAGIC1) ||
	    (be64_to_cpu(label.d_magic2) != MAGIC2))
		return -1;

	for (i = 0; i < MAX_PARTITIONS; i++) {
		if (label.d_partitions[i].p_start && label.d_partitions[i].p_size) {
			sp[n].start =  be64_to_cpu(label.d_partitions[i].p_start);
			sp[n].size =  be64_to_cpu(label.d_partitions[i].p_size);
			n++;
		}
	}

	return n;
}
