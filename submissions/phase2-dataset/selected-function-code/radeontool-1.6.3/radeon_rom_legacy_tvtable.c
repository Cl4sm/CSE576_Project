static void radeon_rom_legacy_tvtable(unsigned char *bios, int hdr)
{
	int offset, rev, refclk, drvstds, biosstds;
	
	offset = BIOS16(hdr + 0x32);
	if (offset == 0) {
		printf("No TV info table\n");
		return;
	}
	rev = BIOS8(offset + 3);
	printf("\nTV table revision: %d\n", rev);


	printf("\nTV table size: %d\n", BIOS16(offset + 4));

	if (BIOS8(offset + 6) == 'T')
	    printf("TV out on board\n");
	else
	    printf("No TV out on board\n");

	printf("Default Standard: ");
	switch (BIOS8(offset + 7)) {
	case 1:
	    printf("NTSC\n");
	    break;
	case 2:
	    printf("PAL\n");
	    break;
	case 3:
	    printf("PAL-M\n");
	    break;
	case 4:
	    printf("PAL-60\n");
	    break;
	case 5:
	    printf("NTSC-J\n");
	    break;
	case 6:
	    printf("SCART-PAL\n");
	    break;
	default:
	    printf("Unknown standard\n");
            break;
	}

	printf("TV Ref Clk: ");
	refclk = BIOS8(offset + 9) >> 2;
	if (refclk == 0)
	    printf("29.498928713 MHz\n");
	else if (refclk == 1)
	    printf("28.636360000 MHz\n");
	else if (refclk == 2)
	    printf("14.318180000 MHz\n");
	else if (refclk == 3)
	    printf("27.000000000 MHz\n");

	printf("TV standards supported by driver: ");
	drvstds = BIOS8(offset + 10);
	if (drvstds & 1)
	    printf("NTSC ");
	if (drvstds & 2)
	    printf("PAL ");
	if (drvstds & 4)
	    printf("PAL-M ");
	if (drvstds & 8)
	    printf("PAL-60 ");
	if (drvstds & 16)
	    printf("NTSC-J ");
	if (drvstds & 32)
	    printf("SCART-PAL");
	printf("\n");

	printf("TV standards supported by bios: ");
	biosstds = BIOS8(offset + 11);
	if (biosstds & 1)
	    printf("NTSC ");
	if (biosstds & 2)
	    printf("PAL ");
	if (biosstds & 4)
	    printf("PAL-M ");
	if (biosstds & 8)
	    printf("PAL-60 ");
	if (biosstds & 16)
	    printf("NTSC-J ");
	if (biosstds & 32)
	    printf("SCART-PAL");
	printf("\n");

	if (rev >= 5) {
	    int temp, temp2;

	    temp = BIOS8(offset + 0xc);
	    temp2 = BIOS8(offset + 0xd);
	    printf("crt2 ps2: 0x%x, 0x%x\n", temp, temp2);

	    temp = BIOS8(offset + 0xe);
	    temp2 = BIOS8(offset + 0xf);
	    printf("crt2 pal: 0x%x, 0x%x\n", temp, temp2);

	    temp = BIOS8(offset + 0x10);
	    temp2 = BIOS8(offset + 0x11);
	    printf("crt2 ntsc: 0x%x, 0x%x\n", temp, temp2);

	    temp = BIOS16(offset + 0x12);
	    printf("gpio reg: 0x%x\n", temp);

	    temp = BIOS16(offset + 0x14);
	    printf("gpio AND mask: 0x%x\n", temp);

	    temp = BIOS16(offset + 0x18);
	    printf("gpio OR mask: 0x%x\n", temp);

	} else if (rev >= 2) {
	    int temp, temp2;

	    temp = BIOS8(offset + 0xc) & 0xf;
	    temp2 = (BIOS8(offset + 0xc) >> 4) & 0xf;
	    printf("crt2 ps2: 0x%x, 0x%x\n", temp, temp2);

	    temp = BIOS8(offset + 0xd) & 0xf;
	    temp2 = (BIOS8(offset + 0xd) >> 4) & 0xf;
	    printf("crt2 pal: 0x%x, 0x%x\n", temp, temp2);

	    temp = BIOS8(offset + 0xe) & 0xf;
	    temp2 = (BIOS8(offset + 0xe) >> 4) & 0xf;
	    printf("crt2 ntsc: 0x%x, 0x%x\n", temp, temp2);

	    if (rev > 2) {
		temp = BIOS8(offset + 0x10);
		printf("gpio bit: 0x%x\n", temp);
	    }

	    if (rev > 3) {
		temp = BIOS16(offset + 0x11);
		printf("gpio reg: 0x%x\n", temp);
	    }

	}


}