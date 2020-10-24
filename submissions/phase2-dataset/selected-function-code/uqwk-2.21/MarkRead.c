int num;
struct sub_ent *sp_head;
/*
 *  Mark article as read
 *
 *  Remember, the list is stored from highest numbers to lowest numbers.
 */
{
	struct sub_ent *sp, *tsp, *lsp;

	sp = sp_head;

	/* If num is much higher than highest list, or the list is
	   empty, we need new entry */
	if ( (sp == NULL) || (num > (sp->hi + 1))) {
		if (NULL == (tsp = (struct sub_ent *)
			malloc (sizeof (struct sub_ent)))) OutOfMemory();

		tsp->lo = tsp->hi = num;
		tsp->next = sp;

		return (tsp);
	}

	/* lsp remembers last entry in case we need to add a new entry */
	lsp = NULL;

	/* Find appropriate entry for this number */
	while (sp != NULL) {
		/* Have to squeeze one in before this one? */
		if (num > (sp->hi + 1)) {
			if (NULL == (tsp = (struct sub_ent *)
				malloc (sizeof (struct sub_ent))))
					OutOfMemory();

			tsp->lo = tsp->hi = num;
			tsp->next = sp;
			lsp->next = tsp;

			return (sp_head);
		}

		/* One greater than entry's hi? */
		if (num == (sp->hi + 1)) {
			sp->hi = num;
			return (sp_head);
		}

		/* In middle of range, do nothing */
		if ( (num >= sp->lo) && (num <= sp->hi) ) return (sp_head);

		/* One too lo, must check if we merge with next entry */
		if (num == (sp->lo - 1)) {
			if (NULL == sp->next) {
				/* No next entry to merge with */
				sp->lo = num;
				return (sp_head);
			}

			/* Check for merge */
			if (num == (sp->next->hi + 1)) {
				tsp = sp->next;
				sp->lo = tsp->lo;
				sp->next = tsp->next;
				free (tsp);
				return (sp_head);
			} else {
				/* No merge */
				sp->lo = num;
				return (sp_head);
			}
		}

		lsp = sp;
		sp = sp->next;
	}

	/* We flew off the end and need a new entry */
	if (NULL == (tsp = (struct sub_ent *)
		malloc (sizeof (struct sub_ent)))) OutOfMemory();

	tsp->lo = tsp->hi = num;
	tsp->next = NULL;
	lsp->next = tsp;

	return (sp_head);
}
