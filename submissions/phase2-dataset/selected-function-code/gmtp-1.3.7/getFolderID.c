    gchar** pathcomponents;
    if (g_ascii_strcasecmp(foldername, ".") == 0) {
        // We have a root directory...
        return 0;
    }
    // Get the first component of the foldername.
    pathcomponents = g_strsplit(foldername, "/", 2);
    while (folderptr != NULL) {
        if (g_ascii_strcasecmp(pathcomponents[0], folderptr->name) == 0) {
            // We have found our path...
            // If we have of the path to process then...
            if (pathcomponents[1] != NULL) {
                return (getFolderID(folderptr->child, pathcomponents[1]));
            } else {
                return folderptr->folder_id;
            }
        } else {
            folderptr = folderptr->sibling;
        }
    }
    return -1;
}
