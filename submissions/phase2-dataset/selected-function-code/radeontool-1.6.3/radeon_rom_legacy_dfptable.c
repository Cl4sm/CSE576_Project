static void radeon_rom_legacy_dfptable(unsigned char *bios, int hdr)
{
    int offset, i, n, rev, stride;

    offset = BIOS16(hdr + 0x34);
    if (offset == 0) {
        printf("No DFP info table\n");
        return;
    }

    rev = BIOS8(offset);
    printf("DFP table revision: %d\n", rev);

    switch(rev) {
    case 3:
        n = BIOS8(offset + 5) + 1;
        if (n > 4)
            n = 4;
        for (i = 0; i < n; i++) {
            /* Looks weird ... but that's what is in X.org */
            printf("  PixClock: %f\t TMDS_PLL_CNTL: %08x\n",
                   BIOS16(offset+i*10+0x10) / 100.0,
                   BIOS32(offset+i*10+0x08));
        }
        break;

    /* revision 4 has some problem as it appears in RV280...
     */
    case 4:
        stride = 0;
        n = BIOS8(offset+ 5) + 1;
        if (n > 4)
            n = 4;
        for (i = 0; i < n; i++) {
            printf("  PixClock: %f\t TMDS_PLL_CNTL: %08x\n",
                   BIOS16(offset+stride+0x10) / 100.0,
                   BIOS32(offset+stride+0x08));
            if (i == 0)
                stride += 10;
            else
                stride += 6;
        }
        break;
    }
}