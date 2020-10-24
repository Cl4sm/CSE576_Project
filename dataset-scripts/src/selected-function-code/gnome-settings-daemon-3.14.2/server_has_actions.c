static gboolean
server_has_actions (void)
{
        gboolean has;
        GList   *caps;
        GList   *l;

        caps = notify_get_server_caps ();
        if (caps == NULL) {
                fprintf (stderr, "Failed to receive server caps.\n");
                return FALSE;
        }

        l = g_list_find_custom (caps, "actions", (GCompareFunc)strcmp);
        has = l != NULL;

        g_list_foreach (caps, (GFunc) g_free, NULL);
        g_list_free (caps);

        return has;
}