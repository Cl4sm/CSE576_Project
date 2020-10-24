static void radeon_rom_legacy_crttable(unsigned char *bios, int hdr)
{
	int offset, rev, dac2table, dac2muxreg, bgadjcrt1, dacwhitecrt1, bgadjcrt2, dacadjcrt2, temp;
	
	offset = BIOS16(hdr + 0x60);
	if (offset == 0) {
		printf("No CRT info table\n");
		return;
	}
	rev = BIOS8(offset) & 0x3;
	printf("\nCRT table revision: %d\n", rev);

	printf("\nCRT table size: %d bytes\n", BIOS8(offset+1));

	if (rev == 2) {

	    bgadjcrt1 = BIOS8(offset + 0x2);
	    dacwhitecrt1 = BIOS8(offset + 0x3);
	    printf("crt1: 0x%x, 0x%x\n", bgadjcrt1, dacwhitecrt1);

	    bgadjcrt2 = BIOS8(offset + 0x4);
	    dacadjcrt2 = BIOS8(offset + 0x5);
	    printf("crt2: 0x%x, 0x%x\n", bgadjcrt2, dacadjcrt2);

	    dac2table = BIOS16(offset+8); 

	} else {

	    bgadjcrt1 = BIOS8(offset + 0x2) & 0xf;
	    dacwhitecrt1 = (BIOS8(offset + 0x2) >> 4) & 0xf;
	    printf("crt1: 0x%x, 0x%x\n", bgadjcrt1, dacwhitecrt1);

	    bgadjcrt2 = BIOS8(offset + 0x3) & 0xf;
	    dacadjcrt2 = (BIOS8(offset + 0x3) >> 4) & 0xf;
	    printf("crt2: 0x%x, 0x%x\n", bgadjcrt2, dacadjcrt2);

	    dac2table = BIOS16(offset + 0x5); 
	}

	printf("reg val 0x%x\n", (bgadjcrt2 << 16) | (dacadjcrt2 << 20));

	if (dac2table) {
	    printf("\nDac2 table: 0x%x\n", dac2table);
	    printf("revision: %d\n", (BIOS16(dac2table) & 0x3));
	    dac2muxreg = BIOS16(dac2table + 0x2);
	    printf("\nmux reg: 0x%x\n", dac2muxreg);
	    temp = BIOS8(dac2table + 0x4);
	    if (temp & 0x80)
		printf("CRT2 active high\n");
	    else
		printf("CRT2 active low\n");

	    printf("pin detection bits: 0x%x\n", (temp & 0x1f));
	    printf("access mode: 0x%x\n", ((temp >> 5) & 0x3));

	}

}