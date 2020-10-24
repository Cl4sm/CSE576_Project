void deviceRescan() {
    gchar* tmp_string;
    //g_print("You selected deviceRescan\n");
    // First we clear the file and folder list...
    fileListClear();
    folderListClear();
    // Now clear the folder/file structures.
    if (deviceFolders != NULL)
        LIBMTP_destroy_folder_t(deviceFolders);
    if (deviceFiles != NULL)
        clearDeviceFiles(deviceFiles);
    // Add in track, playlist globals as well.
    deviceFolders = NULL;
    deviceFiles = NULL;
    // Now get started.
    if (DeviceMgr.deviceConnected) {
        // Get a list of folder on the device. (Note: this may fail on some devices, and we end up with zero folders being listed)
        deviceFolders = LIBMTP_Get_Folder_List_For_Storage(DeviceMgr.device, DeviceMgr.devicestorage->id);
        if (deviceFolders == NULL) {
            LIBMTP_Dump_Errorstack(DeviceMgr.device);
            LIBMTP_Clear_Errorstack(DeviceMgr.device);
        }
        // Now get a list of files from the device.
        if (!Preferences.use_alt_access_method) {
            deviceFiles = LIBMTP_Get_Filelisting_With_Callback(DeviceMgr.device, NULL, NULL);
        } else {
            // Alternate access method ONLY gets the files for the CURRENT FOLDER ONLY. This should help some Android based devices.
            deviceFiles = LIBMTP_Get_Files_And_Folders(DeviceMgr.device, DeviceMgr.devicestorage->id, currentFolderID);
        }
        if (deviceFiles == NULL) {
            LIBMTP_Dump_Errorstack(DeviceMgr.device);
            LIBMTP_Clear_Errorstack(DeviceMgr.device);
        }
        devicePlayLists = getPlaylists();
        deviceTracks = getTracks();
        fileListAdd();
        folderListAdd(deviceFolders, NULL);
        // Now update the storage...
        if (DeviceMgr.devicestorage == NULL) {
            if (LIBMTP_Get_Storage(DeviceMgr.device, 0) < 0) {
                // We have an error getting our storage, so let the user know and then disconnect the device.
                displayError(_("Failed to get storage parameters from the device - need to disconnect."));
                on_deviceConnect_activate(NULL, NULL);
                return;
            }
            if (DeviceMgr.storagedeviceID == MTP_DEVICE_SINGLE_STORAGE) {
                DeviceMgr.devicestorage = DeviceMgr.device->storage;
            } else {
                DeviceMgr.devicestorage = getCurrentDeviceStoragePtr(DeviceMgr.storagedeviceID);
            }
        }
        // Update the status bar.
        if (DeviceMgr.storagedeviceID == MTP_DEVICE_SINGLE_STORAGE) {
            tmp_string = g_strdup_printf(_("Connected to %s - %d MB free"), DeviceMgr.devicename->str,
                    (int) (DeviceMgr.devicestorage->FreeSpaceInBytes / MEGABYTE));
        } else {
            if (DeviceMgr.devicestorage->StorageDescription != NULL) {
                tmp_string = g_strdup_printf(_("Connected to %s (%s) - %d MB free"), DeviceMgr.devicename->str,
                        DeviceMgr.devicestorage->StorageDescription,
                        (int) (DeviceMgr.devicestorage->FreeSpaceInBytes / MEGABYTE));
            } else {
                tmp_string = g_strdup_printf(_("Connected to %s - %d MB free"), DeviceMgr.devicename->str,
                        (int) (DeviceMgr.devicestorage->FreeSpaceInBytes / MEGABYTE));
            }
        }
        statusBarSet(tmp_string);
        g_free(tmp_string);
    } else {
        g_fprintf(stderr, _("Rescan: How did I get called?\n"));
    }
}
