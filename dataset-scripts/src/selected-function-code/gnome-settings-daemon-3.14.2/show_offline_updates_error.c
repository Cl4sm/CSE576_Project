static void
show_offline_updates_error (GsdUpdatesManager *manager)
{
        const gchar *title;
        gboolean show_geeky = FALSE;
        GString *msg;
        GtkWidget *dialog;

        /* TRANSLATORS: this is when the offline update failed */
        title = _("Failed To Update");
        msg = g_string_new ("");
        switch (pk_error_get_code (manager->priv->offline_update_error)) {
        case PK_ERROR_ENUM_UNFINISHED_TRANSACTION:
                /* TRANSLATORS: the transaction could not be completed
                 * as a previous transaction was unfinished */
                g_string_append (msg, _("A previous update was unfinished."));
                show_geeky = TRUE;
                break;
        case PK_ERROR_ENUM_PACKAGE_DOWNLOAD_FAILED:
        case PK_ERROR_ENUM_NO_CACHE:
        case PK_ERROR_ENUM_NO_NETWORK:
        case PK_ERROR_ENUM_NO_MORE_MIRRORS_TO_TRY:
        case PK_ERROR_ENUM_CANNOT_FETCH_SOURCES:
                /* TRANSLATORS: the package manager needed to download
                 * something with no network available */
                g_string_append (msg, _("Network access was required but not available."));
                break;
        case PK_ERROR_ENUM_BAD_GPG_SIGNATURE:
        case PK_ERROR_ENUM_CANNOT_UPDATE_REPO_UNSIGNED:
        case PK_ERROR_ENUM_GPG_FAILURE:
        case PK_ERROR_ENUM_MISSING_GPG_SIGNATURE:
        case PK_ERROR_ENUM_PACKAGE_CORRUPT:
                /* TRANSLATORS: if the package is not signed correctly
                 *  */
                g_string_append (msg, _("An update was not signed in the correct way."));
                show_geeky = TRUE;
                break;
        case PK_ERROR_ENUM_DEP_RESOLUTION_FAILED:
        case PK_ERROR_ENUM_FILE_CONFLICTS:
        case PK_ERROR_ENUM_INCOMPATIBLE_ARCHITECTURE:
        case PK_ERROR_ENUM_PACKAGE_CONFLICTS:
                /* TRANSLATORS: the transaction failed in a way the user
                 * probably cannot comprehend. Package management systems
                 * really are teh suck.*/
                g_string_append (msg, _("The update could not be completed."));
                show_geeky = TRUE;
                break;
        case PK_ERROR_ENUM_TRANSACTION_CANCELLED:
                /* TRANSLATORS: the user aborted the update manually */
                g_string_append (msg, _("The update was cancelled."));
                break;
        case PK_ERROR_ENUM_NO_PACKAGES_TO_UPDATE:
        case PK_ERROR_ENUM_UPDATE_NOT_FOUND:
                /* TRANSLATORS: the user must have updated manually after
                 * the updates were prepared */
                g_string_append (msg, _("An offline update was requested but no packages required updating."));
                break;
        case PK_ERROR_ENUM_NO_SPACE_ON_DEVICE:
                /* TRANSLATORS: we ran out of disk space */
                g_string_append (msg, _("No space was left on the drive."));
                break;
        case PK_ERROR_ENUM_PACKAGE_FAILED_TO_BUILD:
        case PK_ERROR_ENUM_PACKAGE_FAILED_TO_INSTALL:
        case PK_ERROR_ENUM_PACKAGE_FAILED_TO_REMOVE:
                /* TRANSLATORS: the update process failed in a general
                 * way, usually this message will come from source distros
                 * like gentoo */
                g_string_append (msg, _("An update failed to install correctly."));
                show_geeky = TRUE;
                break;
        default:
                /* TRANSLATORS: We didn't handle the error type */
                g_string_append (msg, _("The offline update failed in an unexpected way."));
                show_geeky = TRUE;
                break;
        }
        if (show_geeky) {
                g_string_append_printf (msg, "\n%s\n\n%s",
                                        /* TRANSLATORS: these are geeky messages from the
                                         * package manager no mortal is supposed to understand,
                                         * but google might know what they mean */
                                        _("Detailed errors from the package manager follow:"),
                                        pk_error_get_details (manager->priv->offline_update_error));
        }
        dialog = gtk_message_dialog_new (NULL,
                                         0,
                                         GTK_MESSAGE_INFO,
                                         GTK_BUTTONS_CLOSE,
                                         "%s", title);
        gtk_message_dialog_format_secondary_text (GTK_MESSAGE_DIALOG (dialog),
                                                  "%s", msg->str);
        g_signal_connect_swapped (dialog, "response",
                                  G_CALLBACK (gtk_widget_destroy),
                                  dialog);
        gtk_widget_show (dialog);
        clear_offline_updates_message ();
        g_string_free (msg, TRUE);
}