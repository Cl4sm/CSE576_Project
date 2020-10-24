static void menu_load_event(GtkMenuItem *menuitem, gpointer data)
{
    frontend *fe = (frontend *)data;
    char *name, *err;

    name = file_selector(fe, "Enter name of saved game file to load", FALSE);

    if (name) {
        FILE *fp = fopen(name, "r");
        sfree(name);

        if (!fp) {
            error_box(fe->window, "Unable to open saved game file");
            return;
        }

        err = midend_deserialise(fe->me, savefile_read, fp);

        fclose(fp);

        if (err) {
            error_box(fe->window, err);
            return;
        }

	changed_preset(fe);
        resize_fe(fe);
    }
}