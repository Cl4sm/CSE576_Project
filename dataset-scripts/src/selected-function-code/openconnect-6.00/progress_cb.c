static void progress_cb(void *privdata, int level, const char *fmt, ...)
{
	struct libctx *ctx = privdata;
	va_list ap;
	char *msg;
	jstring jmsg;
	int ret, loglevel;
	jmethodID mid;

	(*ctx->jenv)->MonitorEnter(ctx->jenv, ctx->async_lock);
	loglevel = ctx->loglevel;
	(*ctx->jenv)->MonitorExit(ctx->jenv, ctx->async_lock);

	if (level > loglevel)
		return;

	va_start(ap, fmt);
	ret = vasprintf(&msg, fmt, ap);
	va_end(ap);

	if (ret < 0) {
		OOM(ctx->jenv);
		return;
	}

	if ((*ctx->jenv)->PushLocalFrame(ctx->jenv, 256) < 0)
		return;

	jmsg = dup_to_jstring(ctx->jenv, msg);
	free(msg);
	if (!jmsg)
		goto out;

	mid = get_obj_mid(ctx, ctx->jobj, "onProgress", "(ILjava/lang/String;)V");
	if (mid)
		(*ctx->jenv)->CallVoidMethod(ctx->jenv, ctx->jobj, mid, level, jmsg);

out:
	(*ctx->jenv)->PopLocalFrame(ctx->jenv, NULL);
}