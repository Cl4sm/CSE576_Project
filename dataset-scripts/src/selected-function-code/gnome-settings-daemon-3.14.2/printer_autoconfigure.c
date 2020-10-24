static void
printer_autoconfigure (gchar *printer_name)
{
        gchar *commands;
        gchar *commands_lowercase;
        ipp_t *response = NULL;

        if (!printer_name)
                return;

        commands = get_dest_attr (printer_name, "printer-commands");
        commands_lowercase = g_ascii_strdown (commands, -1);

        if (g_strrstr (commands_lowercase, "autoconfigure")) {
                response = execute_maintenance_command (printer_name,
                                                        "AutoConfigure",
                                                        ("Automatic configuration"));
                if (response) {
                        if (ippGetState (response) == IPP_ERROR)
                                g_warning ("An error has occured during automatic configuration of new printer.");
                        ippDelete (response);
                }
        }
        g_free (commands);
        g_free (commands_lowercase);
}