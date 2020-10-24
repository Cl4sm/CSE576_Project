static void
load_nss (GsdSmartcardManager *self)
{
        GsdSmartcardManagerPrivate *priv = self->priv;
        SECStatus status = SECSuccess;
        static const guint32 flags = NSS_INIT_READONLY
                                   | NSS_INIT_FORCEOPEN
                                   | NSS_INIT_NOROOTINIT
                                   | NSS_INIT_OPTIMIZESPACE
                                   | NSS_INIT_PK11RELOAD;

        g_debug ("attempting to load NSS database '%s'",
                 GSD_SMARTCARD_MANAGER_NSS_DB);

        PR_Init (PR_USER_THREAD, PR_PRIORITY_NORMAL, 0);

        status = NSS_Initialize (GSD_SMARTCARD_MANAGER_NSS_DB,
                                 "", "", SECMOD_DB, flags);

        if (status != SECSuccess) {
                gsize error_message_size;
                char *error_message;

                error_message_size = PR_GetErrorTextLength ();

                if (error_message_size == 0) {
                        g_debug ("NSS security system could not be initialized");
                } else {
                        error_message = g_alloca (error_message_size);
                        PR_GetErrorText (error_message);

                        g_debug ("NSS security system could not be initialized - %s",
                                 error_message);
                }
                priv->nss_is_loaded = FALSE;
                return;

        }

        g_debug ("NSS database '%s' loaded", GSD_SMARTCARD_MANAGER_NSS_DB);
        priv->nss_is_loaded = TRUE;
}