{
	struct multipath * mpp;
	int i,j, ret, isFound;
	struct path * pp = (struct path *)pathp;
	unsigned char *keyp;
	uint64_t prkey;
	struct prout_param_descriptor *param;
	struct prin_resp *resp;

	mpp = pp->mpp;

	resp = mpath_alloc_prin_response(MPATH_PRIN_RKEY_SA);
	if (!resp){
		condlog(0,"%s Alloc failed for prin response", pp->dev);
		return NULL;
	}

	ret = prin_do_scsi_ioctl(pp->dev, MPATH_PRIN_RKEY_SA, resp, 0);
	if (ret != MPATH_PR_SUCCESS )
	{
		condlog(0,"%s : pr in read keys service action failed. Error=%d", pp->dev, ret);
		goto out;
	}

	condlog(3, " event pr=%d addlen=%d",resp->prin_descriptor.prin_readkeys.prgeneration,
			resp->prin_descriptor.prin_readkeys.additional_length );

	if (resp->prin_descriptor.prin_readkeys.additional_length == 0 )
	{
		condlog(1, "%s: No key found. Device may not be registered.", pp->dev);
		ret = MPATH_PR_SUCCESS;
		goto out;
	}
	prkey = 0;
	keyp = (unsigned char *)mpp->reservation_key;
	for (j = 0; j < 8; ++j) {
		if (j > 0)
			prkey <<= 8;
		prkey |= *keyp;
		++keyp;
	}
	condlog(2, "Multipath  reservation_key: 0x%" PRIx64 " ", prkey);

	isFound =0;
	for (i = 0; i < resp->prin_descriptor.prin_readkeys.additional_length/8; i++ )
	{
		condlog(2, "PR IN READKEYS[%d]  reservation key:",i);
		dumpHex((char *)&resp->prin_descriptor.prin_readkeys.key_list[i*8], 8 , -1);
		if (!memcmp(mpp->reservation_key, &resp->prin_descriptor.prin_readkeys.key_list[i*8], 8))
		{
			condlog(2, "%s: pr key found in prin readkeys response", mpp->alias);
			isFound =1;
			break;
		}
	}
	if (!isFound)
	{
		condlog(0, "%s: Either device not registered or ", pp->dev);
		condlog(0, "host is not authorised for registration. Skip path");
		ret = MPATH_PR_OTHER;
		goto out;
	}

	param= malloc(sizeof(struct prout_param_descriptor));
	memset(param, 0 , sizeof(struct prout_param_descriptor));

	for (j = 7; j >= 0; --j) {
		param->sa_key[j] = (prkey & 0xff);
		prkey >>= 8;
	}
	param->num_transportid = 0;

	condlog(3, "device %s:%s", pp->dev, pp->mpp->wwid);

	ret = prout_do_scsi_ioctl(pp->dev, MPATH_PROUT_REG_IGN_SA, 0, 0, param, 0);
	if (ret != MPATH_PR_SUCCESS )
	{
		condlog(0,"%s: Reservation registration failed. Error: %d", pp->dev, ret);
	}
	mpp->prflag = 1;

	free(param);
out:
	free(resp);
	return NULL;
}
