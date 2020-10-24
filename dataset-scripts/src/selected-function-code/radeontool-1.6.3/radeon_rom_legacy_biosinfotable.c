static void radeon_rom_legacy_biosinfotable(unsigned char *bios, int hdr)
{
    int offset, i;
    char strp[4];

    offset = BIOS16(hdr + 0x14);
    if (offset == 0) {
	printf("bios info table\n");
	return;
    }
    printf("bios info table\n");
    for (i = 0; i < 4; i++)
	strp[i] = BIOS8(offset + i);
    printf("chip type: %s\n", strp);
    for (i = 0; i < 4; i++)
	strp[i] = BIOS32(offset + 4 + i);
    printf("bus type: %s\n", strp);
    for (i = 0; i < 4; i++)
	strp[i] = BIOS32(offset + 8 + i);
    printf("mem type: %s\n", strp);


}