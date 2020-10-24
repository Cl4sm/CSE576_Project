    FILE* fd;
    uint32_t numtracks = playlist->no_tracks;
    uint32_t *tracks = playlist->tracks;
    uint32_t trackid = 0;
    gchar* trackname = NULL;

    // Open the file to save it to...
    fd = fopen(filename, "w");
    if (fd == NULL) {
        g_fprintf(stderr, _("Couldn't save playlist file %s\n"), filename);
        displayError(_("Couldn't save playlist file\n"));
    } else {
        fprintf(fd, "#GMTPPLA: %s\n", playlist->name);
        fflush(fd);
        while (numtracks--) {
            trackid = *tracks++;
            // We now have our track id. Let's form the complete path to the file including the filename.
            // Then store that string in the file...
            trackname = getFullFilename(trackid);
            if (trackname != NULL) {
                fprintf(fd, "%s\n", trackname);
                g_free(trackname);
                trackname = NULL;
            }
        }
        fclose(fd);
    }
}
