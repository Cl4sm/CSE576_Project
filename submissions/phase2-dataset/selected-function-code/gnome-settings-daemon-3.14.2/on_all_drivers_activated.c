static void
on_all_drivers_activated (GsdSmartcardManager *self,
                          GAsyncResult        *result,
                          GTask               *task)
{
        GError *error = NULL;
        gboolean driver_activated;
        PK11SlotInfo *login_token;

        driver_activated = activate_all_drivers_async_finish (self, result, &error);

        if (!driver_activated) {
                g_task_return_error (task, error);
                return;
        }

        login_token = gsd_smartcard_manager_get_login_token (self);

        if (login_token || g_getenv ("PKCS11_LOGIN_TOKEN_NAME") != NULL) {
                /* The card used to log in was removed before login completed.
                 * Do removal action immediately
                 */
                if (!login_token || !PK11_IsPresent (login_token))
                        gsd_smartcard_manager_do_remove_action (self);
        }

        g_task_return_boolean (task, TRUE);
}