    gint error;
    if (DeviceMgr.deviceConnected == TRUE) {
        // We must be wanting to disconnect the device.
        return deviceDisconnect();
    } else {
        error = LIBMTP_Detect_Raw_Devices(&DeviceMgr.rawdevices, &DeviceMgr.numrawdevices);
        switch (error) {
            case LIBMTP_ERROR_NONE:
                break;
            case LIBMTP_ERROR_NO_DEVICE_ATTACHED:
                g_fprintf(stderr, _("Detect: No raw devices found.\n"));
                displayError(_("Detect: No raw devices found.\n"));
                return MTP_GENERAL_FAILURE;
            case LIBMTP_ERROR_CONNECTING:
                g_fprintf(stderr, _("Detect: There has been an error connecting. \n"));
                displayError(_("Detect: There has been an error connecting. \n"));
                return MTP_GENERAL_FAILURE;
            case LIBMTP_ERROR_MEMORY_ALLOCATION:
                g_fprintf(stderr, _("Detect: Encountered a Memory Allocation Error. \n"));
                displayError(_("Detect: Encountered a Memory Allocation Error. \n"));
                return MTP_GENERAL_FAILURE;
            default:
                // Some other generic error, so let's exit.
                g_fprintf(stderr, _("Detect: There has been an error connecting. \n"));
                displayError(_("Detect: There has been an error connecting. \n"));
                return MTP_GENERAL_FAILURE;
        }
        // We have at least 1 raw device, so we connect to the first device.
        if (DeviceMgr.numrawdevices > 1) {
            DeviceMgr.rawdeviceID = displayMultiDeviceDialog();
            if (!Preferences.use_alt_access_method) {
                DeviceMgr.device = LIBMTP_Open_Raw_Device(&DeviceMgr.rawdevices[DeviceMgr.rawdeviceID]);
            } else {
                DeviceMgr.device = LIBMTP_Open_Raw_Device_Uncached(&DeviceMgr.rawdevices[DeviceMgr.rawdeviceID]);
            }
        } else {
            // Connect to the first device.
            if (!Preferences.use_alt_access_method) {
                DeviceMgr.device = LIBMTP_Open_Raw_Device(&DeviceMgr.rawdevices[0]);
            } else {
                DeviceMgr.device = LIBMTP_Open_Raw_Device_Uncached(&DeviceMgr.rawdevices[0]);
            }
            DeviceMgr.rawdeviceID = 0;
        }
        if (DeviceMgr.device == NULL) {
            g_fprintf(stderr, _("Detect: Unable to open raw device?\n"));
            displayError(_("Detect: Unable to open raw device?\n"));
            LIBMTP_Dump_Errorstack(DeviceMgr.device);
            LIBMTP_Clear_Errorstack(DeviceMgr.device);
            DeviceMgr.deviceConnected = FALSE;
            return MTP_GENERAL_FAILURE;
        }

        LIBMTP_Dump_Errorstack(DeviceMgr.device);
        LIBMTP_Clear_Errorstack(DeviceMgr.device);
        DeviceMgr.deviceConnected = TRUE;

        // We have a successful device connect, but lets check for multiple storageIDs.
        if (DeviceMgr.device->storage == NULL) {
            g_fprintf(stderr, _("Detect: No available Storage found on device?\n"));
            displayError(_("Detect: No available Storage found on device?\n"));
            LIBMTP_Dump_Errorstack(DeviceMgr.device);
            LIBMTP_Clear_Errorstack(DeviceMgr.device);
            deviceDisconnect();
            return MTP_GENERAL_FAILURE;
        }
        if (DeviceMgr.device->storage->next != NULL) {
            // Oops we have multiple storage IDs.
            DeviceMgr.storagedeviceID = displayDeviceStorageDialog();
        } else {
            DeviceMgr.storagedeviceID = MTP_DEVICE_SINGLE_STORAGE;
        }
        currentFolderID = 0;
        DeviceMgr.devicename = NULL;
        DeviceMgr.manufacturername = NULL;
        DeviceMgr.modelname = NULL;
        DeviceMgr.serialnumber = NULL;
        DeviceMgr.deviceversion = NULL;
        //DeviceMgr.syncpartner = NULL;
        //DeviceMgr.sectime = NULL;
        DeviceMgr.devcert = NULL;
        DeviceMgr.Vendor = NULL;
        DeviceMgr.Product = NULL;
        DeviceMgr.devicestorage = NULL;

        // if in alt connection mode;
        if (Preferences.use_alt_access_method) {
            if (stackFolderIDs != NULL) {
                g_queue_free(stackFolderIDs);
            }
            stackFolderIDs = g_queue_new();
            if (stackFolderNames != NULL) {
                g_queue_free(stackFolderNames);
            }
            stackFolderNames = g_queue_new();
        }
        return MTP_SUCCESS;
    }
}
