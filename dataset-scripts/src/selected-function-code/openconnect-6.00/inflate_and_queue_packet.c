static int inflate_and_queue_packet(struct openconnect_info *vpninfo,
				    unsigned char *buf, int len)
{
	struct pkt *new = malloc(sizeof(struct pkt) + vpninfo->ip_info.mtu);
	uint32_t pkt_sum;

	if (!new)
		return -ENOMEM;

	new->next = NULL;

	vpninfo->inflate_strm.next_in = buf;
	vpninfo->inflate_strm.avail_in = len - 4;

	vpninfo->inflate_strm.next_out = new->data;
	vpninfo->inflate_strm.avail_out = vpninfo->ip_info.mtu;
	vpninfo->inflate_strm.total_out = 0;

	if (inflate(&vpninfo->inflate_strm, Z_SYNC_FLUSH)) {
		vpn_progress(vpninfo, PRG_ERR, _("inflate failed\n"));
		free(new);
		return -EINVAL;
	}

	new->len = vpninfo->inflate_strm.total_out;

	vpninfo->inflate_adler32 = adler32(vpninfo->inflate_adler32,
					   new->data, new->len);

	pkt_sum = buf[len - 1] | (buf[len - 2] << 8) |
		(buf[len - 3] << 16) | (buf[len - 4] << 24);

	if (vpninfo->inflate_adler32 != pkt_sum)
		vpninfo->quit_reason = "Compression (inflate) adler32 failure";

	vpn_progress(vpninfo, PRG_TRACE,
		     _("Received compressed data packet of %ld bytes\n"),
		     (long)vpninfo->inflate_strm.total_out);

	queue_packet(&vpninfo->incoming_queue, new);
	return 0;
}