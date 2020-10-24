int
node_send(txup, txfp, dtid, code)
	struct pmsg *txup;		/* tx message or null */
	struct frag *txfp;		/* cur tx frag or null */
	int dtid;				/* dest */
	int code;				/* type code */
{
	int mask = pvmtidhmask;		/* host */
	char *txcp = 0;				/* point to remainder of txfp */
	int txtogo = 0;				/* len of txfp */
	long node;					/* destination node */
	long ptype;					/* destination process type */
	int i;
	int ff;
	char dummy[TDFRAGHDR+TTMSGHDR];	/* for inplace data */
#if defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) || defined(IMA_AIX5SP2)
	MPI_Status mpista;
	int mpiflag = 0;
#endif

#if defined(IMA_PGON) || defined(IMA_CM5) || defined(IMA_SP2MPI) \
		|| defined(IMA_AIX4SP2) || defined(IMA_AIX5SP2)
	mask |= pvmtidtmask;				/* process type */
#endif

	if (TIDISNODE(dtid) && (dtid & mask) == (pvmmytid & mask)) {
		node = dtid & pvmtidnmask;
		ptype = pvmmyptype;			/* send to node directly */
	} else {
		node = pvmhostnode;
		ptype = PVMDPTYPE;			/* send to pvmd first */
	}

	do {

	/* check any pending sends; free data if send is completed */

		if (nextsmhd == NUMSMHD)
			nextsmhd = 0;
		i = nextsmhd;

		while (outfrags[i].tf_mhdl != PVMMHDNULL &&
		ASYNCDONE(outfrags[i].tf_mhdl) > 0) {
			fr_unref(outfrags[i].tf_fp);
#ifdef IMA_CM5
			CMMD_free_mcb(outfrags[i].tf_mhdl);
#endif
			outfrags[i++].tf_mhdl = PVMMHDNULL;
		}

		if (outfrags[nextsmhd].tf_mhdl != PVMMHDNULL) {
			if (pvmdebmask & PDMPACKET)
				pvmlogerror("out of mid's?");
			nextsmhd++;		/* don't get stuck here; check out next mhd */
			continue;
		}

		if (txfp->fr_u.dab) 		/* packed data */
			txcp = txfp->fr_dat;
		else 						/* inplace data */
			txcp = dummy + sizeof(dummy);
		txtogo = txfp->fr_len;

	/*
	* if this is first frag, prepend t-t header
	*/
		ff = 0;
		if (txfp->fr_rlink == txup->m_frag) {
			txcp -= TTMSGHDR;
			txtogo += TTMSGHDR;
			pvmput32(txcp, code);
			pvmput32(txcp + 4, (txup->m_enc == 2 ? pvmmydsig : txup->m_enc));
			pvmput32(txcp + 8, txup->m_wid);
#ifdef	MCHECKSUM
			pvmput32(txcp + 12, umbuf_crc(txup));
#else
			pvmput32(txcp + 12, 0);
#endif
			ff = FFSOM;
		}
		if (txfp->fr_link == txup->m_frag)
			ff |= FFEOM;
	/*
	* prepend t-d header
	*/
		txcp -= TDFRAGHDR;
		pvmput32(txcp, dtid);
		pvmput32(txcp + 4, pvmmytid);
		pvmput32(txcp + 8, txtogo);
		pvmput32(txcp + 12, 0);			/* to keep putrify happy */
		pvmput8(txcp + 12, ff);
		txtogo += TDFRAGHDR;
		if (pvmdebmask & PDMPACKET) {
			sprintf(pvmtxt, "node_send() dst t%x len %d ptype=%ld node=%ld\n",
				dtid, txfp->fr_len, ptype, node);
			pvmlogerror(pvmtxt);
		}

		if (!txfp->fr_u.dab) {           /* inplace data */
			if ((mpierrcode = PVMCSEND(PMTPACK, txcp, txtogo- txfp->fr_len, 
			node, ptype))
#if defined(IMA_PGON) || defined(IMA_CM5) || defined(IMA_I860)
			&& (mpierrcode < 0)
#endif
			) {
				pvmlogperror("node_send() csend header");
				return PvmSysErr;
			}
#ifdef IMA_I860
			/* crecv can't select msg with node #; it can only select type */
			if (_csend(pvmmyvnode, txfp->fr_dat, txfp->fr_len, node, 0) < 0) {
				pvmlogperror("node_send() csend inplace data");
				return PvmSysErr;
			}
#else
			/* here the type field is reserved for psend/precv */
			if ((mpierrcode = 
			PVMCSEND(PMTPACK, txfp->fr_dat, txfp->fr_len, node, ptype))
#if defined(IMA_PGON) || defined(IMA_CM5) || defined(IMA_I860)
			&& (mpierrcode < 0)
#endif
			) {
				pvmlogperror("node_send() csend inplace data");
				return PvmSysErr;
			}
#endif
		} else {

			if (node != pvmhostnode) {
#if defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) || defined(IMA_AIX5SP2)
				if (mpierrcode = MPI_Isend(txcp, txtogo, MPI_BYTE, node, 
				PMTPACK, MPI_COMM_WORLD, &outfrags[nextsmhd].tf_mhdl)) {
#else
				if ((outfrags[nextsmhd].tf_mhdl =
				ASYNCSEND(PMTPACK, txcp, txtogo, node, ptype)) < 0) {
#endif
					pvmlogperror("node_send() asyncsend");
					return PvmSysErr;
				}
				/* don't free data 'til frag's sent */
				txfp->fr_u.ref++;		
				outfrags[nextsmhd++].tf_fp = txfp;
			} else {
				/* 
			 	 * In multicast, the fragment header may get overwritten
			 	 * if we use ASYNCSEND. This is OK for node-node send,
			 	 * because the dst field is not used. But pvmd relies on
			 	 * this value to deliever the packet.
			 	 */
				if ((mpierrcode = PVMCSEND(PMTPACK, txcp, txtogo, node, ptype)) 
#if defined(IMA_PGON) || defined(IMA_CM5) || defined(IMA_I860)
				&& (mpierrcode < 0)
#endif
				) {
					pvmlogperror("node_send() csend");
					return PvmSysErr;
				}
			}
		}

		txfp = txfp->fr_link;
		if (!txfp->fr_buf)
			txfp = 0;

	} while (txfp);
}
