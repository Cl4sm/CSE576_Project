static UI_METHOD *create_openssl_ui(struct openconnect_info *vpninfo)
{
	UI_METHOD *ui_method = UI_create_method((char *)"AnyConnect VPN UI");

	/* There is a race condition here because of the use of the
	   static ui_vpninfo pointer. This sucks, but it's OpenSSL's
	   fault and in practice it's *never* going to hurt us.

	   This UI is only used for loading certificates from a TPM; for
	   PKCS#12 and PEM files we hook the passphrase request differently.
	   The ui_vpninfo variable is set here, and is used from ui_open()
	   when the TPM ENGINE decides it needs to ask the user for a PIN.

	   The race condition exists because theoretically, there
	   could be more than one thread using libopenconnect and
	   trying to authenticate to a VPN server, within the *same*
	   process. And if *both* are using certificates from the TPM,
	   and *both* manage to be within that short window of time
	   between setting ui_vpninfo and invoking ui_open() to fetch
	   the PIN, then one connection's ->process_auth_form() could
	   get a PIN request for the *other* connection.

	   However, the only thing that ever does run libopenconnect more
	   than once from the same process is KDE's NetworkManager support,
	   and NetworkManager doesn't *support* having more than one VPN
	   connected anyway, so first that would have to be fixed and then
	   you'd have to connect to two VPNs simultaneously by clicking
	   'connect' on both at *exactly* the same time and then getting
	   *really* unlucky.

	   Oh, and the KDE support won't be using OpenSSL anyway because of
	   licensing conflicts... so although this sucks, I'm not going to
	   lose sleep over it.
	*/
	ui_vpninfo = vpninfo;

	/* Set up a UI method of our own for password/passphrase requests */
	UI_method_set_opener(ui_method, ui_open);
	UI_method_set_writer(ui_method, ui_write);
	UI_method_set_flusher(ui_method, ui_flush);
	UI_method_set_closer(ui_method, ui_close);

	return ui_method;
}