    gchar *tmp_string;
    GtkWidget *menuText;

    deviceConnect();
    //g_printf("Device connect/disconnect code = %d\n", result);
    // Update our label to indicate current condition.
    if (DeviceMgr.deviceConnected == TRUE) {
        // Set up our properties.
        deviceProperties();
        deviceRescan();

        // Update the toolbar to show a disconnect string.
        gtk_tool_button_set_label(GTK_TOOL_BUTTON(toolbuttonConnect), _("Disconnect"));

        // Now update the status bar;
        if (DeviceMgr.storagedeviceID == MTP_DEVICE_SINGLE_STORAGE) {
            tmp_string = g_strdup_printf(_("Connected to %s - %d MB free"), DeviceMgr.devicename->str,
                    (int) (DeviceMgr.devicestorage->FreeSpaceInBytes / MEGABYTE));
        } else {
            if (DeviceMgr.devicestorage->StorageDescription != NULL) {
                tmp_string = g_strdup_printf(_("Connected to %s (%s) - %d MB free"),
                        DeviceMgr.devicename->str,
                        DeviceMgr.devicestorage->StorageDescription,
                        (int) (DeviceMgr.devicestorage->FreeSpaceInBytes / MEGABYTE));
            } else {
                tmp_string = g_strdup_printf(_("Connected to %s - %d MB free"),
                        DeviceMgr.devicename->str,
                        (int) (DeviceMgr.devicestorage->FreeSpaceInBytes / MEGABYTE));
            }
        }
        statusBarSet(tmp_string);
        g_free(tmp_string);

        // Now update the filemenu;
        menuText = gtk_bin_get_child(GTK_BIN(fileConnect));
        gtk_label_set_text(GTK_LABEL(menuText), _("Disconnect Device"));

        // Enable the Drag'n'Drop interface for the main window and folder window.
        gmtp_drag_dest_set(scrolledwindowMain);
        gmtp_drag_dest_set(treeviewFolders);

    } else {

        // Update the toolbar to show the Connect String.
        gtk_tool_button_set_label(GTK_TOOL_BUTTON(toolbuttonConnect), _("Connect"));

        // Now update the status bar;
        statusBarSet(_("No device attached"));

        // Now update the filemenu;
        menuText = gtk_bin_get_child(GTK_BIN(fileConnect));
        gtk_label_set_text(GTK_LABEL(menuText), _("Connect Device"));

        // Now update the file list area and disable Drag'n'Drop.
        fileListClear();
        folderListClear();
        gtk_drag_dest_unset(scrolledwindowMain);
        gtk_drag_dest_unset(treeviewFolders);
        setWindowTitle(NULL);
        // Hide the find toolbar if open and force search mode to false.
        gtk_widget_hide(findToolbar);
        gtk_widget_set_sensitive(GTK_WIDGET(cfileAdd), TRUE);
        gtk_widget_set_sensitive(GTK_WIDGET(cfileNewFolder), TRUE);
        inFindMode = FALSE;
    }

    // Update the Toolbar and Menus enabling/disabling the menu items.
    SetToolbarButtonState(DeviceMgr.deviceConnected);
} // on_deviceConnect_activate()
