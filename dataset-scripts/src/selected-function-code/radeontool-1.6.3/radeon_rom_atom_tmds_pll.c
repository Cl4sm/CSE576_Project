static void radeon_rom_atom_tmds_pll(unsigned char *bios, int master)
{
    int offset, tmp, tmp0;
    int i;

    offset = BIOS16(master + 18);
    if (offset) {
        printf("TMDS PLLs:\n");
        /* As far as I can tell, these are in hecto Hertz (i.e. e2).
         * Yes, this is weird. */
        tmp = BIOS16(offset + 4);
        printf("Maximum frequency: %dHz\n", tmp * 10);
        
        for (i = 0; i < 4; i++) {
            tmp = BIOS16(offset + (i * 6) + 6);
            tmp0 = (BIOS8(offset + (i * 8) + 8) & 0x3f) |
                   ((BIOS8(offset + (i * 8) + 10) & 0x3f) << 6) |
                   ((BIOS8(offset + (i * 8) + 9) & 0xf) << 12) |
                   ((BIOS8(offset + (i * 8) + 11) & 0xf) << 16);
            printf("    %d: %dHz %x\n", i, tmp * 10, tmp0);
        }
    }
    else {
        printf("No TMDS PLLs\n");
    }
}