void v_init(hfsvol *vol, int flags)
{
  btree *ext = &vol->ext;
  btree *cat = &vol->cat;

  vol->priv       = 0;
  vol->flags      = flags & HFS_VOL_OPT_MASK;

  vol->pnum       = -1;
  vol->vstart     = 0;
  vol->vlen       = 0;
  vol->lpa        = 0;

  vol->cache      = 0;

  vol->vbm        = 0;
  vol->vbmsz      = 0;

  f_init(&ext->f, vol, HFS_CNID_EXT, "extents overflow");

  ext->map        = 0;
  ext->mapsz      = 0;
  ext->flags      = 0;

  ext->keyunpack  = (keyunpackfunc)  r_unpackextkey;
  ext->keycompare = (keycomparefunc) r_compareextkeys;

  f_init(&cat->f, vol, HFS_CNID_CAT, "catalog");

  cat->map        = 0;
  cat->mapsz      = 0;
  cat->flags      = 0;

  cat->keyunpack  = (keyunpackfunc)  r_unpackcatkey;
  cat->keycompare = (keycomparefunc) r_comparecatkeys;

  vol->cwd        = HFS_CNID_ROOTDIR;

  vol->refs       = 0;
  vol->files      = 0;
  vol->dirs       = 0;

  vol->prev       = 0;
  vol->next       = 0;
}
