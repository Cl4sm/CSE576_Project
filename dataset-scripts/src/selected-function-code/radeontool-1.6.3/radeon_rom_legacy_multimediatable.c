static void radeon_rom_legacy_multimediatable(unsigned char *bios, int hdr)
{
	int offset, rev, size;
	int tmp;

	offset = BIOS16(hdr + 0x38);
	if (offset == 0) {
		printf("no mm info table\n");
		return;
	}
	rev = BIOS8(offset - 2);
	size = BIOS8(offset - 1);

	printf("mm table rev %d size %d\n", rev, size);

	tmp = BIOS8(offset);
	printf("tuner id %x - video input %d\n", tmp & 0x1f,
	       (tmp >> 5) & 0x7);

	tmp = BIOS8(offset + 1);
	printf("audio chip %x\n", tmp & 0xf);
	printf("product %s - rev %d\n", tmp & 0x10 ? "ATI" : "OEM",
	       tmp >> 5);
	printf("product ID %d\n", BIOS8(offset + 2));
	
	tmp = BIOS8(offset + 3);
	printf("tuner regulator control %d\n", tmp & 0x3);
	printf("teletext support %d\n", (tmp >> 2) & 0x3);
	printf("fm audio decoder %d\n", (tmp >> 4) & 0x3);
	printf("audio scramming %d\n", tmp >> 7);

	tmp = BIOS8(offset + 4);
	printf("i2s input config %d, output config %d\n", tmp & 0x1,
	       (tmp >> 1) & 0x1);
	printf("i2s audio chip %d\n", (tmp >> 2) & 0x3);

	tmp = BIOS8(offset + 5);
	printf("video decoder type %x\n", tmp & 0xf);
	printf("video crystal %d\n", (tmp >> 4) & 0xf);
	
	tmp = BIOS8(offset + 6);
	printf("video decoder config %d\n", tmp);
}