    gchar *tmp_string;

    // We confirm our device properties, this should setup the device structure information we use below.
    deviceProperties();

    // Update the status bar with our information.
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
            tmp_string = g_strdup_printf(_("Connected to %s - %d MB free"), DeviceMgr.devicename->str,
                    (int) (DeviceMgr.devicestorage->FreeSpaceInBytes / MEGABYTE));
        }
    }
    statusBarSet(tmp_string);
    g_free(tmp_string);

    // No idea how this could come about, but we should take it into account so we don't have a
    // memleak due to recreating the window multiple times.
    if (windowPropDialog != NULL) {
        gtk_widget_hide(windowPropDialog);
        gtk_widget_destroy(windowPropDialog);
    }

    // Create and show the dialog box.
    windowPropDialog = create_windowProperties();
    gtk_widget_show(GTK_WIDGET(windowPropDialog));
} // end on_deviceProperties_activate()
