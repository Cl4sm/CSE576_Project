void
pvmdisplaymhfinfo(who, msg, tid)
	char *who;			/* who made this call */
	char *msg;			/* message from caller */
	int tid;			/* tid of caller */
{	char *me = "pvmdisplaymhfinfo";
	int i;
	int head;

	printf("\n%s t%x: Display Message Handler Information: ",
		who, tid );
	printf("ndhandles = %d\tnhandles = %d\tfl_dhandles = %d", 
		ndhandles, nhandles, fl_dhandles);

	printf("\n%s: free list.  head <-", me);
	head = fl_dhandles;
	while (head != -1){
		printf(" %d", head);
		head = dhandles[head].mhid;
	}
	printf(" -< tail");

	printf("\n%s t%x: %s\n", who, tid, msg);
	for (i = 0; i < ndhandles; i++ ) {
		printf("%s t%x: dhandles[%2d].mhid = %2d  .handle = %2d     ",
			who, tid, i, dhandles[i].mhid, dhandles[i].handle);
		printf("handles[%2d].mhid = %2d  .header.src = t%-8x  ",
			i, handles[i].mhid, handles[i].header.src);
		printf(".header.ctx = %8d  .header.tag= %8d\n",
			handles[i].header.ctx, handles[i].header.tag);
	}
	printf("\n\n"); fflush(stdout);
} /* pvmdisplaymhfinfo */
