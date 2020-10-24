static void radeon_rom_atom_connectors(unsigned char *bios, int master)
{
    int offset = BIOS16(master + 22);
    int tmp, i, tmp0;
    int crtc, dac, connector, ddc=0;

    if (offset == 0) {
        printf("No connector table in BIOS\n");
        return;
    }

    tmp = BIOS16(offset + 4);
    printf("Connector table:\n");

    for (i = 0; i < 8; i++) {
        if (tmp & (1 << i)) {
            int gpio;
            int portinfo = BIOS16(offset + 6 + i * 2);

            crtc = (portinfo >> 8) & 0xf;
            dac = (portinfo & 0xf) - 1;
            connector = (portinfo >> 4) & 0xf;

            tmp0 = BIOS16(master + 24);
            if (1 /* crtc */) {
                gpio = BIOS16(tmp0 + 4 + 27 * crtc) * 4;
                switch(gpio)
                {
                case RADEON_GPIO_MONID: ddc = 1; break;
                case RADEON_GPIO_DVI_DDC: ddc = 2; break;
                case RADEON_GPIO_VGA_DDC: ddc = 3; break;
                case RADEON_GPIO_CRT2_DDC: ddc = 4; break;
                case AVIVO_GPIO_0: ddc = 7; break;
                case AVIVO_GPIO_1: ddc = 5; break;
                case AVIVO_GPIO_2: ddc = 6; break;
                case AVIVO_GPIO_3: ddc = 7; break;
                default: ddc = 0; break;
                }
            }

            printf("%d:    %08x ", i, portinfo);
            printf(", Id: %d", crtc);
            printf(", Type: %s",  radeon_valname(atomconn_type_name,
                                                 connector));
            if (1 /* crtc */)
                printf(", DDC: %s", radeon_valname(lddc_type_name, ddc));
            /* On AVIVO cards, the DAC is unset for TMDS */
            if (dac >= 0 || (i != 3 && i != 7))
                printf(", DAC: %s", radeon_valname(ldac_type_name, dac));
            printf(", GPIO: 0x%04X", gpio);
            if (i == 3)
                printf(" TMDS: Internal\n");
            else if (i == 7)
                printf(" TMDS: External\n");
            else
                printf("\n");
                
        }
    }
}