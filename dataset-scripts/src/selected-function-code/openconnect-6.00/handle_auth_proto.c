static void handle_auth_proto(struct openconnect_info *vpninfo,
			      struct auth_method *method, char *hdr)
{
	struct proxy_auth_state *auth = &vpninfo->auth[method->state_index];
	int l = strlen(method->name);

	if (auth->state <= AUTH_FAILED)
		return;

	if (strncmp(method->name, hdr, l))
		return;
	if (hdr[l] != ' ' && hdr[l] != 0)
		return;

	if (auth->state == AUTH_UNSEEN)
		auth->state = AUTH_AVAILABLE;

	free(auth->challenge);
	if (hdr[l])
		auth->challenge = strdup(hdr + l + 1);
	else
		auth->challenge = NULL;
}