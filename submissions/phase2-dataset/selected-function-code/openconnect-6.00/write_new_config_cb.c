static int write_new_config_cb(void *privdata, char *buf, int buflen)
{
	struct libctx *ctx = privdata;
	jmethodID mid;
	jbyteArray jbuf;
	int ret = -1;

	if ((*ctx->jenv)->PushLocalFrame(ctx->jenv, 256) < 0)
		return -1;

	mid = get_obj_mid(ctx, ctx->jobj, "onWriteNewConfig", "([B)I");
	if (!mid)
		goto out;

	jbuf = (*ctx->jenv)->NewByteArray(ctx->jenv, buflen);
	if (!jbuf)
		goto out;
	(*ctx->jenv)->SetByteArrayRegion(ctx->jenv, jbuf, 0, buflen, (jbyte *)buf);

	ret = (*ctx->jenv)->CallIntMethod(ctx->jenv, ctx->jobj, mid, jbuf);

out:
	(*ctx->jenv)->PopLocalFrame(ctx->jenv, NULL);
	return ret;
}