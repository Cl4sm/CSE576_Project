static void radeon_rom_legacy_componenttv_table(unsigned char *bios, int hdr)
{
    int offset, rev, temp;

	offset = BIOS16(hdr + 0x62);
	if (offset == 0) {
		printf("No component tv info table\n");
		return;
	}
	rev = BIOS8(offset);
	printf("\ncomponent tv table revision: %d\n", rev);

	if (rev < 3) {
	    if (BIOS8(offset + 0x2) & 0x4)
		printf("CRT2 Mux used as well to switch to CV\n");
	    if (BIOS8(offset + 0x2) & 0x8)
		printf("Low voltage TV/CV MUX control\n");
	} else {
	    if (BIOS8(offset + 0x2) & 0x10)
		printf("enable access for i2c D-Connect Support\n");
	}
	printf("GPIO register for detection: 0x%x \n", BIOS16(offset + 0x3));
	temp = BIOS8(offset + 0x5);
	if (temp & 0x80)
	    printf("Active high\n");
	else
	    printf("Active low\n");
	printf("GPIO TV/HDTV MUX control register: 0x%x \n", BIOS16(offset + 0x6));
	temp = BIOS8(offset + 0x8);
	if (temp & 0x80)
	    printf("Active high\n");
	else
	    printf("Active low\n");

}