static void stats_cb(void *privdata, const struct oc_stats *stats)
{
	struct libctx *ctx = privdata;
	jmethodID mid;
	jclass jcls;
	jobject jobj = NULL;

	if ((*ctx->jenv)->PushLocalFrame(ctx->jenv, 256) < 0)
		return;

	jcls = (*ctx->jenv)->FindClass(ctx->jenv, "org/infradead/libopenconnect/LibOpenConnect$VPNStats");
	if (jcls == NULL)
		goto out;

	mid = (*ctx->jenv)->GetMethodID(ctx->jenv, jcls, "<init>", "()V");
	if (!mid)
		goto out;
	jobj = (*ctx->jenv)->NewObject(ctx->jenv, jcls, mid);
	if (!jobj)
		goto out;

	if (set_long(ctx, jobj, "txPkts", stats->tx_pkts) ||
	    set_long(ctx, jobj, "txBytes", stats->tx_bytes) ||
	    set_long(ctx, jobj, "rxPkts", stats->rx_pkts) ||
	    set_long(ctx, jobj, "rxBytes", stats->rx_bytes))
		goto out;

	mid = get_obj_mid(ctx, ctx->jobj, "onStatsUpdate",
			  "(Lorg/infradead/libopenconnect/LibOpenConnect$VPNStats;)V");
	if (mid)
		(*ctx->jenv)->CallVoidMethod(ctx->jenv, ctx->jobj, mid, jobj);

out:
	(*ctx->jenv)->PopLocalFrame(ctx->jenv, NULL);
}