int ntlm_authorization(struct openconnect_info *vpninfo, struct oc_text_buf *buf)
{
	if (vpninfo->auth[AUTH_TYPE_NTLM].state == AUTH_AVAILABLE) {
		vpninfo->auth[AUTH_TYPE_NTLM].state = NTLM_MANUAL;
		/* Don't attempt automatic NTLM auth if we were given a password */
		if (!vpninfo->proxy_pass && !ntlm_helper_spawn(vpninfo, buf)) {
			vpninfo->auth[AUTH_TYPE_NTLM].state = NTLM_SSO_REQ;
			return 0;
		}
	}
	if (vpninfo->auth[AUTH_TYPE_NTLM].state == NTLM_SSO_REQ) {
		int ret;
		ret = ntlm_helper_challenge(vpninfo, buf);
		/* Clean up after it. We're done here, whether it worked or not */
		cleanup_ntlm_auth(vpninfo);
		vpninfo->auth[AUTH_TYPE_NTLM].state = NTLM_MANUAL;
		if (!ret || ret == -EAGAIN)
			return ret;
	}
	if (vpninfo->auth[AUTH_TYPE_NTLM].state == NTLM_MANUAL && vpninfo->proxy_user &&
	    vpninfo->proxy_pass) {
		buf_append(buf, "Proxy-Authorization: NTLM %s\r\n",
			   "TlRMTVNTUAABAAAABYIIAAAAAAAAAAAAAAAAAAAAAAAAAAAAMAAAAAAAAAAwAAAA");
		vpninfo->auth[AUTH_TYPE_NTLM].state = NTLM_MANUAL_REQ;
		return 0;
	}
	if (vpninfo->auth[AUTH_TYPE_NTLM].state == NTLM_MANUAL_REQ &&
	    !ntlm_manual_challenge(vpninfo, buf)) {
		/* Leave the state as it is. If we come back there'll be no
		   challenge string and we'll fail then. */
		return 0;
	}
	vpninfo->auth[AUTH_TYPE_NTLM].state = AUTH_FAILED;
	return -EAGAIN;
}