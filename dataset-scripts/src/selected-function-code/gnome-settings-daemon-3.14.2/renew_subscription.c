static gboolean
renew_subscription (gpointer data)
{
        GsdPrintNotificationsManager *manager = (GsdPrintNotificationsManager *) data;
        ipp_attribute_t              *attr = NULL;
        http_t                       *http;
        ipp_t                        *request;
        ipp_t                        *response;
        gint                          num_events = 7;
        static const char * const events[] = {
                "job-created",
                "job-completed",
                "job-state-changed",
                "job-state",
                "printer-added",
                "printer-deleted",
                "printer-state-changed"};

        if ((http = httpConnectEncrypt (cupsServer (), ippPort (),
                                        cupsEncryption ())) == NULL) {
                g_debug ("Connection to CUPS server \'%s\' failed.", cupsServer ());
        } else {
                if (manager->priv->subscription_id >= 0) {
                        request = ippNewRequest (IPP_RENEW_SUBSCRIPTION);
                        ippAddString (request, IPP_TAG_OPERATION, IPP_TAG_URI,
                                     "printer-uri", NULL, "/");
                        ippAddString (request, IPP_TAG_OPERATION, IPP_TAG_NAME,
                                     "requesting-user-name", NULL, cupsUser ());
                        ippAddInteger (request, IPP_TAG_OPERATION, IPP_TAG_INTEGER,
                                      "notify-subscription-id", manager->priv->subscription_id);
                        ippAddInteger (request, IPP_TAG_SUBSCRIPTION, IPP_TAG_INTEGER,
                                      "notify-lease-duration", SUBSCRIPTION_DURATION);
                        ippDelete (cupsDoRequest (http, request, "/"));
                } else {
                        request = ippNewRequest (IPP_CREATE_PRINTER_SUBSCRIPTION);
                        ippAddString (request, IPP_TAG_OPERATION, IPP_TAG_URI,
                                      "printer-uri", NULL,
                                      "/");
                        ippAddString (request, IPP_TAG_OPERATION, IPP_TAG_NAME,
                                      "requesting-user-name", NULL, cupsUser ());
                        ippAddStrings (request, IPP_TAG_SUBSCRIPTION, IPP_TAG_KEYWORD,
                                       "notify-events", num_events, NULL, events);
                        ippAddString (request, IPP_TAG_SUBSCRIPTION, IPP_TAG_KEYWORD,
                                      "notify-pull-method", NULL, "ippget");
                        if (server_is_local (cupsServer ())) {
                                ippAddString (request, IPP_TAG_SUBSCRIPTION, IPP_TAG_URI,
                                              "notify-recipient-uri", NULL, "dbus://");
                        }
                        ippAddInteger (request, IPP_TAG_SUBSCRIPTION, IPP_TAG_INTEGER,
                                       "notify-lease-duration", SUBSCRIPTION_DURATION);
                        response = cupsDoRequest (http, request, "/");

                        if (response != NULL && ippGetStatusCode (response) <= IPP_OK_CONFLICT) {
                                if ((attr = ippFindAttribute (response, "notify-subscription-id",
                                                              IPP_TAG_INTEGER)) == NULL)
                                        g_debug ("No notify-subscription-id in response!\n");
                                else
                                        manager->priv->subscription_id = ippGetInteger (attr, 0);
                        }

                        if (response)
                                ippDelete (response);
                }
                httpClose (http);
        }
        return TRUE;
}