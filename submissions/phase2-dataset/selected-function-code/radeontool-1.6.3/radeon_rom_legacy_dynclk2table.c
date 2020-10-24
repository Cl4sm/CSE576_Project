static void radeon_rom_legacy_dynclk2table(unsigned char *bios, int hdr)
{
    int offset, type, start, rev, i = 0;

    offset = BIOS16(hdr + 0x52);
    if (offset == 0) {
	printf("No dyn clock 2 info table\n");
	return;
    }
    rev = BIOS8(offset - 1);
    start = BIOS16(offset);
    printf("dyn clock 2 info table revision %d\n", rev);
    while (start != 0) {
	type = (start & 0xe000) >> 13;
	if (type == 0) {
	    unsigned int reg = BIOS16(offset + i) & 0x1fff;
	    unsigned int val = BIOS32(offset + i + 2);
	    printf("mmio reg: 0x%x, val: 0x%x\n", reg * 4, val);
	    i += 6;
	} else if (type == 1) {
	    unsigned int reg = BIOS16(offset + i) & 0x1fff;
	    unsigned int val = BIOS32(offset + i + 2);
	    printf("I/O reg: 0x%x, val: 0x%x\n", reg * 4, val);
	    i += 6;
	} else if (type == 2) {
	    unsigned int reg = BIOS16(offset + i) & 0x1fff;
	    unsigned int and_mask = BIOS32(offset + i + 2);
	    unsigned int or_mask = BIOS32(offset + i + 6);
	    printf("mmio reg: 0x%x, AND mask: 0x%x, OR mask: 0x%x\n",
		   reg * 4, and_mask, or_mask);
	    i += 10;
	} else if (type == 3) {
	    unsigned int reg = BIOS16(offset + i) & 0x1fff;
	    unsigned int and_mask = BIOS32(offset + i + 2);
	    unsigned int or_mask = BIOS32(offset + i + 6);
	    printf("I/O reg: 0x%x, AND mask: 0x%x, OR mask: 0x%x\n",
		   reg * 4, and_mask, or_mask);
	    i += 10;
	} else if (type == 4) {
	    printf("delay %d us\n", BIOS16(offset + i + 2));
	    i += 4;
	} else
	    break;
	start = BIOS16(offset + i);
    }
}