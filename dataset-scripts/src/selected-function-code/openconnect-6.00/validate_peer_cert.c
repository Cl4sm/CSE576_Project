static int validate_peer_cert(void *_vpninfo, OPENCONNECT_X509 *peer_cert,
			      const char *reason)
{
	struct openconnect_info *vpninfo = _vpninfo;
	char fingerprint[SHA1_SIZE * 2 + 1];
	struct accepted_cert *this;
	int ret;

	if (nocertcheck)
		return 0;

	ret = openconnect_get_cert_sha1(vpninfo, peer_cert, fingerprint);
	if (ret)
		return ret;

	for (this = accepted_certs; this; this = this->next) {
		if (!strcasecmp(this->host, vpninfo->hostname) &&
		    !strcasecmp(this->fingerprint, fingerprint))
			return 0;
	}

	while (1) {
		char buf[80];
		char *details;
		char *p;

		fprintf(stderr, _("\nCertificate from VPN server \"%s\" failed verification.\n"
			 "Reason: %s\n"), vpninfo->hostname, reason);

		if (non_inter)
			return -EINVAL;

		fprintf(stderr, _("Enter '%s' to accept, '%s' to abort; anything else to view: "),
		       _("yes"), _("no"));
		if (!fgets(buf, sizeof(buf), stdin))
			return -EINVAL;
		p = strchr(buf, '\n');
		if (p)
			*p = 0;

		if (!strcasecmp(buf, _("yes"))) {
			struct accepted_cert *newcert = malloc(sizeof(*newcert) +
							       strlen(vpninfo->hostname) + 1);
			if (newcert) {
				newcert->next = accepted_certs;
				accepted_certs = newcert;
				strcpy(newcert->fingerprint, fingerprint);
				strcpy(newcert->host, vpninfo->hostname);
			}
			return 0;
		}
		if (!strcasecmp(buf, _("no")))
			return -EINVAL;

		details = openconnect_get_cert_details(vpninfo, peer_cert);
		fputs(details, stderr);
		free(details);
		fprintf(stderr, _("SHA1 fingerprint: %s\n"), fingerprint);
	}
}