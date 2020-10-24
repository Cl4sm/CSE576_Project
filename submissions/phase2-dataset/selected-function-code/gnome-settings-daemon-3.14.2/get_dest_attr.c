static char *
get_dest_attr (const char *dest_name,
               const char *attr)
{
        cups_dest_t *dests;
        int          num_dests;
        cups_dest_t *dest;
        const char  *value;
        char        *ret;

        if (dest_name == NULL)
                return NULL;

        ret = NULL;

        num_dests = cupsGetDests (&dests);
        if (num_dests < 1) {
                g_debug ("Unable to get printer destinations");
                return NULL;
        }

        dest = cupsGetDest (dest_name, NULL, num_dests, dests);
        if (dest == NULL) {
                g_debug ("Unable to find a printer named '%s'", dest_name);
                goto out;
        }

        value = cupsGetOption (attr, dest->num_options, dest->options);
        if (value == NULL) {
                g_debug ("Unable to get %s for '%s'", attr, dest_name);
                goto out;
        }
        ret = g_strdup (value);
out:
        cupsFreeDests (num_dests, dests);

        return ret;
}