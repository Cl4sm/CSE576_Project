static jobject new_auth_form(struct libctx *ctx, struct oc_auth_form *form)
{
	jmethodID mid;
	jclass jcls;
	jobject jobj = NULL;

	jcls = (*ctx->jenv)->FindClass(ctx->jenv, "org/infradead/libopenconnect/LibOpenConnect$AuthForm");
	if (jcls == NULL)
		return NULL;

	mid = (*ctx->jenv)->GetMethodID(ctx->jenv, jcls, "<init>", "()V");
	if (!mid)
		return NULL;
	jobj = (*ctx->jenv)->NewObject(ctx->jenv, jcls, mid);
	if (!jobj)
		return NULL;

	if (set_string(ctx, jobj, "banner", form->banner) ||
	    set_string(ctx, jobj, "message", form->message) ||
	    set_string(ctx, jobj, "error", form->error) ||
	    set_string(ctx, jobj, "authID", form->auth_id) ||
	    set_string(ctx, jobj, "method", form->method) ||
	    set_string(ctx, jobj, "action", form->action) ||
	    set_int(ctx, jobj, "authgroupSelection", form->authgroup_selection)) {
		return NULL;
	}

	return jobj;
}