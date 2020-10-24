static gboolean
gsd_smartcard_service_handle_get_login_token (GsdSmartcardServiceManager *manager,
                                              GDBusMethodInvocation      *invocation)
{
        GsdSmartcardService *self = GSD_SMARTCARD_SERVICE (manager);
        GsdSmartcardServicePrivate *priv = self->priv;
        PK11SlotInfo *card_slot;
        char *object_path;

        card_slot = gsd_smartcard_manager_get_login_token (priv->smartcard_manager);

        if (card_slot == NULL) {
                g_dbus_method_invocation_return_error (invocation,
                                                       GSD_SMARTCARD_MANAGER_ERROR,
                                                       GSD_SMARTCARD_MANAGER_ERROR_FINDING_SMARTCARD,
                                                       _("User was not logged in with smartcard."));

                return TRUE;
        }

        object_path = get_object_path_for_token (self, card_slot);
        gsd_smartcard_service_manager_complete_get_login_token (manager,
                                                                invocation,
                                                                object_path);
        g_free (object_path);

        return TRUE;
}