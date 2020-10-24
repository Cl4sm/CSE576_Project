int keepalive_action(struct keepalive_info *ka, int *timeout)
{
	time_t now = time(NULL);

	if (ka->rekey_method != REKEY_NONE &&
	    ka_check_deadline(timeout, now, ka->last_rekey + ka->rekey)) {
		ka->last_rekey = now;
		return KA_REKEY;
	}

	/* DPD is bidirectional -- PKT 3 out, PKT 4 back */
	if (ka->dpd) {
		time_t due = ka->last_rx + ka->dpd;
		time_t overdue = ka->last_rx + (2 * ka->dpd);

		/* Peer didn't respond */
		if (now > overdue)
			return KA_DPD_DEAD;

		/* If we already have DPD outstanding, don't flood. Repeat by
		   all means, but only after half the DPD period. */
		if (ka->last_dpd > ka->last_rx)
			due = ka->last_dpd + ka->dpd / 2;

		/* We haven't seen a packet from this host for $DPD seconds.
		   Prod it to see if it's still alive */
		if (ka_check_deadline(timeout, now, due)) {
			ka->last_dpd = now;
			return KA_DPD;
		}
	}

	/* Keepalive is just client -> server.
	   If we haven't sent anything for $KEEPALIVE seconds, send a
	   dummy packet (which the server will discard) */
	if (ka->keepalive &&
	    ka_check_deadline(timeout, now, ka->last_tx + ka->keepalive))
		return KA_KEEPALIVE;

	return KA_NONE;
}