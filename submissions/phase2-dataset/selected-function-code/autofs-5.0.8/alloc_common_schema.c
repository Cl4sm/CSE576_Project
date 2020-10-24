static struct ldap_schema *alloc_common_schema(struct ldap_schema *s)
{
	struct ldap_schema *schema;
	char *mc, *ma, *ec, *ea, *va;

	mc = strdup(s->map_class);
	if (!mc)
		return NULL;

	ma = strdup(s->map_attr);
	if (!ma) {
		free(mc);
		return NULL;
	}

	ec = strdup(s->entry_class);
	if (!ec) {
		free(mc);
		free(ma);
		return NULL;
	}

	ea = strdup(s->entry_attr);
	if (!ea) {
		free(mc);
		free(ma);
		free(ec);
		return NULL;
	}

	va = strdup(s->value_attr);
	if (!va) {
		free(mc);
		free(ma);
		free(ec);
		free(ea);
		return NULL;
	}

	schema = malloc(sizeof(struct ldap_schema));
	if (!schema) {
		free(mc);
		free(ma);
		free(ec);
		free(ea);
		free(va);
		return NULL;
	}

	schema->map_class = mc;
	schema->map_attr = ma;
	schema->entry_class = ec;
	schema->entry_attr = ea;
	schema->value_attr = va;

	return schema;
}