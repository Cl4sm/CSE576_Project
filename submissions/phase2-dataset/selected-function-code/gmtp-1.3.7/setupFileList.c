    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    GtkWidget *header;
    GtkWidget *parent;

    // Filename column
    //renderer = gtk_cell_renderer_text_new();
    //column = gtk_tree_view_column_new_with_attributes(_("Filename"), renderer,
    //    "text", COL_FILENAME,
    //    NULL);

    column = gtk_tree_view_column_new();
    gtk_tree_view_column_set_title(column, _("Filename"));
    renderer = gtk_cell_renderer_pixbuf_new();

    gtk_tree_view_column_pack_start(column, renderer, FALSE);
    gtk_tree_view_column_set_attributes(column, renderer, "pixbuf", COL_ICON, NULL);

    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_column_pack_start(column, renderer, TRUE);
    gtk_tree_view_column_set_attributes(column, renderer, "text", COL_FILENAME_ACTUAL, NULL);

    gtk_tree_view_append_column(GTK_TREE_VIEW(treeviewFiles), column);
    gtk_tree_view_column_set_sort_column_id(column, COL_FILENAME_HIDDEN);
    gtk_tree_view_column_set_resizable(column, TRUE);
    gtk_tree_view_column_set_spacing(column, 5);

    header = gtk_label_new(_("Filename"));
    gtk_widget_show(header);
    gtk_tree_view_column_set_widget(column, header);
    parent = gtk_widget_get_ancestor(gtk_tree_view_column_get_widget(column), GTK_TYPE_BUTTON);

    g_signal_connect_swapped(parent, "button_press_event",
            G_CALLBACK(on_windowViewContextMenu_activate), contextMenuColumn);

    // Filename column for sorting.
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Filename Hidden", renderer,
            "text", COL_FILENAME_HIDDEN,
            NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeviewFiles), column);
    gtk_tree_view_column_set_visible(column, FALSE);

    // File name actual - used for renaming operations.
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Filename Actual", renderer,
            "text", COL_FILENAME_ACTUAL,
            NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeviewFiles), column);
    gtk_tree_view_column_set_visible(column, FALSE);

    // File Size column
    renderer = gtk_cell_renderer_text_new();
    column_Size = gtk_tree_view_column_new_with_attributes(_("Size"), renderer,
            "text", COL_FILESIZE,
            NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeviewFiles), column_Size);
    gtk_tree_view_column_set_sort_column_id(column_Size, COL_FILESIZE_HID);
    gtk_tree_view_column_set_resizable(column_Size, TRUE);
    gtk_tree_view_column_set_spacing(column_Size, 5);
    gtk_tree_view_column_set_visible(column_Size, Preferences.view_size);

    header = gtk_label_new(_("Size"));
    gtk_widget_show(header);
    gtk_tree_view_column_set_widget(column_Size, header);
    parent = gtk_widget_get_ancestor(gtk_tree_view_column_get_widget(column_Size), GTK_TYPE_BUTTON);

    g_signal_connect_swapped(parent, "button_press_event",
            G_CALLBACK(on_windowViewContextMenu_activate), contextMenuColumn);


    // Folder/FileID column
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Object ID", renderer,
            "text", COL_FILEID,
            NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeviewFiles), column);
    gtk_tree_view_column_set_visible(column, FALSE);

    // isFolder column
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("isFolder", renderer,
            "text", COL_ISFOLDER,
            NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeviewFiles), column);
    gtk_tree_view_column_set_visible(column, FALSE);

    // File size column - hidden used for sorting the visible file size column
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("FileSize Hidden", renderer,
            "text", COL_FILESIZE_HID,
            NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeviewFiles), column);
    gtk_tree_view_column_set_visible(column, FALSE);

    // File Type column
    renderer = gtk_cell_renderer_text_new();
    column_Type = gtk_tree_view_column_new_with_attributes(_("File Type"), renderer,
            "text", COL_TYPE,
            NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeviewFiles), column_Type);
    gtk_tree_view_column_set_sort_column_id(column_Type, COL_TYPE);
    gtk_tree_view_column_set_resizable(column_Type, TRUE);
    gtk_tree_view_column_set_spacing(column_Type, 5);
    gtk_tree_view_column_set_visible(column_Type, Preferences.view_type);

    header = gtk_label_new(_("File Type"));
    gtk_widget_show(header);
    gtk_tree_view_column_set_widget(column_Type, header);
    parent = gtk_widget_get_ancestor(gtk_tree_view_column_get_widget(column_Type), GTK_TYPE_BUTTON);

    g_signal_connect_swapped(parent, "button_press_event",
            G_CALLBACK(on_windowViewContextMenu_activate), contextMenuColumn);

    // Track Number column
    renderer = gtk_cell_renderer_text_new();
    column_Track_Number = gtk_tree_view_column_new_with_attributes(_("Track"), renderer,
            "text", COL_TRACK_NUMBER,
            NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeviewFiles), column_Track_Number);
    gtk_tree_view_column_set_sort_column_id(column_Track_Number, COL_TRACK_NUMBER_HIDDEN);
    gtk_tree_view_column_set_resizable(column_Track_Number, TRUE);
    gtk_tree_view_column_set_spacing(column_Track_Number, 5);
    gtk_tree_view_column_set_visible(column_Track_Number, Preferences.view_track_number);

    header = gtk_label_new(_("Track"));
    gtk_widget_show(header);
    gtk_tree_view_column_set_widget(column_Track_Number, header);
    parent = gtk_widget_get_ancestor(gtk_tree_view_column_get_widget(column_Track_Number), GTK_TYPE_BUTTON);

    g_signal_connect_swapped(parent, "button_press_event",
            G_CALLBACK(on_windowViewContextMenu_activate), contextMenuColumn);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Track Num Hidden", renderer,
            "text", COL_TRACK_NUMBER_HIDDEN,
            NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeviewFiles), column);
    gtk_tree_view_column_set_visible(column, FALSE);

    // Track Title column
    renderer = gtk_cell_renderer_text_new();
    column_Title = gtk_tree_view_column_new_with_attributes(_("Track Name"), renderer,
            "text", COL_TITLE,
            NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeviewFiles), column_Title);
    gtk_tree_view_column_set_sort_column_id(column_Title, COL_TITLE);
    gtk_tree_view_column_set_resizable(column_Title, TRUE);
    gtk_tree_view_column_set_spacing(column_Title, 5);
    gtk_tree_view_column_set_visible(column_Title, Preferences.view_track_number);

    header = gtk_label_new(_("Track Name"));
    gtk_widget_show(header);
    gtk_tree_view_column_set_widget(column_Title, header);
    parent = gtk_widget_get_ancestor(gtk_tree_view_column_get_widget(column_Title), GTK_TYPE_BUTTON);

    g_signal_connect_swapped(parent, "button_press_event",
            G_CALLBACK(on_windowViewContextMenu_activate), contextMenuColumn);

    // Artist column
    renderer = gtk_cell_renderer_text_new();
    column_Artist = gtk_tree_view_column_new_with_attributes(_("Artist"), renderer,
            "text", COL_FL_ARTIST,
            NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeviewFiles), column_Artist);
    gtk_tree_view_column_set_sort_column_id(column_Artist, COL_FL_ARTIST);
    gtk_tree_view_column_set_resizable(column_Artist, TRUE);
    gtk_tree_view_column_set_spacing(column_Artist, 5);
    gtk_tree_view_column_set_visible(column_Artist, Preferences.view_artist);

    header = gtk_label_new(_("Artist"));
    gtk_widget_show(header);
    gtk_tree_view_column_set_widget(column_Artist, header);
    parent = gtk_widget_get_ancestor(gtk_tree_view_column_get_widget(column_Artist), GTK_TYPE_BUTTON);

    g_signal_connect_swapped(parent, "button_press_event",
            G_CALLBACK(on_windowViewContextMenu_activate), contextMenuColumn);

    // Album column
    renderer = gtk_cell_renderer_text_new();
    column_Album = gtk_tree_view_column_new_with_attributes(_("Album"), renderer,
            "text", COL_FL_ALBUM,
            NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeviewFiles), column_Album);
    gtk_tree_view_column_set_sort_column_id(column_Album, COL_FL_ALBUM);
    gtk_tree_view_column_set_resizable(column_Album, TRUE);
    gtk_tree_view_column_set_spacing(column_Album, 5);
    gtk_tree_view_column_set_visible(column_Album, Preferences.view_album);

    header = gtk_label_new(_("Album"));
    gtk_widget_show(header);
    gtk_tree_view_column_set_widget(column_Album, header);
    parent = gtk_widget_get_ancestor(gtk_tree_view_column_get_widget(column_Album), GTK_TYPE_BUTTON);

    g_signal_connect_swapped(parent, "button_press_event",
            G_CALLBACK(on_windowViewContextMenu_activate), contextMenuColumn);

    // Year column
    renderer = gtk_cell_renderer_text_new();
    column_Year = gtk_tree_view_column_new_with_attributes(_("Year"), renderer,
            "text", COL_YEAR,
            NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeviewFiles), column_Year);
    gtk_tree_view_column_set_sort_column_id(column_Year, COL_YEAR);
    gtk_tree_view_column_set_resizable(column_Year, TRUE);
    gtk_tree_view_column_set_spacing(column_Year, 5);
    gtk_tree_view_column_set_visible(column_Year, Preferences.view_year);

    header = gtk_label_new(_("Year"));
    gtk_widget_show(header);
    gtk_tree_view_column_set_widget(column_Year, header);
    parent = gtk_widget_get_ancestor(gtk_tree_view_column_get_widget(column_Year), GTK_TYPE_BUTTON);

    g_signal_connect_swapped(parent, "button_press_event",
            G_CALLBACK(on_windowViewContextMenu_activate), contextMenuColumn);

    // Genre column
    renderer = gtk_cell_renderer_text_new();
    column_Genre = gtk_tree_view_column_new_with_attributes(_("Genre"), renderer,
            "text", COL_GENRE,
            NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeviewFiles), column_Genre);
    gtk_tree_view_column_set_sort_column_id(column_Genre, COL_GENRE);
    gtk_tree_view_column_set_resizable(column_Genre, TRUE);
    gtk_tree_view_column_set_spacing(column_Genre, 5);
    gtk_tree_view_column_set_visible(column_Genre, Preferences.view_genre);

    header = gtk_label_new(_("Genre"));
    gtk_widget_show(header);
    gtk_tree_view_column_set_widget(column_Genre, header);
    parent = gtk_widget_get_ancestor(gtk_tree_view_column_get_widget(column_Genre), GTK_TYPE_BUTTON);

    g_signal_connect_swapped(parent, "button_press_event",
            G_CALLBACK(on_windowViewContextMenu_activate), contextMenuColumn);

    // Duration Visible column
    renderer = gtk_cell_renderer_text_new();
    column_Duration = gtk_tree_view_column_new_with_attributes(_("Duration"), renderer,
            "text", COL_DURATION,
            NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeviewFiles), column_Duration);
    gtk_tree_view_column_set_sort_column_id(column_Duration, COL_DURATION_HIDDEN);
    gtk_tree_view_column_set_resizable(column_Duration, TRUE);
    gtk_tree_view_column_set_spacing(column_Duration, 5);
    gtk_tree_view_column_set_visible(column_Duration, Preferences.view_duration);

    header = gtk_label_new(_("Duration"));
    gtk_widget_show(header);
    gtk_tree_view_column_set_widget(column_Duration, header);
    parent = gtk_widget_get_ancestor(gtk_tree_view_column_get_widget(column_Duration), GTK_TYPE_BUTTON);

    g_signal_connect_swapped(parent, "button_press_event",
            G_CALLBACK(on_windowViewContextMenu_activate), contextMenuColumn);

    // Duration Hidden column
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Duration Hidden", renderer,
            "text", COL_DURATION_HIDDEN,
            NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeviewFiles), column);
    gtk_tree_view_column_set_visible(column, FALSE);

    // Location column - only used in search mode.
    renderer = gtk_cell_renderer_text_new();
    column_Location = gtk_tree_view_column_new_with_attributes(_("Location"), renderer,
            "text", COL_LOCATION,
            NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeviewFiles), column_Location);
    gtk_tree_view_column_set_sort_column_id(column_Location, COL_LOCATION);
    gtk_tree_view_column_set_resizable(column_Location, TRUE);
    gtk_tree_view_column_set_spacing(column_Location, 5);
    gtk_tree_view_column_set_visible(column_Location, FALSE);

    header = gtk_label_new(_("Location"));
    gtk_widget_show(header);
    gtk_tree_view_column_set_widget(column_Location, header);
    parent = gtk_widget_get_ancestor(gtk_tree_view_column_get_widget(column_Location), GTK_TYPE_BUTTON);

    g_signal_connect_swapped(parent, "button_press_event",
            G_CALLBACK(on_windowViewContextMenu_activate), contextMenuColumn);
}
