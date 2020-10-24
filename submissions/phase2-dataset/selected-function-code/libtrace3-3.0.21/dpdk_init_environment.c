static inline int dpdk_init_environment(char * uridata, struct dpdk_format_data_t * format_data,
                                        char * err, int errlen) {
    int ret; /* Returned error codes */
    struct rte_pci_addr use_addr; /* The only address that we don't blacklist */
    char cpu_number[10] = {0}; /* The CPU mask we want to bind to */
    char mem_map[20] = {0}; /* The memory name */
    long nb_cpu; /* The number of CPUs in the system */
    long my_cpu; /* The CPU number we want to bind to */
	struct saved_getopts save_opts;
    
#if DEBUG
    rte_set_log_level(RTE_LOG_DEBUG);
#else 
    rte_set_log_level(RTE_LOG_WARNING);
#endif
    /*
     * Using unique file prefixes mean separate memory is used, unlinking
     * the two processes. However be careful we still cannot access a 
     * port that already in use.
     */
    char* argv[] = {"libtrace", "-c", cpu_number, "-n", "1", "--proc-type", "auto",
		"--file-prefix", mem_map, "-m", "256", NULL};
    int argc = sizeof(argv) / sizeof(argv[0]) - 1;

    /* This initialises the Environment Abstraction Layer (EAL)
     * If we had slave workers these are put into WAITING state
     * 
     * Basically binds this thread to a fixed core, which we choose as
     * the last core on the machine (assuming fewer interrupts mapped here).
     * "-c" controls the cpu mask 0x1=1st core 0x2=2nd 0x4=3rd and so on
     * "-n" the number of memory channels into the CPU (hardware specific)
     *      - Most likely to be half the number of ram slots in your machine.
     *        We could count ram slots by "dmidecode -t 17 | grep -c 'Size:'"
     * Controls where in memory packets are stored and should spread across
     * the channels. We just use 1 to be safe.
     */

    /* Get the number of cpu cores in the system and use the last core */
    nb_cpu = sysconf(_SC_NPROCESSORS_ONLN);
    if (nb_cpu <= 0) {
        perror("sysconf(_SC_NPROCESSORS_ONLN) failed. Falling back to the first core.");
        nb_cpu = 1; /* fallback to the first core */
    }
    if (nb_cpu > RTE_MAX_LCORE)
        nb_cpu = RTE_MAX_LCORE;

    my_cpu = nb_cpu;
    /* This allows the user to specify the core - we would try to do this 
     * automatically but it's hard to tell that this is secondary
     * before running rte_eal_init(...). Currently we are limited to 1 
     * instance per core due to the way memory is allocated. */
    if (parse_pciaddr(uridata, &use_addr, &my_cpu) != 0) {
        snprintf(err, errlen, "Failed to parse URI");
        return -1;
    }

    snprintf(format_data->mempool_name, MEMPOOL_NAME_LEN,
                "libtrace_pool_%"PRIu32, (uint32_t) nb_cpu);

    if (!(my_cpu > 0 && my_cpu <= nb_cpu)) {
        snprintf(err, errlen, 
          "Intel DPDK - User defined a bad CPU number %"PRIu32" must be"
          " between 1 and %"PRIu32, (uint32_t) my_cpu, (uint32_t) nb_cpu);
        return -1;
    }

    /* Make our mask */
    snprintf(cpu_number, sizeof(cpu_number), "%x", 0x1 << (my_cpu - 1));


	/* Give the memory map a unique name */
	snprintf(mem_map, sizeof(mem_map), "libtrace-%d", (int) getpid());
    /* rte_eal_init it makes a call to getopt so we need to reset the 
     * global optind variable of getopt otherwise this fails */
	save_getopts(&save_opts);
    optind = 1;
    if ((ret = rte_eal_init(argc, argv)) < 0) {
        snprintf(err, errlen, 
          "Intel DPDK - Initialisation of EAL failed: %s", strerror(-ret));
        return -1;
    }
	restore_getopts(&save_opts);

#if DEBUG
    dump_configuration();
#endif

#if DPDK_USE_PMD_INIT
    /* This registers all available NICs with Intel DPDK
     * These are not loaded until rte_eal_pci_probe() is called.
     */
    if ((ret = rte_pmd_init_all()) < 0) {
        snprintf(err, errlen, 
          "Intel DPDK - rte_pmd_init_all failed: %s", strerror(-ret));
        return -1;
    }
#endif

    /* Blacklist all ports besides the one that we want to use */
	if ((ret = blacklist_devices(format_data, &use_addr)) < 0) {
		snprintf(err, errlen, "Intel DPDK - Whitelisting PCI device failed,"
		         " are you sure the address is correct?: %s", strerror(-ret));
		return -1;
	}

    /* This loads DPDK drivers against all ports that are not blacklisted */
	if ((ret = rte_eal_pci_probe()) < 0) {
        snprintf(err, errlen, 
            "Intel DPDK - rte_eal_pci_probe failed: %s", strerror(-ret));
        return -1;
    }

    format_data->nb_ports = rte_eth_dev_count();

    if (format_data->nb_ports != 1) {
        snprintf(err, errlen, 
            "Intel DPDK - rte_eth_dev_count returned %d but it should be 1",
            format_data->nb_ports);
        return -1;
    }

    return 0;
}