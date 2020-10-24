free_multipath_attributes (struct multipath * mpp)
{
	if (!mpp)
		return;

	if (mpp->selector &&
	    mpp->selector != conf->selector &&
	    (!mpp->mpe || (mpp->mpe && mpp->selector != mpp->mpe->selector)) &&
	    (!mpp->hwe || (mpp->hwe && mpp->selector != mpp->hwe->selector))) {
		FREE(mpp->selector);
		mpp->selector = NULL;
	}

	if (mpp->features) {
		FREE(mpp->features);
		mpp->features = NULL;
	}

	if (mpp->hwhandler &&
	    mpp->hwhandler != conf->hwhandler &&
	    (!mpp->hwe || (mpp->hwe && mpp->hwhandler != mpp->hwe->hwhandler))) {
		FREE(mpp->hwhandler);
		mpp->hwhandler = NULL;
	}
}
