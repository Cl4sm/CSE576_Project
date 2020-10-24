static char **
get_thumbnail_dirs (void)
{
        GPtrArray *array;
        char *path;

        array = g_ptr_array_new ();

        /* check new XDG cache */
        path = g_build_filename (g_get_user_cache_dir (),
                                 "thumbnails",
                                 "normal",
                                 NULL);
        g_ptr_array_add (array, path);

        path = g_build_filename (g_get_user_cache_dir (),
                                 "thumbnails",
                                 "large",
                                 NULL);
        g_ptr_array_add (array, path);

        path = g_build_filename (g_get_user_cache_dir (),
                                 "thumbnails",
                                 "fail",
                                 "gnome-thumbnail-factory",
                                 NULL);
        g_ptr_array_add (array, path);

        /* cleanup obsolete locations too */
        path = g_build_filename (g_get_home_dir (),
                                 ".thumbnails",
                                 "normal",
                                 NULL);
        g_ptr_array_add (array, path);

        path = g_build_filename (g_get_home_dir (),
                                 ".thumbnails",
                                 "large",
                                 NULL);
        g_ptr_array_add (array, path);

        path = g_build_filename (g_get_home_dir (),
                                 ".thumbnails",
                                 "fail",
                                 "gnome-thumbnail-factory",
                                 NULL);
        g_ptr_array_add (array, path);

        g_ptr_array_add (array, NULL);

        return (char **) g_ptr_array_free (array, FALSE);
}