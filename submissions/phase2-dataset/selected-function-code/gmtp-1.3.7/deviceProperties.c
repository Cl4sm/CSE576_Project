void deviceProperties() {
    gint ret;
    gchar *tmp_string;

    // We first see if we have a connected device, and then extract the information from it.
    if (DeviceMgr.deviceConnected == TRUE) {
        // Lets get our information. Let's start with the raw information.
        if (DeviceMgr.rawdevices[DeviceMgr.rawdeviceID].device_entry.vendor == NULL) {
            DeviceMgr.Vendor = g_string_new(_("Unknown"));
        } else {
            DeviceMgr.Vendor = g_string_new(DeviceMgr.rawdevices[DeviceMgr.rawdeviceID].device_entry.vendor);
        }
        if (DeviceMgr.rawdevices[DeviceMgr.rawdeviceID].device_entry.product == NULL) {
            DeviceMgr.Product = g_string_new(_("Unknown"));
        } else {
            DeviceMgr.Product = g_string_new(DeviceMgr.rawdevices[DeviceMgr.rawdeviceID].device_entry.product);
        }
        DeviceMgr.VendorID = DeviceMgr.rawdevices[DeviceMgr.rawdeviceID].device_entry.vendor_id;
        DeviceMgr.ProductID = DeviceMgr.rawdevices[DeviceMgr.rawdeviceID].device_entry.product_id;
        DeviceMgr.BusLoc = DeviceMgr.rawdevices[DeviceMgr.rawdeviceID].bus_location;
        DeviceMgr.DeviceID = DeviceMgr.rawdevices[DeviceMgr.rawdeviceID].devnum;

        // Now lets get our other information.
        // Nice name:
        tmp_string = LIBMTP_Get_Friendlyname(DeviceMgr.device);
        if (tmp_string == NULL) {
            DeviceMgr.devicename = g_string_new(_("N/A"));
        } else {
            DeviceMgr.devicename = g_string_new(tmp_string);
            g_free(tmp_string);
        }
        // Sync Partner
 //       tmp_string = LIBMTP_Get_Syncpartner(DeviceMgr.device);
 //       if (tmp_string == NULL) {
 //           DeviceMgr.syncpartner = g_string_new(_("N/A"));
 //       } else {
 //           DeviceMgr.syncpartner = g_string_new(tmp_string);
 //           g_free(tmp_string);
 //       }
        // Battery Level
        ret = LIBMTP_Get_Batterylevel(DeviceMgr.device, &DeviceMgr.maxbattlevel, &DeviceMgr.currbattlevel);
        if (ret != 0) {
            // Silently ignore. Some devices does not support getting the
            // battery level.
            DeviceMgr.maxbattlevel = 0;
            DeviceMgr.currbattlevel = 0;
            LIBMTP_Clear_Errorstack(DeviceMgr.device);
        }
        // Manufacturer Name.
        tmp_string = LIBMTP_Get_Manufacturername(DeviceMgr.device);
        if (tmp_string == NULL) {
            DeviceMgr.manufacturername = g_string_new(_("N/A"));
        } else {
            DeviceMgr.manufacturername = g_string_new(tmp_string);
            g_free(tmp_string);
        }
        // Model Number,
        tmp_string = LIBMTP_Get_Modelname(DeviceMgr.device);
        if (tmp_string == NULL) {
            DeviceMgr.modelname = g_string_new(_("N/A"));
        } else {
            DeviceMgr.modelname = g_string_new(tmp_string);
            g_free(tmp_string);
        }
        // Serial Number.
        tmp_string = LIBMTP_Get_Serialnumber(DeviceMgr.device);
        if (tmp_string == NULL) {
            DeviceMgr.serialnumber = g_string_new(_("N/A"));
        } else {
            DeviceMgr.serialnumber = g_string_new(tmp_string);
            g_free(tmp_string);
        }
        // Device Version.
        tmp_string = LIBMTP_Get_Deviceversion(DeviceMgr.device);
        if (tmp_string == NULL) {
            DeviceMgr.deviceversion = g_string_new(_("N/A"));
        } else {
            DeviceMgr.deviceversion = g_string_new(tmp_string);
            g_free(tmp_string);
        }
        // Secure Time
 //       ret = LIBMTP_Get_Secure_Time(DeviceMgr.device, &tmp_string);
 //       if (ret == 0 && tmp_string != NULL) {
 //           // tmp_string is a XML fragment, and we need just the date/time out of it.
 //           DeviceMgr.sectime = g_string_new(tmp_string);
 //           g_free(tmp_string);
 //       } else {
 //           // Silently ignore - there may be devices not supporting secure time.
 //           DeviceMgr.sectime = g_string_new(_("N/A"));
 //          LIBMTP_Clear_Errorstack(DeviceMgr.device);
 //       }

        // Storage.
        if (DeviceMgr.devicestorage == NULL) {
            if (LIBMTP_Get_Storage(DeviceMgr.device, 0) < 0) {
                // We have an error getting our storage, so let the user know and then disconnect the device.
                displayError("Failed to get storage parameters from the device - need to disconnect.");
                on_deviceConnect_activate(NULL, NULL);
                return;
            }
            if (DeviceMgr.storagedeviceID == MTP_DEVICE_SINGLE_STORAGE) {
                DeviceMgr.devicestorage = DeviceMgr.device->storage;
            } else {
                DeviceMgr.devicestorage = getCurrentDeviceStoragePtr(DeviceMgr.storagedeviceID);
            }
            // Supported filetypes;
            ret = LIBMTP_Get_Supported_Filetypes(DeviceMgr.device, &DeviceMgr.filetypes, &DeviceMgr.filetypes_len);
            if (ret != 0) {
                LIBMTP_Dump_Errorstack(DeviceMgr.device);
                LIBMTP_Clear_Errorstack(DeviceMgr.device);
            }
        }
    } else {
        // Set to to none.
        g_fprintf(stderr, _("DevicePropeties: How did I get called?\n"));
        DeviceMgr.device = NULL;
    }
}
