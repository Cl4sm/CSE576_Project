static void radeon_rom_legacy_miscinfotable(unsigned char *bios, int hdr)
{
    int offset, rev, start, size;

    offset = BIOS16(hdr + 0x5e);
    if (offset == 0) {
	printf("No misc info table\n");
	return;
    }
    rev = BIOS8(offset);
    printf("misc info table revision: %d\n", rev);
    if (rev > 0) {
	size = BIOS8(offset + 2);
	printf("misc info table size: %d bytes\n", size);

    }
    start = BIOS8(offset + 1);
    if (start & 0x1)
	printf("dualview supported\n");
    if (rev == 8)
	if (start & 0x8)
	    printf("sleep mode enabled in perfomance mode\n");
    if (start & 0x20)
	printf("hypermemory flag\n");
    if (rev == 5) {
	if (start & 0x40)
	    printf("sleep mode enabled in perfomance mode\n");
    } else if (rev == 8) {
	if (start & 0x40)
	    printf("power state load balancing is enabled in perfomance mode\n");
    }
    if (rev == 6)
	if (start & 0x80)
	    printf("WMI thermal method supported\n");
    if (rev > 0) {
        if (BIOS16(offset + 0x3)) {
	    printf("asic init table 3 offset: 0x%x\n", BIOS16(offset + 0x3));
	    radeon_rom_legacy_mmio_table(bios, BIOS16(offset + 0x3));
	}
	if (BIOS16(offset + 0x5)) {
	    printf("asic init table 4 offset: 0x%x\n", BIOS16(offset + 0x5));
	    radeon_rom_legacy_mmio_table(bios, BIOS16(offset + 0x5));
	}
	if (BIOS16(offset + 0x7))
	    printf("detected mem config table offset: 0x%x\n", BIOS16(offset + 0x7));
	if (rev == 2) {
	    if (BIOS16(offset + 0x9))
		printf("asic init table 5 offset: 0x%x\n", BIOS16(offset + 0x9));
	    if (BIOS16(offset + 0xb))
		printf("memreset sequence table offset: 0x%x\n", BIOS16(offset + 0xb));
	} else if ((rev == 4) || (rev == 5) || (rev == 6)) {
	    if (BIOS16(offset + 0x9))
		printf("pointer to asic init table 5 offset list: 0x%x\n", BIOS16(offset + 0x9));
	    if (BIOS16(offset + 0xb))
		printf("pointer to memreset sequence table offset list: 0x%x\n", BIOS16(offset + 0xb));
	}
	if ((rev == 5) || (rev == 6)) {
	    if (BIOS16(offset + 0xd))
		printf("mem info table offset: 0x%x\n", BIOS16(offset + 0xd));
	    printf("min backlight value: 0x%x\n", BIOS8(offset + 0xf));
	}
	if (rev == 7) {
	    if (BIOS16(offset + 0x7))
		printf("pointer to detected mem config table offset list: 0x%x\n", BIOS16(offset + 0x7));
	    if (BIOS16(offset + 0x9))
		printf("pointer to asic init table 5 offset list: 0x%x\n", BIOS16(offset + 0x9));
	    if (BIOS16(offset + 0xb))
		printf("pointer to memreset sequence table offset list: 0x%x\n", BIOS16(offset + 0xb));
	}
	if (rev >= 6) {
	    int flags = BIOS8(offset + 0x10);
	    if (flags & 0x1)
		printf("driver controls brightness\n");
	    if (flags & 0x2)
		printf("bios controls powerplay\n");
	    if (flags & 0x4)
		printf("use new mem reset sequence\n");
	    if (flags & 0x8)
		printf("power state load balancing is disabled in performance mode\n");
	    if (rev == 9)
		if (flags & 0x40)
		    printf("PCIE to PCI bridge present\n");
	}

    }
}