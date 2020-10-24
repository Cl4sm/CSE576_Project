static int ui_write(UI *ui, UI_STRING *uis)
{
	struct ui_data *ui_data = UI_get0_user_data(ui);
	struct ui_form_opt *opt;

	switch (UI_get_string_type(uis)) {
	case UIT_ERROR:
		ui_data->form.error = (char *)UI_get0_output_string(uis);
		break;
	case UIT_INFO:
		ui_data->form.message = (char *)UI_get0_output_string(uis);
		break;
	case UIT_PROMPT:
		opt = malloc(sizeof(*opt));
		if (!opt)
			return 1;
		memset(opt, 0, sizeof(*opt));
		opt->uis = uis;
		opt->opt.label = opt->opt.name = (char *)UI_get0_output_string(uis);
		if (UI_get_input_flags(uis) & UI_INPUT_FLAG_ECHO)
			opt->opt.type = OC_FORM_OPT_TEXT;
		else
			opt->opt.type = OC_FORM_OPT_PASSWORD;
		*(ui_data->last_opt) = &opt->opt;
		ui_data->last_opt = &opt->opt.next;
		break;

	default:
		fprintf(stderr, "Unhandled SSL UI request type %d\n",
			UI_get_string_type(uis));
		return 0;
	}
	return 1;
}