static int movecmp(void *av, void *bv)
{
    struct move *a = (struct move *)av;
    struct move *b = (struct move *)bv;

    if (a->y < b->y)
	return -1;
    else if (a->y > b->y)
	return +1;

    if (a->x < b->x)
	return -1;
    else if (a->x > b->x)
	return +1;

    if (a->dy < b->dy)
	return -1;
    else if (a->dy > b->dy)
	return +1;

    if (a->dx < b->dx)
	return -1;
    else if (a->dx > b->dx)
	return +1;

    return 0;
}