static char *file_selector(frontend *fe, char *title, int save)
{
    char *filesel_name = NULL;

    GtkWidget *filesel =
        gtk_file_chooser_dialog_new(title,
				    GTK_WINDOW(fe->window),
				    save ? GTK_FILE_CHOOSER_ACTION_SAVE :
				    GTK_FILE_CHOOSER_ACTION_OPEN,
				    GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
				    save ? GTK_STOCK_SAVE : GTK_STOCK_OPEN,
				    GTK_RESPONSE_ACCEPT,
				    NULL);

    if (gtk_dialog_run(GTK_DIALOG(filesel)) == GTK_RESPONSE_ACCEPT) {
        const char *name = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(filesel));
        filesel_name = dupstr(name);
    }

    gtk_widget_destroy(filesel);

    return filesel_name;
}