static void map_radeon_cntl_mem(void)
{
    struct pci_slot_match match;
    struct pci_device_iterator *iter;
    struct pci_device *device;
    pciaddr_t fb_base, fb_size, ctrl_base, ctrl_size;
    int i = 0, ret;

    ret = pci_system_init();
    if (ret) {
        fprintf(stderr, "error: failed to initialise libpciaccess: %s\n",
		strerror(ret));
        return;
    }

    match.domain = PCI_MATCH_ANY;
    match.bus = PCI_MATCH_ANY;
    match.dev = PCI_MATCH_ANY;

    match.func = 0;
    match.match_data = 0;
    iter = pci_slot_match_iterator_create(&match);

    while ((device = pci_device_next(iter))) {
        pci_device_probe(device);
        if (device->vendor_id != 0x1002)
            continue;
        if ((device->device_class & 0x00ffff00) != 0x00030000)
            continue;

        if (debug)
            printf("Found card %x:%x (%x)\n", device->vendor_id,
                   device->device_id, device->device_class);
        if (skip--)
            continue;
        break;
    }

    if (!device) {
        fprintf(stderr, "error: cannot find Radeon device\n");
        return;
    }

    ctrl_base = device->regions[2].base_addr;
    ctrl_size = device->regions[2].size;
    if (!ctrl_size) {
        fprintf(stderr, "error: missing ctrl region\n");
        return;
    }
    ret = pci_device_map_range(device, ctrl_base, ctrl_size,
			     PCI_DEV_MAP_FLAG_WRITABLE, (void **) &ctrl_mem);
    if (ret) {
        fprintf(stderr, "cannot map ctrl region: %s\n", strerror(ret));
        return;
    }

    fb_base = device->regions[0].base_addr;
    fb_size = device->regions[0].size;
    if (!fb_size || pci_device_map_range(device, fb_base, fb_size,
			     PCI_DEV_MAP_FLAG_WRITABLE, (void **) &fb_mem))
        fb_mem = NULL;

    pci_iterator_destroy(iter);

    if (debug)
        printf("Radeon found. Base control address is %lx; "
               "base framebuffer address is %lx.\n",
               (unsigned long) ctrl_mem, (unsigned long) fb_mem);

    if (!ctrl_mem)
        die("internal error");
    radeon_cntl_mem = ctrl_mem;
}