    gchar *devicename = NULL;
    gchar *tmp_string = NULL;

    // Get our new device name.
    devicename = displayChangeDeviceNameDialog(DeviceMgr.devicename->str);

    // If the user supplied something, then update the name of the device.
    if (devicename != NULL) {
        // add change to MTP device.
        setDeviceName(devicename);
        g_free(devicename);
        // Attempt to read it back as confirmation that something may of happened.
        tmp_string = LIBMTP_Get_Friendlyname(DeviceMgr.device);
        if (tmp_string == NULL) {
            DeviceMgr.devicename = g_string_new(_("N/A"));
        } else {
            DeviceMgr.devicename = g_string_new(tmp_string);
            g_free(tmp_string);
        }
        // Perform a device Rescan operation to reset all device parameters.
        deviceRescan();
    }
} // end on_editDeviceName_activate()
