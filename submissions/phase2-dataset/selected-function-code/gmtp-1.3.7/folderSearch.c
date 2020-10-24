    FileListStruc *filestruc = NULL;
    gchar *tmpstring = NULL;
    while (folderptr != NULL) {
        // Make search case insensitive.
        tmpstring = g_utf8_strup(folderptr->name, -1);
        if (g_pattern_match_string(pspec, tmpstring) == TRUE) {
            // We have found a matching string...
            filestruc = g_malloc(sizeof (FileListStruc));
            if (filestruc == NULL) {
                if (tmpstring != NULL) {
                    g_free(tmpstring);
                    tmpstring = NULL;
                }
                g_fprintf(stderr, _("malloc in foldersearch failed\n"));
                displayError(_("Failed searching? 'malloc in foldersearch'\n"));
                return;
            }
            filestruc->filename = g_strdup(folderptr->name);
            filestruc->filesize = 0;
            filestruc->isFolder = TRUE;
            filestruc->itemid = folderptr->folder_id;
#if defined(LIBMTP_FILETYPE_FOLDER)
            filestruc->filetype = LIBMTP_FILETYPE_FOLDER;
#else
            filestruc->filetype = LIBMTP_FILETYPE_UNKNOWN;
#endif
            filestruc->location = getFullFolderPath(folderptr->parent_id);
            *(list) = g_slist_append(*(list), filestruc);
        }
        if (tmpstring != NULL) {
            g_free(tmpstring);
            tmpstring = NULL;
        }
        // Search child if present;
        if (folderptr->child != NULL) {
            folderSearch(pspec, list, folderptr->child);
        }
        folderptr = folderptr->sibling;
    }
}
