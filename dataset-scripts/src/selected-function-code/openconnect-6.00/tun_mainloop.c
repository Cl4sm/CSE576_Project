int tun_mainloop(struct openconnect_info *vpninfo, int *timeout)
{
	int work_done = 0;

	if (read_fd_monitored(vpninfo, tun)) {
		while (1) {
			int len = vpninfo->ip_info.mtu;

			if (!out_pkt) {
				out_pkt = malloc(sizeof(struct pkt) + len);
				if (!out_pkt) {
					vpn_progress(vpninfo, PRG_ERR, "Allocation failed\n");
					break;
				}
				out_pkt->len = len;
			}

			if (os_read_tun(vpninfo, out_pkt))
				break;

			vpninfo->stats.tx_pkts++;
			vpninfo->stats.tx_bytes += out_pkt->len;

			queue_packet(&vpninfo->outgoing_queue, out_pkt);
			out_pkt = NULL;

			work_done = 1;
			vpninfo->outgoing_qlen++;
			if (vpninfo->outgoing_qlen == vpninfo->max_qlen) {
				unmonitor_read_fd(vpninfo, tun);
				break;
			}
		}
	} else if (vpninfo->outgoing_qlen < vpninfo->max_qlen) {
		monitor_read_fd(vpninfo, tun);
	}

	while (vpninfo->incoming_queue) {
		struct pkt *this = vpninfo->incoming_queue;

		if (os_write_tun(vpninfo, this))
			break;

		vpninfo->stats.rx_pkts++;
		vpninfo->stats.rx_bytes += this->len;

		vpninfo->incoming_queue = this->next;

		free(this);
	}
	/* Work is not done if we just got rid of packets off the queue */
	return work_done;
}