static int blacklist_devices(struct dpdk_format_data_t *format_data, struct rte_pci_addr *whitelist)
{
	struct rte_pci_device *dev = NULL;
	format_data->nb_blacklist = 0;

	memset(format_data->blacklist, 0, sizeof (format_data->blacklist));

	TAILQ_FOREACH(dev, &device_list, next) {
        if (whitelist != NULL && whitelist->domain == dev->addr.domain 
            && whitelist->bus == dev->addr.bus
            && whitelist->devid == dev->addr.devid
            && whitelist->function == dev->addr.function)
            continue;
		if (format_data->nb_blacklist >= sizeof (format_data->blacklist)
                                / sizeof (format_data->blacklist[0])) {
			printf("Warning: too many devices to blacklist consider"
                                        " increasing BLACK_LIST_SIZE");
			break;
		}
		format_data->blacklist[format_data->nb_blacklist] = dev->addr;
		++format_data->nb_blacklist;
	}

	rte_eal_pci_set_blacklist(format_data->blacklist, format_data->nb_blacklist);
	return 0;
}