void on_editFind_activate(GtkMenuItem *menuitem, gpointer user_data) {
    if (inFindMode == FALSE) {

        gtk_widget_show(findToolbar);
        gtk_widget_hide(scrolledwindowFolders);

        fileListClear();
        //folderListClear();
        inFindMode = TRUE;
        statusBarSet(_("Please enter search item."));
        setWindowTitle(_("Search"));
        gtk_tree_view_column_set_visible(column_Location, TRUE);

        //Disable some of the menu options, while in search mode.
        gtk_widget_set_sensitive(GTK_WIDGET(cfileAdd), FALSE);
        gtk_widget_set_sensitive(GTK_WIDGET(cfileNewFolder), FALSE);
        gtk_widget_set_sensitive(GTK_WIDGET(fileAdd), FALSE);
        gtk_widget_set_sensitive(GTK_WIDGET(fileNewFolder), FALSE);
        gtk_widget_set_sensitive(GTK_WIDGET(toolbuttonAddFile), FALSE);
        gtk_widget_set_sensitive(GTK_WIDGET(menu_view_folders), FALSE);
        // Get focus on text entry box.

        gtk_widget_grab_focus(GTK_WIDGET(FindToolbar_entry_FindText));
    } else {
        on_editFindClose_activate(menuitem, user_data);
    }

} // end on_editFind_activate()
