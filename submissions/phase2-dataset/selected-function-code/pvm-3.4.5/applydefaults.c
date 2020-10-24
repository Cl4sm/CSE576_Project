applydefaults(hp, defhp)
	struct hostd *hp;		/* hostd to modify */
	struct hostd *defhp;	/* default settings */
{
	if (!hp->hd_login && defhp->hd_login)
		hp->hd_login = STRALLOC(defhp->hd_login);

	if (!hp->hd_dpath && defhp->hd_dpath)
		hp->hd_dpath = STRALLOC(defhp->hd_dpath);

	if (!hp->hd_epath && defhp->hd_epath)
		hp->hd_epath = STRALLOC(defhp->hd_epath);

	if (!hp->hd_bpath && defhp->hd_bpath)
		hp->hd_bpath = STRALLOC(defhp->hd_bpath);

	if (!hp->hd_wdir && defhp->hd_wdir)
		hp->hd_wdir = STRALLOC(defhp->hd_wdir);

	if (!hp->hd_sopts && defhp->hd_sopts)
		hp->hd_sopts = STRALLOC(defhp->hd_sopts);

	if (!hp->hd_aname && defhp->hd_aname)
		hp->hd_aname = STRALLOC(defhp->hd_aname);

	if (!(hp->hd_flag & HF_SPEED) && (defhp->hd_flag & HF_SPEED))
		hp->hd_speed = defhp->hd_speed;

	if (!hp->hd_vmid && defhp->hd_vmid)
		hp->hd_vmid = STRALLOC(defhp->hd_vmid);

	hp->hd_flag |= defhp->hd_flag;

	return 0;
}
