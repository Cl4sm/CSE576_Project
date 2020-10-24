gint
gconf_pk_can_set (const gchar *key, gboolean mandatory)
{
        time_t now;
	GHashTable **cache;
	CacheEntry *entry;

        time (&now);
	cache = mandatory ? &mandatory_cache : &defaults_cache;
	if (!*cache)
		*cache = g_hash_table_new (g_str_hash, g_str_equal);
	entry = (CacheEntry *) g_hash_table_lookup (*cache, key);
	if (!entry) {
		entry = g_new0 (CacheEntry, 1);
		g_hash_table_insert (*cache, (char *) key, entry);
	}
        if (ABS (now - entry->last_refreshed) > CACHE_VALIDITY_SEC) {
        	entry->can_set = can_set (key, mandatory);
                entry->last_refreshed = now;
        }

        return entry->can_set;
}