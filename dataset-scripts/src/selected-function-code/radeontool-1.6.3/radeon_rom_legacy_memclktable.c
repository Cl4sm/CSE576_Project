void radeon_rom_legacy_memclktable(unsigned char *bios, int hdr)
{ 
	int offset = BIOS16(hdr + 0x5a);
	int block_rev;
	if (!offset)
		return;

	block_rev = BIOS8(offset);
	while (block_rev != 0xff) {
		printf("mem clock table header %d\n", block_rev);
		if (block_rev == 1) {
			printf("mem clock min %d ,max %d\n", BIOS16(offset + 1), BIOS16(offset + 3));
			offset += 5;
		}
		if (block_rev == 2) {
			printf("pre mem clock reset table %d\n", BIOS16(offset + 1));
			offset += 3;
		}
		if (block_rev == 3) {
			printf("pll change table %d\n", BIOS16(offset + 1));
			offset += 3;
		}
		if (block_rev == 4) {
			printf("register change table %d\n", BIOS16(offset + 1));
			offset += 3;
		}
		if (block_rev == 5) {
			printf("post memory clock reset seq table %d\n", BIOS16(offset + 1));
			offset += 3;
		}
		if (block_rev == 6) {
			printf("mc register change table %d\n", BIOS16(offset + 1));
			offset += 3;
		}
		if (block_rev == 7) {
			printf("pll pre clock lock table %d\n", BIOS16(offset + 1));
			offset += 3;
		}
		if (block_rev == 8) {
			printf("pre clock change table %d\n", BIOS16(offset + 1));
			offset += 3;
		}
		if (block_rev == 9) {
			printf("post memory clock reset seq table 2 %d\n", BIOS16(offset + 1));
			offset += 3;
		}
		block_rev = BIOS8(offset);
	}
}