List *get_purchasable_unit_lib_entries( const char *nationid, 
				const char *uclassid, const Date *date )
{
	List *l = list_create( LIST_NO_AUTO_DELETE, LIST_NO_CALLBACK );
	Unit_Lib_Entry *e = NULL;
	
	if (l == NULL) {
		fprintf( stderr, tr("Out of memory\n") );
		return NULL;
	}
	
	list_reset(unit_lib);
	while ((e = list_next(unit_lib))) {
		if (e->cost <= 0)
			continue;
		if (nationid && (e->nation == -1 || 
				strcmp(nations[e->nation].id, nationid)))
			continue;
		if (uclassid && strcmp(unit_classes[e->class].id, uclassid))
			continue;
		if (date ) {
			if (e->start_year > date->year)
				continue;
			if (e->start_year == date->year && e->start_month > 
								date->month)
				continue;
			if (e->last_year && e->last_year < date->year)
				continue;
		}
		list_add(l,e);
	}
	return l;
}