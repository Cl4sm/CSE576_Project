static void radeon_rom_legacy_list_tables_present(unsigned char *bios, int hdr)
{
    int offset;

    offset = BIOS16(hdr + 0xc);
    if (offset) {
	printf("asic init table 1\n");
    }
    offset = BIOS16(hdr + 0x14);
    if (offset) {
	printf("bios support info table\n");
    }
    offset = BIOS16(hdr + 0x2a);
    if (offset) {
	printf("dac programming info table\n");
    }
    offset = BIOS16(hdr + 0x2c);
    if (offset) {
	printf("max color depth info table\n");
    }
    offset = BIOS16(hdr + 0x2e);
    if (offset) {
	printf("crtc info table\n");
    }
    offset = BIOS16(hdr + 0x30);
    if (offset) {
	printf("pll info table\n");
    }
    offset = BIOS16(hdr + 0x32);
    if (offset) {
	printf("TV info table\n");
    }
    offset = BIOS16(hdr + 0x34);
    if (offset) {
	printf("DFP info table\n");
    }
    offset = BIOS16(hdr + 0x36);
    if (offset) {
	printf("HW config info table\n");
    }
    offset = BIOS16(hdr + 0x38);
    if (offset) {
	printf("multimedia info table\n");
    }
    offset = BIOS16(hdr + 0x3e);
    if (offset) {
	printf("tv standard patch table\n");
    }
    offset = BIOS16(hdr + 0x40);
    if (offset) {
	printf("LCD info table\n");
    }
    offset = BIOS16(hdr + 0x42);
    if (offset) {
	printf("Mobile info table\n");
    }
    offset = BIOS16(hdr + 0x46);
    if (offset) {
	printf("pll init info table\n");
    }
    offset = BIOS16(hdr + 0x48);
    if (offset) {
	int check_offset = offset;
	printf("mem config info table\n");
	while (BIOS8(check_offset++));
	check_offset += 2;
	if (check_offset)
		printf("mem ram reset table %d %x %x\n", check_offset, BIOS8(check_offset -2), BIOS8(check_offset - 1));
    }
    offset = BIOS16(hdr + 0x4a);
    if (offset) {
	printf("save mask info table\n");
    }
    offset = BIOS16(hdr + 0x4c);
    if (offset) {
	printf("Hardcoded DFP edid\n");
    }
    offset = BIOS16(hdr + 0x4e);
    if (offset) {
	printf("asic init table 2\n");
    }
    offset = BIOS16(hdr + 0x50);
    if (offset) {
	printf("connector info table\n");
    }
    offset = BIOS16(hdr + 0x52);
    if (offset) {
	printf("dyn clock 1 info table\n");
    }
    offset = BIOS16(hdr + 0x54);
    if (offset) {
	printf("reserved mem info table\n");
    }
    offset = BIOS16(hdr + 0x58);
    if (offset) {
	printf("Ext TMDS info table\n");
    }
    offset = BIOS16(hdr + 0x5a);
    if (offset) {
	printf("mem clock info table\n");
    }
    offset = BIOS16(hdr + 0x5c);
    if (offset) {
	printf("ext DAC info table\n");
    }
    offset = BIOS16(hdr + 0x5e);
    if (offset) {
	printf("misc info table\n");
    }
    offset = BIOS16(hdr + 0x60);
    if (offset) {
	printf("CRT info table\n");
    }
    offset = BIOS16(hdr + 0x62);
    if (offset) {
	printf("integrated system info table\n");
    }
    offset = BIOS16(hdr + 0x64);
    if (offset) {
	printf("Component video info table\n");
    }
    offset = BIOS16(hdr + 0x66);
    if (offset) {
	printf("fan speed info table\n");
    }
    offset = BIOS16(hdr + 0x68);
    if (offset) {
	printf("overdrive info table\n");
    }
    offset = BIOS16(hdr + 0x6a);
    if (offset) {
	printf("OEM info table\n");
    }
    offset = BIOS16(hdr + 0x6c);
    if (offset) {
	printf("dyn clock 2 info table\n");
    }
    offset = BIOS16(hdr + 0x6e);
    if (offset) {
	printf("power connector info table\n");
    }
    offset = BIOS16(hdr + 0x70);
    if (offset) {
	printf("i2c info table\n");
    }
    offset = BIOS16(hdr + 0x72);
    if (offset) {
	printf("object header info table\n\n");
    }
    printf("\n");
}