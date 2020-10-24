static void radeon_rom_legacy_ram_reset_table(unsigned char *bios, int hdr)
{
	int offset = BIOS16(hdr + 0x48);
	uint32_t val;

	if (!offset)
		return;

	while (BIOS8(offset++));
	offset += 2;

	printf("ram reset table %d rev %d and mask %x\n", offset, BIOS8(offset - 1), BIOS8(offset - 2));
	val = BIOS8(offset);
	while(val != 0xff) {
		offset++;
		if (val == 0x0f) {
			printf("pwup complete test\n");
		} else {
			uint32_t or_mask = BIOS16(offset);
			offset+=2;
			printf("rmw SDRAM mask %x %x\n", or_mask, val);
		}
		val = BIOS8(offset);
	}
}