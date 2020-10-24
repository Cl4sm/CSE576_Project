static gboolean
update_ignore_paths (GSList **ignore_paths,
                     const gchar *mount_path,
                     gboolean ignore)
{
        GSList *found;
        gchar *path_to_remove;
        
        found = g_slist_find_custom (*ignore_paths, mount_path, (GCompareFunc) ignore_path_compare);
        
        if (ignore && (found == NULL)) {
                *ignore_paths = g_slist_prepend (*ignore_paths, g_strdup (mount_path));
                return TRUE;
        }
        
        if (!ignore && (found != NULL)) {
                path_to_remove = found->data;
                *ignore_paths = g_slist_remove (*ignore_paths, path_to_remove);
                g_free (path_to_remove);
                return TRUE;
        }
                
        return FALSE;
}