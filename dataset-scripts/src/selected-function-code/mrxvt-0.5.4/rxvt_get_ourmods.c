void
rxvt_get_ourmods( rxvt_t *r )
{
    int			i, j, k;
    int			requestedmeta, realmeta, realalt;
    const char*		cm;
    const char*		rsmod;
    XModifierKeymap*	map;
    KeyCode*		kc;
    const unsigned int	modmasks[] =
    {
	Mod1Mask, Mod2Mask, Mod3Mask, Mod4Mask, Mod5Mask
    };


    requestedmeta = realmeta = realalt = 0;

    rsmod = r->h->rs[Rs_modifier];
    if (rsmod &&
	STRCASECMP(rsmod, "mod1") >= 0 &&
	STRCASECMP(rsmod, "mod5") <= 0)
	requestedmeta = rsmod[3] - '0';

    map = XGetModifierMapping(r->Xdisplay);
    kc = map->modifiermap;
    for (i = 1; i < 6; i++)
    {
	k = (i + 2) * map->max_keypermod;   /* skip shift/lock/control */
	for (j = map->max_keypermod; j--; k++)
	{
	    if (kc[k] == 0)
		break;

	    switch (XKeycodeToKeysym(r->Xdisplay, kc[k], 0))
	    {
		case XK_Num_Lock:
		    r->h->ModNumLockMask = modmasks[i - 1];
		    /* FALLTHROUGH */
		default:
		    continue;	/* for(;;) */
		case XK_Meta_L:
		case XK_Meta_R:
		    cm = "meta";
		    realmeta = i;
		    break;
		case XK_Alt_L:
		case XK_Alt_R:
		    cm = "alt";
		    realalt = i;
		    break;
		case XK_Super_L:
		case XK_Super_R:
		    cm = "super";
		    break;
		case XK_Hyper_L:
		case XK_Hyper_R:
		    cm = "hyper";
		    break;
	    }

	    if (rsmod && STRNCASECMP(rsmod, cm, STRLEN(cm)) == 0)
		requestedmeta = i;
	}
    }
    XFreeModifiermap(map);

    i = (requestedmeta ? requestedmeta : (
	    realmeta ? realmeta : (
	    realalt ? realalt : 0)));

    if (i)
	r->h->ModMetaMask = modmasks[i - 1];
}