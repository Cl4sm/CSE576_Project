int
mesg_input(up)
	struct pmsg *up;
{
	int savesbuf;
	int saverbuf;
	int savectx;
	int i;
	TEV_DECLS

	if (pvmdebmask & PDMMESSAGE)
		pvmlogprintf("mesg_input() src t%x ctx %d tag %s len %d\n",
				up->m_src, up->m_ctx,
				pvmnametag(up->m_tag, (int *)0), up->m_len);
	for (i = nhandles; i-- > 0; ) {
		if ( (handles[i].header.tag == -1
				|| handles[i].header.tag == up->m_tag)
			&& (handles[i].header.ctx == -1
				|| handles[i].header.ctx == up->m_ctx)
			&& (handles[i].header.src == -1
				|| handles[i].header.src == up->m_src) )
		{
			if (TEV_DO_TRACE(TEV_MHF_INVOKE,TEV_EVENT_ENTRY)) {

				TEV_PACK_INT( TEV_DID_RST, TEV_DATA_SCALAR,
						&(handles[i].header.src), 1, 1 );
				TEV_PACK_INT( TEV_DID_RMC, TEV_DATA_SCALAR,
						&(handles[i].header.tag), 1, 1 );
				TEV_PACK_INT( TEV_DID_RCX, TEV_DATA_SCALAR,
						&(handles[i].header.ctx), 1, 1 );

				TEV_PACK_INT( TEV_DID_MB, TEV_DATA_SCALAR,
						&(up->m_mid), 1, 1 );
				TEV_PACK_INT( TEV_DID_MNB, TEV_DATA_SCALAR,
						&(up->m_len), 1, 1 );
				TEV_PACK_INT( TEV_DID_MC, TEV_DATA_SCALAR,
						&(up->m_tag), 1, 1 );
				TEV_PACK_INT( TEV_DID_MCX, TEV_DATA_SCALAR,
						&(up->m_ctx), 1, 1 );
				TEV_PACK_INT( TEV_DID_SRC, TEV_DATA_SCALAR,
						&(up->m_src), 1, 1 );

				TEV_FIN;

				TEV_PUSHTOP;
			}

			savesbuf = pvm_setsbuf( 0 );
			saverbuf = pvm_setrbuf( up->m_mid );
			savectx = pvm_setcontext( up->m_ctx );

			(handles[i].f)(up->m_mid);

			pvm_setcontext( savectx );
			pvm_freebuf( pvm_setsbuf( savesbuf ) );
			pvm_freebuf( pvm_setrbuf( saverbuf ) );

			if ( TEV_AMPUSHED )
				TEV_POPTOP;

			return 0;
		}
	}
	LISTPUTBEFORE(pvmrxlist, up, m_link, m_rlink);
	return 0;
}
