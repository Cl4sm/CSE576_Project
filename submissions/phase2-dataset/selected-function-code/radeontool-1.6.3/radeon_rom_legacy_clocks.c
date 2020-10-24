static void radeon_rom_legacy_clocks(unsigned char *bios, int hdr)
{
	int pll_info_block = BIOS16(hdr + 0x30);
	int rev;

	if (pll_info_block == 0) {
		printf("No clock info block in BIOS\n");
		return;
	}
	rev = BIOS8(pll_info_block);
	printf("revision %d\n", rev);

	printf("Clock info block:\n");
	printf("  SCLK   : %f\n", BIOS16(pll_info_block + 0x0a) / 100.0);
	printf("  MCLK   : %f\n", BIOS16(pll_info_block + 0x08) / 100.0);
	printf("  RefClk : %f\n", BIOS16(pll_info_block + 0x0e) / 100.0);
	printf("  RefDiv : %d\n", BIOS16(pll_info_block + 0x10));
	printf("  VCO Min: %f\n", BIOS32(pll_info_block + 0x12) / 100.0);
	printf("  VCO Max: %f\n", BIOS32(pll_info_block + 0x16) / 100.0);

	if (rev > 9) {
	    printf("  VCO in Min: %f\n", BIOS32(pll_info_block + 0x36) / 100.0);
	    printf("  VCO in Max: %f\n", BIOS32(pll_info_block + 0x3a) / 100.0);
	    printf("  VCO out Min: %f\n", BIOS32(pll_info_block + 0x3e) / 100.0);
	    printf("  VCO out Max: %f\n", BIOS32(pll_info_block + 0x42) / 100.0);
	}

	printf("  SPLL RefClk  : %f\n", BIOS16(pll_info_block + 0x1a) / 100.0);
	printf("  SPLL ref div  : %d\n", BIOS16(pll_info_block + 0x1c));
	printf("  VCO Min: %f\n", BIOS32(pll_info_block + 0x1e) / 100.0);
	printf("  VCO Max: %f\n", BIOS32(pll_info_block + 0x22) / 100.0);

	printf("  MPLL RefClk  : %f\n", BIOS16(pll_info_block + 0x26) / 100.0);
	printf("  MPLL ref div  : %d\n", BIOS16(pll_info_block + 0x28));
	printf("  VCO Min: %f\n", BIOS32(pll_info_block + 0x2a) / 100.0);
	printf("  VCO Max: %f\n", BIOS32(pll_info_block + 0x2e) / 100.0);

	printf("\n");
}