static int add_string_pair(struct libctx *ctx, jclass jcls, jobject jobj,
		      const char *name, const char *key, const char *value)
{
	jmethodID mid = (*ctx->jenv)->GetMethodID(ctx->jenv, jcls, name, "(Ljava/lang/String;Ljava/lang/String;)V");
	jstring jarg0, jarg1;

	if (!key || !value)
		return -1;

	if (!mid)
		return -1;

	jarg0 = dup_to_jstring(ctx->jenv, key);
	if (!jarg0)
		return -1;

	jarg1 = dup_to_jstring(ctx->jenv, value);
	if (!jarg1) {
		(*ctx->jenv)->DeleteLocalRef(ctx->jenv, jarg0);
		return -1;
	}

	(*ctx->jenv)->CallVoidMethod(ctx->jenv, jobj, mid, jarg0, jarg1);

	(*ctx->jenv)->DeleteLocalRef(ctx->jenv, jarg1);
	(*ctx->jenv)->DeleteLocalRef(ctx->jenv, jarg0);

	return 0;
}