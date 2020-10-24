int edsf_canonify(int *dsf, int index, int *inverse_return)
{
    int start_index = index, canonical_index;
    int inverse = 0;

/*    fprintf(stderr, "dsf = %p\n", dsf); */
/*    fprintf(stderr, "Canonify %2d\n", index); */

    assert(index >= 0);

    /* Find the index of the canonical element of the 'equivalence class' of
     * which start_index is a member, and figure out whether start_index is the
     * same as or inverse to that. */
    while ((dsf[index] & 2) == 0) {
        inverse ^= (dsf[index] & 1);
	index = dsf[index] >> 2;
/*        fprintf(stderr, "index = %2d, ", index); */
/*        fprintf(stderr, "inverse = %d\n", inverse); */
    }
    canonical_index = index;
    
    if (inverse_return)
        *inverse_return = inverse;
    
    /* Update every member of this 'equivalence class' to point directly at the
     * canonical member. */
    index = start_index;
    while (index != canonical_index) {
	int nextindex = dsf[index] >> 2;
        int nextinverse = inverse ^ (dsf[index] & 1);
	dsf[index] = (canonical_index << 2) | inverse;
        inverse = nextinverse;
	index = nextindex;
    }

    assert(inverse == 0);

/*    fprintf(stderr, "Return %2d\n", index); */
    
    return index;
}