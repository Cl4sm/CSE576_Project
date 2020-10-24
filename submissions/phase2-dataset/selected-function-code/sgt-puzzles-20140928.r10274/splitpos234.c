tree234 *splitpos234(tree234 *t, int index, int before) {
    tree234 *ret;
    node234 *n;
    int count;

    count = countnode234(t->root);
    if (index < 0 || index > count)
	return NULL;		       /* error */
    ret = newtree234(t->cmp);
    n = split234_internal(t, index);
    if (before) {
	/* We want to return the ones before the index. */
	ret->root = n;
    } else {
	/*
	 * We want to keep the ones before the index and return the
	 * ones after.
	 */
	ret->root = t->root;
	t->root = n;
    }
    return ret;
}