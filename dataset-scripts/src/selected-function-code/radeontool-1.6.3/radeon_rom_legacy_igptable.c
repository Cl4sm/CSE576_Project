static void radeon_rom_legacy_igptable(unsigned char *bios, int hdr)
{
    int offset, rev, fsb;

	offset = BIOS16(hdr + 0x62);
	if (offset == 0) {
		printf("No igp info table\n");
		return;
	}
	rev = BIOS8(offset);
	printf("\nigp table revision: %d\n", rev);

	fsb = BIOS16(offset + 0x2);
	if (rev < 2)
	    fsb *= 100;
	printf("FSB: %f Mhz\n", fsb);

	printf("mclk reference frequency: %f Mhz\n", BIOS16(offset + 0x4) / 100.00);
	printf("mclk reference divider: %d \n", BIOS16(offset + 0x6));
	printf("memory refresh period from SPD: %d \n", BIOS8(offset + 0x8));
	if (rev > 0)
	    printf("min mclk: %f \n", BIOS16(offset + 0xd) / 100.00);
	if (rev > 1)
	    printf("max mclk: %f \n", BIOS16(offset + 0xf) / 100.00);
	if (rev > 2) {
	    printf("K8 mclk: %f \n", BIOS16(offset + 0x11));
	    printf("K8 sync start delay: %f us\n", BIOS16(offset + 0x13) / 100);
	    printf("K8 data return time: %f us\n", BIOS16(offset + 0x15) / 100);
	}
	if (rev > 3) {
	    if (BIOS16(offset + 0x17) & 1)
		printf("PCIE lane reserved\n");
	    else
		printf("PCIE lane not reserved\n");
	}
}