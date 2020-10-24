int cstp_mainloop(struct openconnect_info *vpninfo, int *timeout)
{
	unsigned char buf[16384];
	int len, ret;
	int work_done = 0;

	if (vpninfo->ssl_fd == -1)
		goto do_reconnect;

	/* FIXME: The poll() handling here is fairly simplistic. Actually,
	   if the SSL connection stalls it could return a WANT_WRITE error
	   on _either_ of the SSL_read() or SSL_write() calls. In that case,
	   we should probably remove POLLIN from the events we're looking for,
	   and add POLLOUT. As it is, though, it'll just chew CPU time in that
	   fairly unlikely situation, until the write backlog clears. */
	while ((len = cstp_read(vpninfo, buf, sizeof(buf))) > 0) {
		int payload_len;

		if (buf[0] != 'S' || buf[1] != 'T' ||
		    buf[2] != 'F' || buf[3] != 1 || buf[7])
			goto unknown_pkt;

		payload_len = (buf[4] << 8) + buf[5];
		if (len != 8 + payload_len) {
			vpn_progress(vpninfo, PRG_ERR,
				     _("Unexpected packet length. SSL_read returned %d but packet is\n"),
				     len);
			vpn_progress(vpninfo, PRG_ERR,
				     "%02x %02x %02x %02x %02x %02x %02x %02x\n",
				     buf[0], buf[1], buf[2], buf[3],
				     buf[4], buf[5], buf[6], buf[7]);
			continue;
		}
		vpninfo->ssl_times.last_rx = time(NULL);
		switch (buf[6]) {
		case AC_PKT_DPD_OUT:
			vpn_progress(vpninfo, PRG_DEBUG,
				     _("Got CSTP DPD request\n"));
			vpninfo->owe_ssl_dpd_response = 1;
			continue;

		case AC_PKT_DPD_RESP:
			vpn_progress(vpninfo, PRG_DEBUG,
				     _("Got CSTP DPD response\n"));
			continue;

		case AC_PKT_KEEPALIVE:
			vpn_progress(vpninfo, PRG_DEBUG,
				     _("Got CSTP Keepalive\n"));
			continue;

		case AC_PKT_DATA:
			vpn_progress(vpninfo, PRG_TRACE,
				     _("Received uncompressed data packet of %d bytes\n"),
				     payload_len);
			queue_new_packet(&vpninfo->incoming_queue, buf + 8,
					 payload_len);
			work_done = 1;
			continue;

		case AC_PKT_DISCONN: {
			int i;
			for (i = 0; i < payload_len; i++) {
				if (!isprint(buf[payload_len + 8 + i]))
					buf[payload_len + 8 + i] = '.';
			}
			buf[payload_len + 8] = 0;
			vpn_progress(vpninfo, PRG_ERR,
				     _("Received server disconnect: %02x '%s'\n"),
				     buf[8], buf + 9);
			vpninfo->quit_reason = "Server request";
			return -EPIPE;
		}
		case AC_PKT_COMPRESSED:
			if (!vpninfo->deflate) {
				vpn_progress(vpninfo, PRG_ERR,
					     _("Compressed packet received in !deflate mode\n"));
				goto unknown_pkt;
			}
			inflate_and_queue_packet(vpninfo, buf + 8, payload_len);
			work_done = 1;
			continue;

		case AC_PKT_TERM_SERVER:
			vpn_progress(vpninfo, PRG_ERR, _("received server terminate packet\n"));
			vpninfo->quit_reason = "Server request";
			return -EPIPE;
		}

	unknown_pkt:
		vpn_progress(vpninfo, PRG_ERR,
			     _("Unknown packet %02x %02x %02x %02x %02x %02x %02x %02x\n"),
			     buf[0], buf[1], buf[2], buf[3],
			     buf[4], buf[5], buf[6], buf[7]);
		vpninfo->quit_reason = "Unknown packet received";
		return 1;
	}
	if (len < 0)
		goto do_reconnect;


	/* If SSL_write() fails we are expected to try again. With exactly
	   the same data, at exactly the same location. So we keep the
	   packet we had before.... */
	if (vpninfo->current_ssl_pkt) {
	handle_outgoing:
		vpninfo->ssl_times.last_tx = time(NULL);
		unmonitor_write_fd(vpninfo, ssl);

		ret = cstp_write(vpninfo,
				 vpninfo->current_ssl_pkt->hdr,
				 vpninfo->current_ssl_pkt->len + 8);
		if (ret < 0)
			goto do_reconnect;
		else if (!ret) {
			/* -EAGAIN: cstp_write() will have added the SSL fd to
			   ->select_wfds if appropriate, so we can just return
			   and wait. Unless it's been stalled for so long that
			   DPD kicks in and we kill the connection. */
			switch (ka_stalled_action(&vpninfo->ssl_times, timeout)) {
			case KA_DPD_DEAD:
				goto peer_dead;
			case KA_REKEY:
				goto do_rekey;
			case KA_NONE:
				return work_done;
			default:
				/* This should never happen */
				;
			}
		}

		if (ret != vpninfo->current_ssl_pkt->len + 8) {
			vpn_progress(vpninfo, PRG_ERR,
				     _("SSL wrote too few bytes! Asked for %d, sent %d\n"),
				     vpninfo->current_ssl_pkt->len + 8, ret);
			vpninfo->quit_reason = "Internal error";
			return 1;
		}
		/* Don't free the 'special' packets */
		if (vpninfo->current_ssl_pkt == vpninfo->deflate_pkt)
			free(vpninfo->pending_deflated_pkt);
		else if (vpninfo->current_ssl_pkt != &dpd_pkt &&
			 vpninfo->current_ssl_pkt != &dpd_resp_pkt &&
			 vpninfo->current_ssl_pkt != &keepalive_pkt)
			free(vpninfo->current_ssl_pkt);

		vpninfo->current_ssl_pkt = NULL;
	}

	if (vpninfo->owe_ssl_dpd_response) {
		vpninfo->owe_ssl_dpd_response = 0;
		vpninfo->current_ssl_pkt = &dpd_resp_pkt;
		goto handle_outgoing;
	}

	switch (keepalive_action(&vpninfo->ssl_times, timeout)) {
	case KA_REKEY:
	do_rekey:
		/* Not that this will ever happen; we don't even process
		   the setting when we're asked for it. */
		vpn_progress(vpninfo, PRG_INFO, _("CSTP rekey due\n"));
		if (vpninfo->ssl_times.rekey_method == REKEY_TUNNEL)
			goto do_reconnect;
		else if (vpninfo->ssl_times.rekey_method == REKEY_SSL) {
			ret = cstp_handshake(vpninfo, 0);
			if (ret) {
				/* if we failed rehandshake try establishing a new-tunnel instead of failing */
				vpn_progress(vpninfo, PRG_ERR, _("Rehandshake failed; attempting new-tunnel\n"));
				goto do_reconnect;
			}

			goto do_dtls_reconnect;
		}
		break;

	case KA_DPD_DEAD:
	peer_dead:
		vpn_progress(vpninfo, PRG_ERR,
			     _("CSTP Dead Peer Detection detected dead peer!\n"));
	do_reconnect:
		ret = cstp_reconnect(vpninfo);
		if (ret) {
			vpn_progress(vpninfo, PRG_ERR, _("Reconnect failed\n"));
			vpninfo->quit_reason = "CSTP reconnect failed";
			return ret;
		}

	do_dtls_reconnect:
		/* succeeded, let's rekey DTLS, if it is not rekeying
		 * itself. */
		if (vpninfo->dtls_state != DTLS_DISABLED &&
		    vpninfo->dtls_times.rekey_method == REKEY_NONE) {
			dtls_reconnect(vpninfo);
		}

		return 1;

	case KA_DPD:
		vpn_progress(vpninfo, PRG_DEBUG, _("Send CSTP DPD\n"));

		vpninfo->current_ssl_pkt = &dpd_pkt;
		goto handle_outgoing;

	case KA_KEEPALIVE:
		/* No need to send an explicit keepalive
		   if we have real data to send */
		if (vpninfo->dtls_state != DTLS_CONNECTED && vpninfo->outgoing_queue)
			break;

		vpn_progress(vpninfo, PRG_DEBUG, _("Send CSTP Keepalive\n"));

		vpninfo->current_ssl_pkt = &keepalive_pkt;
		goto handle_outgoing;

	case KA_NONE:
		;
	}

	/* Service outgoing packet queue, if no DTLS */
	while (vpninfo->dtls_state != DTLS_CONNECTED && vpninfo->outgoing_queue) {
		struct pkt *this = vpninfo->outgoing_queue;
		vpninfo->outgoing_queue = this->next;
		vpninfo->outgoing_qlen--;

		if (vpninfo->deflate) {
			unsigned char *adler;
			int ret;

			vpninfo->deflate_strm.next_in = this->data;
			vpninfo->deflate_strm.avail_in = this->len;
			vpninfo->deflate_strm.next_out = (void *)vpninfo->deflate_pkt->data;
			vpninfo->deflate_strm.avail_out = 2040;
			vpninfo->deflate_strm.total_out = 0;

			ret = deflate(&vpninfo->deflate_strm, Z_SYNC_FLUSH);
			if (ret) {
				vpn_progress(vpninfo, PRG_ERR, _("deflate failed %d\n"), ret);
				goto uncompr;
			}

			vpninfo->deflate_pkt->hdr[4] = (vpninfo->deflate_strm.total_out + 4) >> 8;
			vpninfo->deflate_pkt->hdr[5] = (vpninfo->deflate_strm.total_out + 4) & 0xff;

			/* Add ongoing adler32 to tail of compressed packet */
			vpninfo->deflate_adler32 = adler32(vpninfo->deflate_adler32,
							   this->data, this->len);

			adler = &vpninfo->deflate_pkt->data[vpninfo->deflate_strm.total_out];
			*(adler++) =  vpninfo->deflate_adler32 >> 24;
			*(adler++) = (vpninfo->deflate_adler32 >> 16) & 0xff;
			*(adler++) = (vpninfo->deflate_adler32 >> 8) & 0xff;
			*(adler)   =  vpninfo->deflate_adler32 & 0xff;

			vpninfo->deflate_pkt->len = vpninfo->deflate_strm.total_out + 4;

			vpn_progress(vpninfo, PRG_TRACE,
				     _("Sending compressed data packet of %d bytes\n"),
				     this->len);

			vpninfo->pending_deflated_pkt = this;
			vpninfo->current_ssl_pkt = vpninfo->deflate_pkt;
		} else {
		uncompr:
			memcpy(this->hdr, data_hdr, 8);
			this->hdr[4] = this->len >> 8;
			this->hdr[5] = this->len & 0xff;

			vpn_progress(vpninfo, PRG_TRACE,
				     _("Sending uncompressed data packet of %d bytes\n"),
				     this->len);

			vpninfo->current_ssl_pkt = this;
		}
		goto handle_outgoing;
	}

	/* Work is not done if we just got rid of packets off the queue */
	return work_done;
}