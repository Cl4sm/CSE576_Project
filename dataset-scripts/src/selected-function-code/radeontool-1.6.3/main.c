int main(int argc, char *argv[])
{
    int ret;
    if (argc == 1)
        usage();

    skip = 0;
    if (argc > 1 && strncmp(argv[1], "--skip=", 7) == 0) {
        skip = atoi(argv[1] + 7);
        argv++;
        argc--;
    }

    ret = map_radeon_mem();
    if (ret!=0)
	    die("Unable to see card");

    if (argc == 2) {
	    if (strcmp(argv[1], "regs") == 0) {
		    radeon_cmd_regs("default");
		    return 0;
	    }
    } else if (argc == 3) {
	    if (strcmp(argv[1], "regmatch") == 0) {
		    radeon_reg_match(argv[2]);
		    return 0;
	    } else if (strcmp(argv[1], "regs") == 0) {
		    radeon_cmd_regs(argv[2]);
		    return 0;
	    }

    } else if (argc == 4) {
        if (strcmp(argv[1], "regset") == 0) {
            radeon_reg_set(argv[2], strtoul(argv[3], NULL, 0));
            return 0;
        }
    }

    usage();

    pci_system_cleanup();

    return 1;
}