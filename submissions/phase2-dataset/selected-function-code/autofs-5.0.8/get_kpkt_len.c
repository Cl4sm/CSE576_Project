static size_t get_kpkt_len(void)
{
	size_t pkt_len = sizeof(struct autofs_v5_packet);
	struct utsname un;
	int kern_vers;

	kern_vers = linux_version_code();
	if (kern_vers >= KERNEL_VERSION(3, 3, 0))
		return pkt_len;

	uname(&un);

	if (pkt_len % 8) {
		if (strcmp(un.machine, "alpha") == 0 ||
		    strcmp(un.machine, "ia64") == 0 ||
		    strcmp(un.machine, "x86_64") == 0 ||
		    strcmp(un.machine, "parisc64") == 0 ||
		    strcmp(un.machine, "ppc64") == 0)
			pkt_len += 4;

	}

	return pkt_len;
}