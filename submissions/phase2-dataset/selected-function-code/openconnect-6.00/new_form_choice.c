static jobject new_form_choice(struct libctx *ctx, struct oc_choice *choice)
{
	jmethodID mid;
	jclass jcls;
	jobject jobj = NULL;

	jcls = (*ctx->jenv)->FindClass(ctx->jenv,
				       "org/infradead/libopenconnect/LibOpenConnect$FormChoice");
	if (jcls == NULL)
		return NULL;

	mid = (*ctx->jenv)->GetMethodID(ctx->jenv, jcls, "<init>", "()V");
	if (!mid)
		return NULL;
	jobj = (*ctx->jenv)->NewObject(ctx->jenv, jcls, mid);
	if (!jobj)
		return NULL;

	if (set_string(ctx, jobj, "name", choice->name) ||
	    set_string(ctx, jobj, "label", choice->label) ||
	    set_string(ctx, jobj, "authType", choice->auth_type) ||
	    set_string(ctx, jobj, "overrideName", choice->override_name) ||
	    set_string(ctx, jobj, "overrideLabel", choice->override_label)) {
		return NULL;
	}

	return jobj;
}