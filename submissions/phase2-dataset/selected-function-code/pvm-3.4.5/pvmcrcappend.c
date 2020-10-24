unsigned int
pvmcrcappend(buf, len, crc)
	char *buf;			/* buffer to append to crc value */
	int len;			/* length */
	unsigned int crc;	/* initial value */
{
	static unsigned int crctbl[256];
	static int once = 1;

	if (once) {
		int i, j;
		unsigned int v;

		for (i = 0; i < 256; i++) {
			v = i;
			for (j = 8; j--; )
				v = v & 1 ? (v >> 1) ^ PVMCRCPOLY : v >> 1;
			crctbl[i] = v;
		}
		once = 0;
	}

	while (len-- > 0)
		crc = ((crc >> 8) & 0x00ffffff) ^ crctbl[(crc ^ *buf++) & 0xff];
	return crc;
}
