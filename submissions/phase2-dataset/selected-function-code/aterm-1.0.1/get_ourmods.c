void
get_ourmods(void)
{
    int             i, j, k;
    int             got_meta, got_numlock;
    XModifierKeymap *map;
    KeyCode        *kc;
    unsigned int    modmasks[] =
    {Mod1Mask, Mod2Mask, Mod3Mask, Mod4Mask, Mod5Mask};

    got_meta = got_numlock = 0;
    map = XGetModifierMapping(Xdisplay);
    kc = map->modifiermap;
    for (i = 3; i < 8; i++) {
	k = i * map->max_keypermod;
	for (j = 0; j < map->max_keypermod; j++, k++) {
	    if (kc[k] == 0)
		break;
	    switch (XKeycodeToKeysym(Xdisplay, kc[k], 0)) {
	    case XK_Num_Lock:
		if (!got_numlock) {
		    ModNumLockMask = modmasks[i - 3];
		    got_numlock = 1;
		}
		break;
	    case XK_Meta_L:
	    case XK_Meta_R:
	    case XK_Alt_L:
	    case XK_Alt_R:
		if (!got_meta) {
		    ModMetaMask = modmasks[i - 3];
		    got_meta = 1;
		}
		break;
	    }
	}
	if (got_meta && got_numlock)
	    break;
    }
    XFreeModifiermap(map);
}
