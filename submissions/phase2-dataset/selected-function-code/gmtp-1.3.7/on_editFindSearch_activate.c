void on_editFindSearch_activate(GtkMenuItem *menuitem, gpointer user_data) {
    gchar *searchstring = NULL;
    gboolean searchfiles = FALSE;
    gboolean searchmeta = FALSE;

    statusBarSet(_("Searching..."));
    // Now clear the Search GList;
    if (searchList != NULL) {
        g_slist_foreach(searchList, (GFunc) g_free_search, NULL);
        g_slist_free(searchList);
        searchList = NULL;
    }

    // Set to upper case to perform case insensitive searches.
    searchstring = g_utf8_strup(gtk_entry_get_text(GTK_ENTRY(FindToolbar_entry_FindText)), -1);
    searchfiles = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(FindToolbar_checkbutton_FindFiles));
    searchmeta = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(FindToolbar_checkbutton_TrackInformation));
    // disable search metadata if treatallmediaasfiles or in alternate access mode is set.
    if(Preferences.allmediaasfiles == TRUE || Preferences.use_alt_access_method == TRUE){
        searchmeta = FALSE;
    }
    // Let's start our search.
    searchList = filesSearch(searchstring, searchfiles, searchmeta);
    inFindMode = TRUE;
    fileListClear();
    fileListAdd();
    g_free(searchstring);
} // end on_editFindSearch_activate()
