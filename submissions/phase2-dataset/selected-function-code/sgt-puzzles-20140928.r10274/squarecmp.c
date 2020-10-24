static int squarecmp(const void *av, const void *bv)
{
    const struct square *a = (const struct square *)av;
    const struct square *b = (const struct square *)bv;
    if (a->type < b->type)
	return -1;
    else if (a->type > b->type)
	return +1;
    else if (a->random < b->random)
	return -1;
    else if (a->random > b->random)
	return +1;
    else if (a->y < b->y)
	return -1;
    else if (a->y > b->y)
	return +1;
    else if (a->x < b->x)
	return -1;
    else if (a->x > b->x)
	return +1;
    return 0;
}