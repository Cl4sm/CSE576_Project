static void radeon_rom_legacy_mmio_table(unsigned char *bios, int offset)
{
	if (!offset)
		return;

	while (BIOS16(offset)) {
		uint16_t cmd = (BIOS16(offset) & 0xe000) >> 13;
		uint32_t addr = BIOS16(offset) & 0x1fff;
		uint32_t val, and_mask, or_mask, tmp;

		offset+=2;
		switch(cmd) {
		case 0:
		case 1:
			val = BIOS32(offset);
			offset += 4;
			printf("write reg %x %x\n", addr, val);
			break;
		case 2:
		case 3:
			and_mask = BIOS32(offset);
			offset += 4;
			or_mask = BIOS32(offset);
			offset += 4;
			printf("write reg %x and %x or %x val\n",
			       addr, and_mask, or_mask);
			break;
		case 4:
			val = BIOS16(offset);
			offset += 2;
			printf("delay %x\n", val);
			break;
		case 5:
			val = BIOS16(offset);
			offset += 2;
			switch (addr) {
			case 8:
				printf("wait for CLK_PWRMGR CNTL MC busy %x\n", val);
				break;
			case 9:
				printf("wait for MC_STATUS MC idle %x\n", val);
				break;
			}
			break;
		default:
			printf("unknown\n");
		}
	}
}