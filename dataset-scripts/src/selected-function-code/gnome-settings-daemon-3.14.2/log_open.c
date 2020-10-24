static void
log_open (void)
{
        char *toggle_filename;
        char *log_filename;
        struct stat st;

        if (log_file)
                return;

        toggle_filename = g_build_filename (g_get_home_dir (), "gsd-debug-randr", NULL);
        log_filename = g_build_filename (g_get_home_dir (), "gsd-debug-randr.log", NULL);

        if (stat (toggle_filename, &st) != 0)
                goto out;

        log_file = fopen (log_filename, "a");

        if (log_file && ftell (log_file) == 0)
                fprintf (log_file, "To keep this log from being created, please rm ~/gsd-debug-randr\n");

out:
        g_free (toggle_filename);
        g_free (log_filename);
}