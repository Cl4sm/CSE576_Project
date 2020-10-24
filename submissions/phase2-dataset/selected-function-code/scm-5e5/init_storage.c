     STACKITEM *stack_start_ptr;
     long init_heap_size;
{
	sizet j;
	/* Because not all protects may get initialized */
	freelist = EOL;
	expmem = 0;
	estk_pool = EOL;
	scm_estk = BOOL_F;
	scm_port_table = 0;
	scm_port_table_len = 0;

#ifdef SHORT_SIZET
	if (sizeof(sizet) >= sizeof(long))
	  fixconfig(remsg, "SHORT_SIZET", 0);
#else
	if (sizeof(sizet) < sizeof(long))
	  fixconfig(addmsg, "SHORT_SIZET", 0);
#endif
#ifdef SHORT_INT
	if (sizeof(int) >= sizeof(long))
	  fixconfig(remsg, "SHORT_INT", 0);
#else
	if (sizeof(int) < sizeof(long))
	  fixconfig(addmsg, "SHORT_INT", 0);
#endif
#ifdef CDR_DOUBLES
	if (sizeof(double) != sizeof(long))
	  fixconfig(remsg, "CDR_DOUBLES", 0);
#else
# ifdef SINGLES
	if (sizeof(float) != sizeof(long)) {
	  if (sizeof(double) == sizeof(long))
	    fixconfig(addmsg, "CDR_DOUBLES", 0);
	  else
	    fixconfig(remsg, "SINGLES", 0);
	}
# endif
#endif
#ifdef BIGDIG
	if (2*BITSPERDIG/CHAR_BIT > sizeof(long))
	  fixconfig(remsg, "BIGDIG", 0);
# ifndef DIGSTOOBIG
	if (DIGSPERLONG*sizeof(BIGDIG) > sizeof(long))
	  fixconfig(addmsg, "DIGSTOOBIG", 0);
# endif
	if (NUMDIGS_MAX > (((unsigned long)-1L)>>16))
	  fixconfig(rdmsg, "NUMDIGS_MAX", 0);
#endif
#ifdef STACK_GROWS_UP
	if (((STACKITEM *)&j - stack_start_ptr) < 0)
	  fixconfig(remsg, "STACK_GROWS_UP", 1);
#else
	if ((stack_start_ptr - (STACKITEM *)&j) < 0)
	  fixconfig(addmsg, "STACK_GROWS_UP", 1);
#endif
	j = HEAP_SEG_SIZE;
	if (HEAP_SEG_SIZE != j)
	  fixconfig(rdmsg, "size of HEAP_SEG_SIZE", 0);

	mtrigger = INIT_MALLOC_LIMIT;
	mltrigger = mtrigger - MIN_MALLOC_YIELD;
	hplims = (CELLPTR *) must_malloc(2L*sizeof(CELLPTR), s_hplims);
	if (0L==init_heap_size) init_heap_size = INIT_HEAP_SIZE;
	j = init_heap_size;
/*  	printf("j = %u; init_heap_size = %lu\n", j, init_heap_size); */
	if ((init_heap_size != j) || !init_heap_seg((CELLPTR) malloc(j), j)) {
	  j = HEAP_SEG_SIZE;
/*  	  printf("j = %u; HEAP_SEG_SIZE = %lu\n", j, HEAP_SEG_SIZE); */
	  if (!init_heap_seg((CELLPTR) malloc(j), j))
	    wta(MAKINUM(j), (char *)NALLOC, s_heap);
	}
	else expmem = 1;
	heap_org = CELL_UP(hplims[0]);
		/* hplims[0] can change. do not remove heap_org */

	scm_port_table_len = 16;
	scm_port_table = (port_info *)
	  must_malloc((long)scm_port_table_len * sizeof(port_info), s_port_table);
	for (j = 0; j < scm_port_table_len; j++) {
	  scm_port_table[j].flags = 0L;
	  scm_port_table[j].data = UNDEFINED;
	  scm_port_table[j].port = UNDEFINED;
	}

	nullstr = must_malloc_cell(1L, MAKE_LENGTH(0, tc7_string), s_string);
	CHARS(nullstr)[0] = 0;
	nullvect = must_malloc_cell(1L, MAKE_LENGTH(0, tc7_vector), s_vector);
	{
	  long i = symhash_dim;
	  SCM *velts;
	  symhash = must_malloc_cell(i * sizeof(SCM),
				     MAKE_LENGTH(i, tc7_vector),
				     s_vector);
	  velts = VELTS(symhash);
	  while(--i >= 0) (velts)[i] = EOL;
	}
	/* Now that symhash is setup, we can sysintern() */
	sysintern("most-positive-fixnum", (SCM)MAKINUM(MOST_POSITIVE_FIXNUM));
	sysintern("most-negative-fixnum", (SCM)MAKINUM(MOST_NEGATIVE_FIXNUM));
#ifdef BIGDIG
	sysintern("bignum-radix", MAKINUM(BIGRAD));
#endif
	def_inp = scm_port_entry(stdin, tc16_fport, OPN|RDNG);
	SCM_PORTDATA(def_inp) = CAR(sysintern("stdin", UNDEFINED));
	def_outp = scm_port_entry(stdout, tc16_fport, OPN|WRTNG|TRACKED);
	SCM_PORTDATA(def_outp) = CAR(sysintern("stdout", UNDEFINED));
	NEWCELL(def_errp);
	CAR(def_errp) = (tc16_fport|OPN|WRTNG);
	SETSTREAM(def_errp, stderr);
	cur_inp = def_inp;
	cur_outp = def_outp;
	cur_errp = def_errp;
	NEWCELL(sys_errp);
	CAR(sys_errp) = (tc16_sysport|OPN|WRTNG);
	SETSTREAM(sys_errp, 0);
	sys_safep = mksafeport(0, def_errp);
	dynwinds = EOL;
	NEWCELL(rootcont);
	SETCONT(rootcont, make_root_continuation(stack_start_ptr));
	CAR(rootcont) = tc7_contin;
	CONT(rootcont)->other.dynenv = EOL;
	CONT(rootcont)->other.parent = BOOL_F;
	listofnull = cons(EOL, EOL);
	undefineds = cons(UNDEFINED, EOL);
	CDR(undefineds) = undefineds;
	/* flo0 is now setup in scl.c */
	/* Set up environment cache */
	scm_ecache_len = sizeof(ecache_v)/sizeof(cell);
	scm_ecache = CELL_UP(ecache_v);
	scm_ecache_len = CELL_DN(ecache_v + scm_ecache_len - 1) - scm_ecache + 1;
	scm_ecache_index = scm_ecache_len;
	scm_egc_root_index = sizeof(scm_egc_roots)/sizeof(SCM);
	scm_estk = BOOL_F;
	scm_estk_reset(0);
}
