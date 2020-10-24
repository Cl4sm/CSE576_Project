struct sub_ent *SubList (c)
char *c;
/*
 *  Parse a newsrc subscription line, make into subscription list
 */
{
	struct sub_ent *sp, *sub_list;
	int lo, hi;
	char *range;

	/* Initialize subscription list */
	sub_list = NULL;

	/* Loop through line entries */
	range = strtok (c, ",");
	while (range != NULL) {
		/* Get space for new list entry */
		if (NULL == (sp = (struct sub_ent *)
			malloc (sizeof (struct sub_ent)))) OutOfMemory();

		/* Determine if it's a range or single entry */
		if (2 == sscanf (range, "%d-%d", &lo, &hi)) {
			sp->lo = lo;
			sp->hi = hi;

			/* Reverse them in case they're backwards */
			if (hi < lo) {
				sp->lo = hi;
				sp->hi = lo;
			}
		} else {
			/* Not a range */
			sp->lo = atoi (range);
			sp->hi = sp->lo;
		}

		/* Check if range overlaps last one */
		if ( (sub_list != NULL) && (sp->lo <= (sub_list->hi + 1))) {
			/* Combine ranges */
			if (sp->lo < sub_list->lo) sub_list->lo = sp->lo;
			if (sp->hi > sub_list->hi) sub_list->hi = sp->hi;

			/* Free old one */
			free (sp);
		} else {
			/* No overlap, maintain pointers */
			sp->next = sub_list;
			sub_list = sp;
		}
		range = strtok (NULL, ",");
	}

	return (sub_list);
}
