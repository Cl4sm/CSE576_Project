static char *
get_object_path_for_token (GsdSmartcardService *self,
                           PK11SlotInfo        *card_slot)
{
        char *object_path;
        char *escaped_library_path;
        SECMODModule *driver;
        CK_SLOT_ID slot_id;

        driver = PK11_GetModule (card_slot);
        slot_id = PK11_GetSlotID (card_slot);

        escaped_library_path = gsd_smartcard_utils_escape_object_path (driver->dllName);

        object_path = g_strdup_printf ("%s/token_from_%s_slot_%lu",
                                       GSD_SMARTCARD_MANAGER_TOKENS_DBUS_PATH,
                                       escaped_library_path,
                                       (gulong) slot_id);
        g_free (escaped_library_path);

        return object_path;
}