static void cfg_start(int which)
{
    char *title;
    int i;

    cfg = midend_get_config(me, which, &title);
    cfg_which = which;

    js_dialog_init(title);
    sfree(title);

    for (i = 0; cfg[i].type != C_END; i++) {
	switch (cfg[i].type) {
	  case C_STRING:
            js_dialog_string(i, cfg[i].name, cfg[i].sval);
	    break;
	  case C_BOOLEAN:
            js_dialog_boolean(i, cfg[i].name, cfg[i].ival);
	    break;
	  case C_CHOICES:
            js_dialog_choices(i, cfg[i].name, cfg[i].sval, cfg[i].ival);
	    break;
	}
    }

    js_dialog_launch();
}