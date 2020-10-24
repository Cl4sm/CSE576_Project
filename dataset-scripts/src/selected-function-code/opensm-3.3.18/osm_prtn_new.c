osm_prtn_t *osm_prtn_new(IN const char *name, IN uint16_t pkey)
{
	osm_prtn_t *p = malloc(sizeof(*p));
	if (!p)
		return NULL;

	memset(p, 0, sizeof(*p));
	p->pkey = pkey;
	p->sl = OSM_DEFAULT_SL;
	p->mgrps = NULL;
	p->nmgrps = 0;
	cl_map_construct(&p->full_guid_tbl);
	cl_map_init(&p->full_guid_tbl, 32);
	cl_map_construct(&p->part_guid_tbl);
	cl_map_init(&p->part_guid_tbl, 32);

	if (name && *name)
		strncpy(p->name, name, sizeof(p->name));
	else
		snprintf(p->name, sizeof(p->name), "%04x", cl_ntoh16(pkey));

	return p;
}