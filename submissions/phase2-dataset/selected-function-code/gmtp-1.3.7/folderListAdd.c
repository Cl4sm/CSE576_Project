    GtkTreeIter rowIter;
    GdkPixbuf *image = NULL;

    if (parent == NULL) {
        // Add in the root node.
        image = gdk_pixbuf_new_from_file(file_folder_png, NULL);
        // Now add in the row information.
        gtk_tree_store_append(GTK_TREE_STORE(folderList), &rowIter, parent);
        gtk_tree_store_set(GTK_TREE_STORE(folderList), &rowIter,
                //COL_FOL_NAME, folders->name,
                COL_FOL_NAME_HIDDEN, "/",
                COL_FOL_ID, 0,
                COL_FOL_ICON, image,
                -1);

        // Indicate we are done with this image.
        g_object_unref(image);
        folderListAdd(folders, &rowIter);
        return TRUE;
    }

    while (folders != NULL) {
        // Only add in folder if it's in the current storage device.
        if (folders->storage_id == DeviceMgr.devicestorage->id) {

            image = gdk_pixbuf_new_from_file(file_folder_png, NULL);
            // Now add in the row information.
            gtk_tree_store_append(GTK_TREE_STORE(folderList), &rowIter, parent);
            gtk_tree_store_set(GTK_TREE_STORE(folderList), &rowIter,
                    //COL_FOL_NAME, folders->name,
                    COL_FOL_NAME_HIDDEN, folders->name,
                    COL_FOL_ID, folders->folder_id,
                    COL_FOL_ICON, image,
                    -1);

            // Indicate we are done with this image.
            g_object_unref(image);
            if (folders->child != NULL) {
                // Call our child.
                folderListAdd(folders->child, &rowIter);
            }
        }
        folders = folders->sibling;
    }
    gtk_tree_view_expand_all(GTK_TREE_VIEW(treeviewFolders));
    gtk_tree_view_column_set_sort_order(folderColumn, GTK_SORT_ASCENDING);
    return TRUE;
}
