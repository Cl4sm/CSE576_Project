GConfSchema *
gconf_client_get_schema_for_key (GConfClient *client, const char *key)
{
	GConfEntry *entry;
	const char *schema_name;
	GConfSchema *schema = NULL;

	entry = gconf_client_get_entry (client, key, NULL, TRUE, NULL);
	schema_name = gconf_entry_get_schema_name (entry);

	if (schema_name == NULL)
		goto out;

	schema = gconf_client_get_schema (client, schema_name, NULL);

out:
	if (entry)
		gconf_entry_free (entry);

	return schema;
}