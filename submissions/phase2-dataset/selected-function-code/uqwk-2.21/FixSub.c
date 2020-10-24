struct sub_ent *FixSub (sp, lo, hi)
struct sub_ent *sp;
int lo, hi;
/*
 *  Sanity fixes to the subscription list
 */
{
	struct sub_ent *tsp1, *tsp2, *tsp3;

	/* If the list is empty, make one new entry marking everything
	   up to the lowest available article as read */
	if (sp == NULL) {
		if (NULL == (tsp1 = (struct sub_ent *) malloc
			(sizeof (struct sub_ent)))) OutOfMemory();

		tsp1->lo = 1;
		tsp1->hi = (lo > 1) ? (lo-1) : 1;
		tsp1->next = NULL;
		return (tsp1);
	}

	/* If the highest read article is greater than the highest
	   available article, assume the group has been reset */
	if (sp->hi > hi) {
		/* Mark everything as unread */
		sp->lo = 1;
		sp->hi = (lo > 1) ? (lo-1) : 1;

		/* Free the rest */
		tsp1 = sp->next;
		while (tsp1 != NULL) {
			tsp2 = tsp1->next;
			free (tsp1);
			tsp1 = tsp2;
		}
		sp->next = NULL;
		return (sp);
	}

	/* Now walk through the list and eliminate ranges lower
	   than the lowest available article */
	tsp1 = sp;
	while (tsp1 != NULL) {
		/* If lowest read article of this range is smaller
		   than the lowest available article, all the rest
		   of the ranges are unnecessary */

		if (tsp1->lo < lo) {
			/* Make this range from 1 to lo */
			tsp1->lo = 1;
			if (tsp1->hi < lo) tsp1->hi = lo - 1;

			/* Free the rest */
			tsp2 = tsp1->next;
			while (tsp2 != NULL) {
				tsp3 = tsp2->next;
				free (tsp2);
				tsp2 = tsp3;
			}
			tsp1->next = NULL;
			return (sp);
		}
		tsp1 = tsp1->next;
	}
	return (sp);	/* Probably shouldn't get here */
}
