pvm_mpp_beatask(mytid, myptid, 
				outtid, outctx, outtag,
				trctid, trcctx, trctag,
				udpmtu, schedtid, topvmd)
int *mytid, *myptid; 
int *outtid, *outctx, *outtag;
int *trctid, *trcctx, *trctag;
int *udpmtu, *schedtid;
struct ttpcb **topvmd;
{
	char errtxt[64];
	char *p;
	char *s;

	int ac = 0;
	int cc;
	int hostid = 0;
	int i;
	int myhost;
	int mynode, mysetpart;
	int partsize, partid;
	int pvminfo[SIZEHINFO];		/* proto, hostpart, ptid, MTU, NDF */

	msgmid_t rmid;				/* msg ID returned by imsgrecv() */

	info_t minfo[MPPINFOSIZE];	/* info that might be returned by msgdone */
	
	MSGFUNC_PTR mfunc;

	if (pvmmytid != -1)			/* already configured */
		return 0;

	mfunc = pvm_hostmsgfunc();

	pvm_mpp_message_init(&mynode, &partsize, &pvmhostnode, &pvmmyptype);

	myhost = pvmhostnode;
	
	if (mynode < 0 || partsize < 0 || myhost < 0)
	{
		sprintf(errtxt, "mppbeatask(): bad init, node %d, part %d, host %d\n",
				mynode, partsize, myhost);
		pvmlogerror(errtxt);
		return PvmSysErr;
	} 	

	if (pvmdebmask & PDMPACKET) 
	{
		sprintf(errtxt," %d Posting receive for config message \n", mynode);
		pvmlogerror(errtxt);
	}

	/* Post receive for the configuration message */
	if ((*mfunc->imsgrecv)(hostid, MPPANY, PMTCONF, (char *) pvminfo,
			 sizeof(pvminfo), MPPANY, (int *) NULL, &rmid) < 0) 
	{
		pvmlogperror("beatask() recv pvminfo");
		return PvmSysErr;
	}

	if (pvmdebmask & PDMPACKET) {
		sprintf(errtxt," %d Receive posted for config message \n", mynode);
		pvmlogerror(errtxt);
	}

	if ((pvm_useruid = getuid()) == -1) {
		pvmlogerror("can't getuid()\n");
		return PvmSysErr;
	}

	pvmmyupid = getpid();

	/*
	*	initialize received-message list 
	*/

	rxpmsgs = TALLOC(1, struct pmsg, "pmsgs");
	BZERO((char*)rxpmsgs, sizeof(struct pmsg));
	rxpmsgs->m_link = rxpmsgs->m_rlink = rxpmsgs;

	if (pvmdebmask & PDMPACKET) {
		sprintf(errtxt," %d Waiting  for config message \n", mynode);
		pvmlogerror(errtxt);
	}
	while (!((*mfunc->msgdone)(0, &rmid, minfo)));

	if (pvmdebmask & PDMPACKET) {
		sprintf(errtxt," %d Got config message \n", mynode);
		pvmlogerror(errtxt);
	}

	if (pvminfo[0] != TDPROTOCOL) {
		sprintf(errtxt, "beatask() t-d protocol mismatch (%d/%d)\n",
			TDPROTOCOL, pvminfo[0]);
		pvmlogerror(errtxt);
		return PvmSysErr;
	}

	/* We now have the configuration message -- set up various parameters
		based on the message */

	pvmmynode = mynode;
	mysetpart = pvminfo[1];
	*myptid = pvminfo[2];
	*udpmtu = pvminfo[3];
	pvmmyndf = pvminfo[4];
	pvmpartsize = pvminfo[5];
	*outtid = pvminfo[6];
	*outtag = pvminfo[7];
	*outctx = pvminfo[8];
	*trctid = pvminfo[9];
	*trctag = pvminfo[10];
	*trcctx = pvminfo[11];
	*mytid = mysetpart + pvmmynode;
	pvmfrgsiz = pvmudpmtu;

	/* ---- set up the pre-allocated receive buffers ---- */

	pvmddirect = new_directstruct( NSBUFS, NRBUFS );
	peerdirect = new_vdirectstruct( pvmpartsize, NSBUFS, NRBUFS);

	pvmdfrags = init_recv_list(NSBUFS, PMTDBASE, MAXFRAGSIZE, 0, MPPANY, 
				pvm_hostmsgfunc());
	nodefrags = init_recv_list(NSBUFS, PMTPBASE, MAXFRAGSIZE, 0, MPPANY,
				pvm_nodemsgfunc()); 

	/* intialize the packet numbering for packets from daemon */
	fill_directstruct (pvmddirect, NRBUFS, pvm_tidtohost(pvmmytid), 
		0, PMTDBASE, 0, MPPANY);
	init_chunkostruct( pvmddirect->ordering, NSBUFS);

	/* intialize the packet numbering for packets from peers */
	for (i = 0; i < pvmpartsize; i ++)
	{
		fill_directstruct (peerdirect + i, NRBUFS, i, 
			0, PMTPBASE, 0, MPPANY);
		init_chunkostruct( (peerdirect+i)->ordering, NSBUFS);
	}

	/* ---- Create Task, pvmd  PCBs so that they can be found easily ----- */

	peerpcbs = TALLOC(pvmpartsize + 1, struct ttpcb, "pcbs");
	for (i = 0; i <= pvmpartsize; i ++)
	{
		BZERO((char *)(peerpcbs + i), sizeof(struct ttpcb));
		(peerpcbs + i) -> tt_tid = i;
		(peerpcbs + i) -> tt_state = TTOPEN;
		(peerpcbs + i) -> mpdirect = peerdirect + i;
	} 

	pvmdpcb = peerpcbs + pvmpartsize;	
	pvmdpcb -> mpdirect = pvmddirect;

	*topvmd = pvmdpcb;
	
	precvIds = msgid_new();
	precvIds->ms_link = precvIds->ms_rlink = precvIds;

	if (s = getenv("PVMINPLACEDELAY"))
	{
		inplaceDelay = atoi(s);
		pvmlogprintf("setting in place delay to %d \n", inplaceDelay);
	}
	else
#if !defined(IMA_PGON)
		inplaceDelay = 0;
#else
		inplaceDelay = 250;				/* uSec. Seems to work well */
#endif

	pvm_setopt(PvmRoute, PvmDontRoute); /* Deny direct routing */
	return 0;


	
}
