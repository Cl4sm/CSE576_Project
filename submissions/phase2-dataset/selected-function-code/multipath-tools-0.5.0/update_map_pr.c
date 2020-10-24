{
	int noisy=0;
	struct prin_resp *resp;
	int i,j, ret, isFound;
	unsigned char *keyp;
	uint64_t prkey;

	if (!mpp->reservation_key)
	{
		/* Nothing to do. Assuming pr mgmt feature is disabled*/
		condlog(3, "%s: reservation_key not set in multiapth.conf", mpp->alias);
		return MPATH_PR_SUCCESS;
	}

	resp = mpath_alloc_prin_response(MPATH_PRIN_RKEY_SA);
	if (!resp)
	{
		condlog(0,"%s : failed to alloc resp in update_map_pr", mpp->alias);
		return MPATH_PR_OTHER;
	}
	ret = mpath_prin_activepath(mpp, MPATH_PRIN_RKEY_SA, resp, noisy);

	if (ret != MPATH_PR_SUCCESS )
	{
		condlog(0,"%s : pr in read keys service action failed Error=%d", mpp->alias, ret);
		free(resp);
		return  ret;
	}

	if (resp->prin_descriptor.prin_readkeys.additional_length == 0 )
	{
		condlog(0,"%s: No key found. Device may not be registered. ", mpp->alias);
		free(resp);
		return MPATH_PR_SUCCESS;
	}

	prkey = 0;
	keyp = mpp->reservation_key;
	for (j = 0; j < 8; ++j) {
		if (j > 0)
			prkey <<= 8;
		prkey |= *keyp;
		++keyp;
	}
	condlog(2, "%s: Multipath  reservation_key: 0x%" PRIx64 " ", mpp->alias, prkey);

	isFound =0;
	for (i = 0; i < resp->prin_descriptor.prin_readkeys.additional_length/8; i++ )
	{
		condlog(2, "%s: PR IN READKEYS[%d]  reservation key:", mpp->alias, i);
		dumpHex((char *)&resp->prin_descriptor.prin_readkeys.key_list[i*8], 8 , 1);

		if (!memcmp(mpp->reservation_key, &resp->prin_descriptor.prin_readkeys.key_list[i*8], 8))
		{
			condlog(2, "%s: reservation key found in pr in readkeys response", mpp->alias);
			isFound =1;
		}
	}

	if (isFound)
	{
		mpp->prflag = 1;
		condlog(2, "%s: prflag flag set.", mpp->alias );
	}

	free(resp);
	return MPATH_PR_SUCCESS;
}
