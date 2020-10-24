static int find_query_dn(unsigned logopt, LDAP *ldap, struct lookup_context *ctxt)
{
	struct ldap_schema *schema;
	unsigned int i;

	if (ctxt->schema)
		return 0;

	for (i = 0; i < common_schema_count; i++) {
		const char *class = common_schema[i].map_class;
		const char *key = common_schema[i].map_attr;
		if (get_query_dn(logopt, ldap, ctxt, class, key)) {
			schema = alloc_common_schema(&common_schema[i]);
			if (!schema) {
				error(logopt, MODPREFIX "failed to allocate schema");
				return 0;
			}
			ctxt->schema = schema;
			return 1;
		}
	}

	return 0;
}