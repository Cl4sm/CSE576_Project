static void print_gss_err(struct openconnect_info *vpninfo, const char *where,
			  OM_uint32 err_maj, OM_uint32 err_min)
{
	OM_uint32 major, minor, msg_ctx = 0;
	gss_buffer_desc status;

	do {
		major = gss_display_status(&minor, err_maj, GSS_C_GSS_CODE,
					   GSS_C_NO_OID, &msg_ctx, &status);
		if (GSS_ERROR(major))
			break;
		vpn_progress(vpninfo, PRG_ERR, "%s: %s\n", where, (char *)status.value);
		gss_release_buffer(&minor, &status);

		major = gss_display_status(&minor, err_min, GSS_C_MECH_CODE,
					   GSS_C_NO_OID, &msg_ctx, &status);
		if (GSS_ERROR(major))
			break;
		vpn_progress(vpninfo, PRG_ERR, "%s: %s\n", where, (char *)status.value);
		gss_release_buffer(&minor, &status);
	} while (msg_ctx);
}