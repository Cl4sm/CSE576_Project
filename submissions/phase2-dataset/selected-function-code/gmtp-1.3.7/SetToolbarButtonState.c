void SetToolbarButtonState(gboolean state) {
    gtk_widget_set_sensitive(GTK_WIDGET(toolbuttonAddFile), state);
    gtk_widget_set_sensitive(GTK_WIDGET(toolbuttonRemoveFile), state);
    gtk_widget_set_sensitive(GTK_WIDGET(toolbuttonRetrieve), state);
    gtk_widget_set_sensitive(GTK_WIDGET(toolbuttonRescan), state);
    gtk_widget_set_sensitive(GTK_WIDGET(toolbuttonAlbumArt), state);
    gtk_widget_set_sensitive(GTK_WIDGET(toolbuttonPlaylist), state);
    gtk_widget_set_sensitive(GTK_WIDGET(toolbuttonProperties), state);
    gtk_widget_set_sensitive(GTK_WIDGET(properties1), state);
    gtk_widget_set_sensitive(GTK_WIDGET(fileAdd), state);
    gtk_widget_set_sensitive(GTK_WIDGET(fileDownload), state);
    gtk_widget_set_sensitive(GTK_WIDGET(fileRemove), state);
    gtk_widget_set_sensitive(GTK_WIDGET(fileRename), state);
    gtk_widget_set_sensitive(GTK_WIDGET(fileMove), state);
    gtk_widget_set_sensitive(GTK_WIDGET(fileNewFolder), state);
    gtk_widget_set_sensitive(GTK_WIDGET(fileRemoveFolder), state);
    gtk_widget_set_sensitive(GTK_WIDGET(fileRescan), state);
    gtk_widget_set_sensitive(GTK_WIDGET(editDeviceName), state);
    gtk_widget_set_sensitive(GTK_WIDGET(editFormatDevice), state);
    gtk_widget_set_sensitive(GTK_WIDGET(editAddAlbumArt), state);
    gtk_widget_set_sensitive(GTK_WIDGET(editFind), state);
    gtk_widget_set_sensitive(GTK_WIDGET(editSelectAll), state);
    gtk_widget_set_sensitive(GTK_WIDGET(editPlaylist), state);
    gtk_widget_set_sensitive(GTK_WIDGET(treeviewFiles), state);
    // Only set this if we are using the normal connection method.
    if (!Preferences.use_alt_access_method) {
        gtk_widget_set_sensitive(GTK_WIDGET(treeviewFolders), state);
    }
}
