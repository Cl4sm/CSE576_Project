static ipp_t *
execute_maintenance_command (const char *printer_name,
                             const char *command,
                             const char *title)
{
        http_t *http;
        GError *error = NULL;
        ipp_t  *request = NULL;
        ipp_t  *response = NULL;
        gchar  *file_name = NULL;
        char   *uri;
        int     fd = -1;

        http = httpConnectEncrypt (cupsServer (),
                                   ippPort (),
                                   cupsEncryption ());

        if (!http)
                return NULL;

        request = ippNewRequest (IPP_PRINT_JOB);

        uri = g_strdup_printf ("ipp://localhost/printers/%s",
                               printer_name);

        ippAddString (request,
                      IPP_TAG_OPERATION,
                      IPP_TAG_URI,
                      "printer-uri",
                      NULL,
                      uri);

        g_free (uri);

        ippAddString (request, IPP_TAG_OPERATION, IPP_TAG_NAME, "job-name",
                      NULL, title);

        ippAddString (request, IPP_TAG_JOB, IPP_TAG_MIMETYPE, "document-format",
                      NULL, "application/vnd.cups-command");

        fd = g_file_open_tmp ("ccXXXXXX", &file_name, &error);

        if (fd != -1) {
                FILE *file;

                file = fdopen (fd, "w");
                fprintf (file, "#CUPS-COMMAND\n");
                fprintf (file, "%s\n", command);
                fclose (file);

                response = cupsDoFileRequest (http, request, "/", file_name);
                g_unlink (file_name);
        } else {
                g_warning ("%s", error->message);
                g_error_free (error);
        }

        g_free (file_name);
        httpClose (http);

        return response;
}