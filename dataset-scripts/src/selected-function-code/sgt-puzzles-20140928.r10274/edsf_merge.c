void edsf_merge(int *dsf, int v1, int v2, int inverse)
{
    int i1, i2;

/*    fprintf(stderr, "dsf = %p\n", dsf); */
/*    fprintf(stderr, "Merge [%2d,%2d], %d\n", v1, v2, inverse); */
    
    v1 = edsf_canonify(dsf, v1, &i1);
    assert(dsf[v1] & 2);
    inverse ^= i1;
    v2 = edsf_canonify(dsf, v2, &i2);
    assert(dsf[v2] & 2);
    inverse ^= i2;

/*    fprintf(stderr, "Doing [%2d,%2d], %d\n", v1, v2, inverse); */

    if (v1 == v2)
        assert(!inverse);
    else {
	assert(inverse == 0 || inverse == 1);
	/*
	 * We always make the smaller of v1 and v2 the new canonical
	 * element. This ensures that the canonical element of any
	 * class in this structure is always the first element in
	 * it. 'Keen' depends critically on this property.
	 *
	 * (Jonas Koelker previously had this code choosing which
	 * way round to connect the trees by examining the sizes of
	 * the classes being merged, so that the root of the
	 * larger-sized class became the new root. This gives better
	 * asymptotic performance, but I've changed it to do it this
	 * way because I like having a deterministic canonical
	 * element.)
	 */
	if (v1 > v2) {
	    int v3 = v1;
	    v1 = v2;
	    v2 = v3;
	}
	dsf[v1] += (dsf[v2] >> 2) << 2;
	dsf[v2] = (v1 << 2) | !!inverse;
    }
    
    v2 = edsf_canonify(dsf, v2, &i2);
    assert(v2 == v1);
    assert(i2 == inverse);

/*    fprintf(stderr, "dsf[%2d] = %2d\n", v2, dsf[v2]); */
}