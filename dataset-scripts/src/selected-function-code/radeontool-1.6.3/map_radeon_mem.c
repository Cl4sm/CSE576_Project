static int map_radeon_mem(void)
{
#if 0
    struct pci_id_match match;
#else
    struct pci_slot_match match;
#endif
    struct pci_device_iterator *iter;
    struct pci_device *device;
    pciaddr_t fb_base, fb_size, ctrl_base, ctrl_size;
    int i = 0, ret;

    ret = pci_system_init();
    if (ret)
        die_error(ret, "failed to initialise libpciaccess");

#if 0
    match.vendor_id = 0x1002;
    match.device_id = PCI_MATCH_ANY;
    match.subvendor_id = PCI_MATCH_ANY;
    match.subdevice_id = PCI_MATCH_ANY;
    match.device_class = (0x03 << 16);
    match.device_class_mask = 0x00ff0000;
    match.match_data = 0;
    iter = pci_id_match_iterator_create(&match);
#else
    match.domain = PCI_MATCH_ANY;
    match.bus = PCI_MATCH_ANY;
    match.dev = PCI_MATCH_ANY;

    match.func = 0;
    match.match_data = 0;
    iter = pci_slot_match_iterator_create(&match);
#endif

    while ((device = pci_device_next(iter))) {
        pci_device_probe(device);
        if (device->vendor_id != 0x1002)
            continue;
        if ((device->device_class & 0x00ffff00) != 0x00030000 &&
            (device->device_class & 0x00ffff00) != 0x00038000)
            continue;
        if (skip--)
            continue;
        break;
    }

    if (!device) {
        printf("cannot find Radeon device\n");
        return -1;
    }

    for (i = 0; i < sizeof(RADEONCards) / sizeof(RADEONCardInfo); i++) {
        if (RADEONCards[i].pci_device_id == device->device_id)
            card_info = &RADEONCards[i];
    }

    ctrl_base = device->regions[2].base_addr;
    ctrl_size = device->regions[2].size;
    if (!ctrl_size)
        die("missing ctrl region");
    ret = pci_device_map_range(device, ctrl_base, ctrl_size,
			     PCI_DEV_MAP_FLAG_WRITABLE, (void **) &ctrl_mem);
    if (ret)
        die_error(ret, "cannot map ctrl region");

    fb_base = device->regions[0].base_addr;
    fb_size = device->regions[0].size;
    if (!fb_size || pci_device_map_range(device, fb_base, fb_size,
			     PCI_DEV_MAP_FLAG_WRITABLE, (void **) &fb_mem))
        fb_mem = NULL;

    pci_iterator_destroy(iter);

    return 0;
}