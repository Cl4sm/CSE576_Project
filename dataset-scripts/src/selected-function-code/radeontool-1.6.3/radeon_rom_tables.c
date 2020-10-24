void radeon_rom_tables(const char * file)
{
#define _64K (64*1024)
    unsigned char bios[_64K];
    char *biosmem;
    int fd, hdr, atom;

    if (strcmp(file, "mmap") == 0) {
        fd = open("/dev/mem", O_RDWR);
        biosmem = mmap(0, _64K, PROT_READ, MAP_SHARED, fd, 0xc0000);
        if (biosmem == MAP_FAILED) {
            perror("can't mmap bios");
            return;
        }
        memset(bios, 0, _64K);
        memcpy(bios, biosmem, _64K);
        munmap(biosmem, _64K);
        close(fd);
    }
    else {
        fd = open(file, O_RDONLY);
        if (fd < 0) {
            perror("can't open rom file");
            return;
        }
        memset(bios, 0, _64K);
        read(fd, bios, _64K);
        close(fd);
    }

    if (bios[0] != 0x55 || bios[1] != 0xaa)
        die("PCI ROM signature 0x55 0xaa missing");

    hdr = BIOS16(0x48);
    printf("\nBIOS Tables:\n------------\n\n");	
    printf("Header at %x, type: %d [%s]\n", hdr, BIOS8(hdr),
           radeon_valname(hdr_type_name, BIOS8(hdr)));
    printf("OEM ID: %02x %02x\n", BIOS8(hdr+2), BIOS8(hdr+3));
    atom = (BIOS8(hdr+4)   == 'A' &&
        BIOS8(hdr+5) == 'T' &&
        BIOS8(hdr+6) == 'O' &&
        BIOS8(hdr+7) == 'M') ||
        (BIOS8(hdr+4)   == 'M' &&
         BIOS8(hdr+5) == 'O' &&
         BIOS8(hdr+6) == 'T' &&
         BIOS8(hdr+7) == 'A');

    if (atom) {
        int master = BIOS16(hdr+32);
        printf("ATOM BIOS detected !\n\n");
        radeon_rom_atom_clocks(bios, master);
        radeon_rom_atom_connectors(bios, master);
        radeon_rom_atom_tmds_pll(bios, master);
        radeon_rom_atom_lvds(bios, master);

	atom_ctx = atom_init(bios);
	atom_dump(atom_ctx);
        // add more ...
    }
    else {
        printf("Legacy BIOS detected !\n");
        printf("BIOS Rev: %x.%x\n\n", BIOS8(hdr+4), BIOS8(hdr+5));
        radeon_rom_legacy_clocks(bios, hdr);
        radeon_rom_legacy_connectors(bios, hdr);
        radeon_rom_legacy_dfptable(bios, hdr);
    }
}