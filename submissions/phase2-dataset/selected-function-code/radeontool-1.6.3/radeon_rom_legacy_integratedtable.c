static void radeon_rom_legacy_integratedtable(unsigned char *bios, int hdr)
{
	int offset, rev, size;

	offset = BIOS16(hdr + 0x62);
	if (offset == 0) {
		printf("no igp table\n");
		return;
	}

	rev = BIOS8(offset);
	size = BIOS8(offset + 1);

	printf("igp table rev %d size %d\n", rev, size);

	if (rev < 2)
		printf("FSB freq in Khz * 10 %d\n", BIOS16(offset + 2));
	else
		printf("FSB freq in Mhz %d\n", BIOS16(offset + 2));

	printf("Memory reference clock in Mhz * 100 %d\n", BIOS16(offset + 4));
	printf("Memory reference divider %d\n", BIOS16(offset + 6));

	if (rev >= 3) {
		printf("K8 memory clock %d\n", BIOS16(offset + 0x11));
	}
}