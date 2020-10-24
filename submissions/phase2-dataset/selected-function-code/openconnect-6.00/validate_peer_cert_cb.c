static int validate_peer_cert_cb(void *privdata, OPENCONNECT_X509 *cert, const char *reason)
{
	struct libctx *ctx = privdata;
	jstring jreason;
	int ret = -1;
	jmethodID mid;

	if ((*ctx->jenv)->PushLocalFrame(ctx->jenv, 256) < 0)
		return -1;

	jreason = dup_to_jstring(ctx->jenv, reason);
	if (!jreason)
		goto out;

	ctx->cert = cert;
	mid = get_obj_mid(ctx, ctx->jobj, "onValidatePeerCert", "(Ljava/lang/String;)I");
	if (mid)
		ret = (*ctx->jenv)->CallIntMethod(ctx->jenv, ctx->jobj, mid, jreason);

out:
	(*ctx->jenv)->PopLocalFrame(ctx->jenv, NULL);
	return ret;
}