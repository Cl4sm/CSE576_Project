static int add_form_option(struct libctx *ctx, jobject jform, struct oc_form_opt *opt, int is_authgroup)
{
	jmethodID addOpt;
	jobject jopt;

	addOpt = get_obj_mid(ctx, jform, "addOpt",
		"(Z)Lorg/infradead/libopenconnect/LibOpenConnect$FormOpt;");
	if (!addOpt)
		return -1;

	jopt = (*ctx->jenv)->CallObjectMethod(ctx->jenv, jform, addOpt, is_authgroup);
	if (jopt == NULL)
		return -1;

	if (set_int(ctx, jopt, "type", opt->type) ||
	    set_string(ctx, jopt, "name", opt->name) ||
	    set_string(ctx, jopt, "label", opt->label) ||
	    set_string(ctx, jopt, "value", opt->value) ||
	    set_long(ctx, jopt, "flags", opt->flags))
		return -1;

	if (opt->type == OC_FORM_OPT_SELECT &&
	    populate_select_choices(ctx, jopt, (struct oc_form_opt_select *)opt))
		return -1;

	return 0;
}