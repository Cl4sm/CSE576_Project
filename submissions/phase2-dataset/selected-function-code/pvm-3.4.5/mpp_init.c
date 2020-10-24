mpp_init(argc, argv)
	int *argc;
	char **argv;
{
	int i;
	int cc;

#if defined(IMA_PGON) 
	if ((partsize = nx_initve((char *)0, 0, "", argc, argv)) < 0) 
	{
		pvmlogperror("mpp_init() nx_initve\n");
		pvmbailout();
	}
	sprintf(etext, "using %d nodes\n", partsize);
	pvmlogerror(etext);
#endif

#if defined(IMA_PGONPUMA)
   	if ( (cc = host_lib_init()) < 0)
	{
		pvmlogperror("mpp_init(): host_lib_int\n"); 	
		pvmbailout();
	}
	for (i=0; i < (sizeof(phystolmap)/sizeof(short)); i++)
		phystolmap[i] = -1;
#endif

	busynodes = TALLOC(1, struct nodeset, "nsets");
	BZERO((char*)busynodes, sizeof(struct nodeset));
	busynodes->n_link = busynodes;
	busynodes->n_rlink = busynodes;

	ptypemask = tidtmask >> (ffs(tidtmask) - 1);

	/* need a list of packets that have been sent */
	mppopq = TALLOC(1, struct pkt , "mppopq");
	mppopq->pk_link = mppopq;
	mppopq->pk_rlink = mppopq;;	

	pvm_init_asynch_list( mppsendmids, (CHUNK_PTR *) mppoutpkts, NMPPSBUFMIDS);
}
