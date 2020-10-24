pvm_parent()
{
	int cc;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_PARENT,TEV_EVENT_ENTRY))
			TEV_FIN;
	}

	if (!(cc = BEATASK))
		cc = pvmmyptid ? pvmmyptid : PvmNoParent;

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_PARENT,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_PT, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0 && cc != PvmNoParent && cc != PvmParentNotSet )
		lpvmerr("pvm_parent", cc);
	return cc;
}
