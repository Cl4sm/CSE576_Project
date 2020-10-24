static void
f_rta_cow(void)
{
  if ((*f_rte)->attrs->aflags & RTAF_CACHED) {

    /* Prepare to modify rte */
    f_rte_cow();

    /* Store old rta to free it later */
    f_old_rta = (*f_rte)->attrs;

    /* 
     * Alloc new rta, do shallow copy and update rte. Fields eattrs
     * and nexthops of rta are shared with f_old_rta (they will be
     * copied when the cached rta will be obtained at the end of
     * f_run()), also the lock of hostentry is inherited (we suppose
     * hostentry is not changed by filters).
     */
    rta *ra = lp_alloc(f_pool, sizeof(rta));
    memcpy(ra, f_old_rta, sizeof(rta));
    ra->aflags = 0;
    (*f_rte)->attrs = ra;
  }
}
