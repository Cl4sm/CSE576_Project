GetCode(FILE *fd, int code_size, int flag)
{
    static unsigned char buf[280];
    static int curbit, lastbit, done, last_byte;
    int i, j, ret;
    unsigned char count;

    if (flag) {
	curbit = 0;
	lastbit = 0;
	done = FALSE;
	return 0;
    }
    if ((curbit + code_size) >= lastbit) {
	if (done) {
	    if (curbit >= lastbit)
		fprintf(stderr, "ran off the end of my bits...\n");
	    return -1;
	}
	buf[0] = buf[last_byte - 2];
	buf[1] = buf[last_byte - 1];

	if ((count = GetDataBlock(fd, &buf[2])) == 0)
	    done = TRUE;

	last_byte = 2 + count;
	curbit = (curbit - lastbit) + 16;
	lastbit = (2 + count) * 8;
    }
    ret = 0;
    for (i = curbit, j = 0; j < code_size; ++i, ++j)
	ret |= ((buf[i / 8] & (1 << (i % 8))) != 0) << j;

    curbit += code_size;

    return ret;
}
