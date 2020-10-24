static void merge_vertices(int *connected,
			   struct solver_scratch *sc, int i, int j)
{
    int exits = -1, border = FALSE;    /* initialise to placate optimiser */

    if (sc) {
	i = dsf_canonify(connected, i);
	j = dsf_canonify(connected, j);

	/*
	 * We have used one possible exit from each of the two
	 * classes. Thus, the viable exit count of the new class is
	 * the sum of the old exit counts minus two.
	 */
	exits = sc->exits[i] + sc->exits[j] - 2;

	border = sc->border[i] || sc->border[j];
    }

    dsf_merge(connected, i, j);

    if (sc) {
	i = dsf_canonify(connected, i);
	sc->exits[i] = exits;
	sc->border[i] = border;
    }
}