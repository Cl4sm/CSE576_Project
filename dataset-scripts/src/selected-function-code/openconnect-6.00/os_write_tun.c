int os_write_tun(struct openconnect_info *vpninfo, struct pkt *pkt)
{
	unsigned char *data = pkt->data;
	int len = pkt->len;

#ifdef TUN_HAS_AF_PREFIX
	if (!vpninfo->script_tun) {
		struct ip *iph = (void *)data;
		int type;

		if (iph->ip_v == 6)
			type = AF_INET6;
		else if (iph->ip_v == 4)
			type = AF_INET;
		else {
			static int complained = 0;
			if (!complained) {
				complained = 1;
				vpn_progress(vpninfo, PRG_ERR,
					     _("Unknown packet (len %d) received: %02x %02x %02x %02x...\n"),
					     len, data[0], data[1], data[2], data[3]);
			}
			return 0;
		}
		data -= sizeof(int);
		len += sizeof(int);
		*(int *)data = htonl(type);
	}
#endif
	if (write(vpninfo->tun_fd, data, len) < 0) {
		/* Handle death of "script" socket */
		if (vpninfo->script_tun && errno == ENOTCONN) {
			vpninfo->quit_reason = "Client connection terminated";
			return -1;
		}
		vpn_progress(vpninfo, PRG_ERR,
			     _("Failed to write incoming packet: %s\n"),
			     strerror(errno));
		/* The kernel returns -ENOMEM when the queue is full, so theoretically
		   we could handle that and retry... but it doesn't let us poll() for
		   the no-longer-full situation, so let's not bother. */
	}
	return 0;

}