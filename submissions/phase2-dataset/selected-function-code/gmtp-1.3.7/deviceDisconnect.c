guint deviceDisconnect() {
    if (DeviceMgr.deviceConnected == FALSE) {
        DeviceMgr.deviceConnected = FALSE;
        return MTP_NO_DEVICE;
    } else {
        DeviceMgr.deviceConnected = FALSE;
        LIBMTP_Release_Device(DeviceMgr.device);
        g_free(DeviceMgr.rawdevices);
        // Now clean up the dymanic data in struc that get's loaded when displaying the properties dialog.
        if (DeviceMgr.devicename != NULL) g_string_free(DeviceMgr.devicename, TRUE);
        if (DeviceMgr.manufacturername != NULL) g_string_free(DeviceMgr.manufacturername, TRUE);
        if (DeviceMgr.modelname != NULL) g_string_free(DeviceMgr.modelname, TRUE);
        if (DeviceMgr.serialnumber != NULL) g_string_free(DeviceMgr.serialnumber, TRUE);
        if (DeviceMgr.deviceversion != NULL) g_string_free(DeviceMgr.deviceversion, TRUE);
        //if (DeviceMgr.syncpartner != NULL) g_string_free(DeviceMgr.syncpartner, TRUE);
        //if (DeviceMgr.sectime != NULL) g_string_free(DeviceMgr.sectime, TRUE);
        if (DeviceMgr.devcert != NULL) g_string_free(DeviceMgr.devcert, TRUE);
        if (DeviceMgr.Vendor != NULL) g_string_free(DeviceMgr.Vendor, TRUE);
        if (DeviceMgr.Product != NULL) g_string_free(DeviceMgr.Product, TRUE);
        g_free(DeviceMgr.filetypes);
        return MTP_SUCCESS;
    }
}
