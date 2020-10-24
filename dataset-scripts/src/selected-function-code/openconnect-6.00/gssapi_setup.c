static int gssapi_setup(struct openconnect_info *vpninfo, const char *service)
{
	OM_uint32 major, minor;
	gss_buffer_desc token = GSS_C_EMPTY_BUFFER;
	char *name;

	if (asprintf(&name, "%s@%s", service, vpninfo->proxy) == -1)
		return -ENOMEM;
	token.length = strlen(name);
	token.value = name;

	major = gss_import_name(&minor, &token, (gss_OID)GSS_C_NT_HOSTBASED_SERVICE,
				&vpninfo->gss_target_name);
	free(name);
	if (GSS_ERROR(major)) {
		vpn_progress(vpninfo, PRG_ERR,
			     _("Error importing GSSAPI name for authentication:\n"));
		print_gss_err(vpninfo, "gss_import_name()", major, minor);
		return -EIO;
	}
	return 0;
}