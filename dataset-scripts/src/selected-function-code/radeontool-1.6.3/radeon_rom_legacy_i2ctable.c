static void radeon_rom_legacy_i2ctable(unsigned char *bios, int hdr)
{
    int offset, rev, size, blocks, i, id, reg, clock, data;

	offset = BIOS16(hdr + 0x70);
	if (offset == 0) {
		printf("No i2c info table\n");
		return;
	}
	rev = BIOS8(offset);
	printf("\ni2c table revision: %d\n", rev);
	size = BIOS8(offset + 1);
	printf("i2c table size: %d\n", size);

	blocks = BIOS8(offset + 2);
	printf("i2c blocks: %d\n", blocks);

	for (i = 0; i < blocks; i++) {
	    id = BIOS8(offset + 3 + (i * 5) + 0);
	    reg = BIOS16(offset + 3 + (i * 5) + 1);
	    clock = BIOS8(offset + 3 + (i * 5) + 3);
	    data = BIOS8(offset + 3 + (i * 5) + 4);
	    printf("id: %d, reg: 0x%x, clk: %d, data: %d\n", id, reg * 4, clock, data);
	}

}