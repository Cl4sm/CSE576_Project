{
  SCM grps, ans;
  int ngroups = getgroups(0, 0);
  if (!ngroups) return BOOL_F;
  scm_protect_temp(&grps);
  DEFER_INTS;
  /* grps is used as a gc protect, its type used to be tc7_string, but
     strings are now checked for null termination during gc.
     The length needs not be exactly right */
  grps = must_malloc_cell((0L + ngroups) * sizeof(gid_t),
			  MAKE_LENGTH(((0L + ngroups) * sizeof(gid_t))/sizeof(long),
				      tc7_VfixN32),
			  scm_s_getgroups);
  ALLOW_INTS;
  {
    gid_t *groups = (gid_t *)CHARS(grps);
    int val = getgroups(ngroups, groups);
    if (val < 0) return BOOL_F;
    ans = make_vector(MAKINUM(ngroups), UNDEFINED);
    while (--ngroups >= 0) VELTS(ans)[ngroups] = MAKINUM(groups[ngroups]);
    return ans;
  }
}
