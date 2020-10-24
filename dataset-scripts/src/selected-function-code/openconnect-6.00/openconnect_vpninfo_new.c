struct openconnect_info *openconnect_vpninfo_new(char *useragent,
						 openconnect_validate_peer_cert_vfn validate_peer_cert,
						 openconnect_write_new_config_vfn write_new_config,
						 openconnect_process_auth_form_vfn process_auth_form,
						 openconnect_progress_vfn progress,
						 void *privdata)
{
	struct openconnect_info *vpninfo = calloc(sizeof(*vpninfo), 1);

	if (!vpninfo)
		return NULL;

#ifndef _WIN32
	vpninfo->tun_fd = -1;
#endif
	vpninfo->ssl_fd = vpninfo->dtls_fd = -1;
	vpninfo->cmd_fd = vpninfo->cmd_fd_write = -1;
	vpninfo->cert_expire_warning = 60 * 86400;
	vpninfo->deflate = 1;
	vpninfo->max_qlen = 10;
	vpninfo->localname = strdup("localhost");
	vpninfo->useragent = openconnect_create_useragent(useragent);
	vpninfo->validate_peer_cert = validate_peer_cert;
	vpninfo->write_new_config = write_new_config;
	vpninfo->process_auth_form = process_auth_form;
	vpninfo->progress = progress;
	vpninfo->cbdata = privdata ? : vpninfo;
	vpninfo->xmlpost = 1;
	openconnect_set_reported_os(vpninfo, NULL);

	if (!vpninfo->localname || !vpninfo->useragent)
		goto err;

#ifdef ENABLE_NLS
	bindtextdomain("openconnect", LOCALEDIR);
#endif

	return vpninfo;

err:
	free(vpninfo->localname);
	free(vpninfo->useragent);
	free(vpninfo);
	return NULL;
}