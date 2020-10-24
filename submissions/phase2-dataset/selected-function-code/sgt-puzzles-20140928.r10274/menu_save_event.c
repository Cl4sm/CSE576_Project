static void menu_save_event(GtkMenuItem *menuitem, gpointer data)
{
    frontend *fe = (frontend *)data;
    char *name;

    name = file_selector(fe, "Enter name of game file to save", TRUE);

    if (name) {
        FILE *fp;

	if ((fp = fopen(name, "r")) != NULL) {
	    char buf[256 + FILENAME_MAX];
	    fclose(fp);
	    /* file exists */

	    sprintf(buf, "Are you sure you want to overwrite the"
		    " file \"%.*s\"?",
		    FILENAME_MAX, name);
	    if (!message_box(fe->window, "Question", buf, TRUE, MB_YESNO))
		return;
	}

	fp = fopen(name, "w");
        sfree(name);

        if (!fp) {
            error_box(fe->window, "Unable to open save file");
            return;
        }

	{
	    struct savefile_write_ctx ctx;
	    ctx.fp = fp;
	    ctx.error = 0;
	    midend_serialise(fe->me, savefile_write, &ctx);
	    fclose(fp);
	    if (ctx.error) {
		char boxmsg[512];
		sprintf(boxmsg, "Error writing save file: %.400s",
			strerror(errno));
		error_box(fe->window, boxmsg);
		return;
	    }
	}

    }
}