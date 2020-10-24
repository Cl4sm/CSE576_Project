    LIBMTP_filesampledata_t *albumart;
    gint ret;
    uint64_t filesize;
    uint8_t *imagedata = NULL;
    struct stat statbuff;
    FILE* fd;

    if (stat(filename, &statbuff) == -1) {
        perror("stat");
        return;
    }
    filesize = (uint64_t) statbuff.st_size;
    imagedata = g_malloc(filesize * sizeof (uint8_t));
    if (imagedata == NULL) {
        g_fprintf(stderr, _("ERROR: Failed memory allocation in albumAddArt()\n"));
        return;
    }
    fd = fopen(filename, "r");
    if (fd == NULL) {
        g_fprintf(stderr, _("Couldn't open image file %s\n"), filename);
        g_free(imagedata);
        return;
    } else {
        size_t i = fread(imagedata, filesize, 1, fd);
        fclose(fd);
        if(i != 1 ){
            g_fprintf(stderr, _("Couldn't open image file %s\n"), filename);
            g_free(imagedata);
            return;
        }
    }

    albumart = LIBMTP_new_filesampledata_t();
    albumart->data = (gchar *) imagedata;
    albumart->size = filesize;
    albumart->filetype = find_filetype(basename(filename));

    ret = LIBMTP_Send_Representative_Sample(DeviceMgr.device, album_id, albumart);
    if (ret != 0) {
        g_fprintf(stderr, _("Couldn't send album art\n"));
        displayError(_("Couldn't send album art\n"));
        LIBMTP_Dump_Errorstack(DeviceMgr.device);
        LIBMTP_Clear_Errorstack(DeviceMgr.device);
    }
    g_free(imagedata);
    albumart->data = NULL;

    // Adjust device storage.
    DeviceMgr.devicestorage->FreeSpaceInBytes -= filesize;
    DeviceMgr.devicestorage->FreeSpaceInObjects--;
    LIBMTP_destroy_filesampledata_t(albumart);
}
