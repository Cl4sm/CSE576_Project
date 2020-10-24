static void radeon_rom_legacy_lcdtable(unsigned char *bios, int hdr)
{
	int offset, i, type_info, extended_offset;
	unsigned int setup;
	char stmp[30];

	offset = BIOS16(hdr + 0x40);
	if (offset == 0) {
		printf("No LCD info table\n");
		return;
	}
	printf("\nLCD info table:\n");
	printf("lcd panel id: %d\n", BIOS8(offset));
	for (i = 0; i < 24; i++)
	    stmp[i] = BIOS8(offset+i+1);
	stmp[24] = 0;
	printf("lcd panel id string: %s\n", stmp);
	printf("lcd panel x: %d\n", BIOS16(offset+0x19));
	printf("lcd panel y: %d\n", BIOS16(offset+0x1b));

	type_info = BIOS16(offset+0x1d);
	if (type_info & 0x1)
	    printf("color ");
	else
	    printf("mono ");

	if (type_info & 0x2)
	    printf("single ");
	else
	    printf("dual ");

	switch ((type_info & 0xfc) >> 2) {
	case 0:
	    printf("STN\n");
	    break;
	case 1:
	default:
	    printf("TFT\n");
	    break;
	case 2:
	    printf("active addressed STN\n");
	    break;
	case 3:
	    printf("EL\n");
	    break;
	case 4:
	    printf("Plasma\n");
	    break;
	}

	printf("lcd panel power delay: %d\n", BIOS16(offset+0x24));

	extended_offset = BIOS16(offset+0x26);
	if (extended_offset) {
	  int extended_rev;
		extended_rev = BIOS8(extended_offset);
		printf("lcd panel extended info offset: %x\n", extended_offset);
		printf("lcd extended info rev: %x\n", extended_rev);
		
		printf("SS delay %x step size %x\n", BIOS8(extended_offset+1),
		       BIOS8(extended_offset + 2));

		if (extended_rev > 1) {
			printf("lcd fake edid id num  bytes %d\n", BIOS8(extended_offset+3));
			printf("lcd fake edid id num offset 0x%x\n", BIOS16(extended_offset+4));
		}
		if (extended_rev > 2) {
			printf("lcd LVPVS %x\n", BIOS8(extended_offset+6));
			printf("lcd PVG/PCP %x\n", BIOS8(extended_offset+7));
			printf("lcd SSPLL table offset %x\n", BIOS16(extended_offset+8));
		}
		if (extended_rev > 3) {
			printf("lcd resp time coeff 0x%x\n", BIOS8(extended_offset+10));
		}
		if (extended_rev > 4) {
			printf("edid low refresh offset 0x%x\n", BIOS16(extended_offset+11));
		}
	}

	printf("lcd panel vcc delay: %d\n", BIOS16(offset+0x2c));
	printf("lcd panel ref div: %d\n", BIOS16(offset+0x2e));
	printf("lcd panel post div: %d\n", BIOS8(offset+0x30));
	printf("lcd panel fb div: %d\n", BIOS16(offset+0x31));
	printf("SS type: %s / %s\n", (BIOS8(offset + 0x33) & 1) ? "center" : "down",
	       (BIOS8(offset + 0x33) & (1<<7)) ? "internal" : "external");
	printf("SS percentage %x\n", BIOS16(offset + 0x34));
	printf("lcd panel DIGON to LVDS active: %d\n", BIOS8(offset+0x38) & 0xf);
	printf("lcd panel LVDS active to BLON: %d\n", (BIOS8(offset+0x38) >> 4) & 0xf);
	setup = BIOS32(offset+0x39);
	printf("lcd panel LVDS setup: 0x%x\n", setup);

	if (setup & 0x1)
	    printf("888 RGB\n");
	else
	    printf("666 RGB\n");

	switch ((setup >> 8) & 0x8) {
	case 0:
	    printf("no frame modulation\n");
	    break;
	case 1:
	    printf("2 levels of grey\n");
	    break;
	case 2:
	    printf("4 levels of grey\n");
	    break;
	default:
	    printf("STN panel\n");
	    break;
	}

	if ((setup >> 16) & 0x1)
	    printf("frame pulse high\n");
	else
	    printf("frame pulse low\n");

	if ((setup >> 17) & 0x1)
	    printf("line pulse high\n");
	else
	    printf("line pulse low\n");

	if ((setup >> 18) & 0x1)
	    printf("MOD high\n");
	else
	    printf("MOD low\n");

	if ((setup >> 23) & 0x1)
	    printf("backlight mod 29 Mhz\n");
	else
	    printf("backlight mod 29 Mhz / 3\n");
}