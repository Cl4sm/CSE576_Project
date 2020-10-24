static gchar *
create_name (gchar *device_id)
{
        cups_dest_t *dests;
        gboolean     already_present = FALSE;
        gchar       *name = NULL;
        gchar       *new_name = NULL;
        gint         num_dests;
        gint         name_index = 2;
        gint         j;

        g_return_val_if_fail (device_id != NULL, NULL);

        name = get_tag_value (device_id, "mdl");
        if (!name)
                name = get_tag_value (device_id, "model");

        if (name)
                name = g_strcanon (name, ALLOWED_CHARACTERS, '-');

        num_dests = cupsGetDests (&dests);
        do {
                if (already_present) {
                        new_name = g_strdup_printf ("%s-%d", name, name_index);
                        name_index++;
                } else {
                        new_name = g_strdup (name);
                }

                already_present = FALSE;
                for (j = 0; j < num_dests; j++)
                        if (g_strcmp0 (dests[j].name, new_name) == 0)
                                already_present = TRUE;

                if (already_present) {
                        g_free (new_name);
                } else {
                        g_free (name);
                        name = new_name;
                }
        } while (already_present);
        cupsFreeDests (num_dests, dests);

        return name;
}