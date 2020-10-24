    gchar* fullfilename = NULL;

    // Maybe something went wrong, so we disconnected. If so, then simple exit....
    if (DeviceMgr.deviceConnected == FALSE){
        return;
    }
    if(progressDialog_killed == TRUE){
	    return;
    }

    //displayProgressBar(_("File download"));
    setProgressFilename(filename);
    // Download the file based on the objectID.
    fullfilename = g_strdup_printf("%s/%s", Preferences.fileSystemDownloadPath->str, filename);
    if (LIBMTP_Get_File_To_File(DeviceMgr.device, objectID, fullfilename, fileprogress, NULL) != 0) {
        g_fprintf(stderr, _("\nError getting file from MTP device.\n"));
        displayError(_("Error getting file from MTP device."));
        LIBMTP_Dump_Errorstack(DeviceMgr.device);
        LIBMTP_Clear_Errorstack(DeviceMgr.device);
    }
    //destroyProgressBar();
    g_free(fullfilename);
}
