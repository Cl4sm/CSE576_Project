static void radeon_rom_legacy_connectors(unsigned char *bios, int hdr)
{
    int offset = BIOS16(hdr + 0x50);
    int i, entry, tmp, chips, entries;

    if (offset == 0) {
        printf("No connector table in BIOS\n");
        return;
    }

    printf("Connector table:\n");

#if 0
    printf("  raw: %02x %02x %02x %02x %02x %02x %02x %02x\n",
           BIOS8(offset+0), BIOS8(offset+1), BIOS8(offset+2),
           BIOS8(offset+3), BIOS8(offset+4), BIOS8(offset+5),
           BIOS8(offset+6), BIOS8(offset+7));
#endif

    chips = BIOS8(offset) >> 4; 
    printf("  Table revision %d for %d chip(s)\n",
           BIOS8(offset) & 0xf, chips);
    if (chips > 1)
        printf("  Only 1 chip supported for now !\n");
    entries = BIOS8(offset + 1) & 0xf;
    printf("  Table for chip %d has %d connector(s):\n",
           BIOS8(offset + 1) >> 4, entries);

    for (i = 0; i < 4; i++) {
        entry = offset + 2 + i*2;

        /* End of table */
        if (!BIOS16(entry)) {
            if (i < entries)
                printf("    <table early termination !>\n");
            break;
        }

        /* Read table entry, check connector type */
        tmp = BIOS16(entry);
        printf("    %08x  ", tmp);
        printf("Type: %s", radeon_valname(lconn_type_name,
                         (tmp >> 12) & 0xf));
        printf(", DDC: %s", radeon_valname(lddc_type_name,
                          (tmp >> 8) & 0xf));
        printf(", DAC: %s", radeon_valname(ldac_type_name, tmp & 0x3));
        printf(", TMDS: %s", (tmp & 0x10) ? "External" : "Internal");
        
        printf("\n");
    }
    printf("\n");
}