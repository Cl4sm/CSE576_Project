void rt_tristripscnv3fv(SceneHandle voidscene, void * tex,
                        int numverts, const float * cnv, int numstrips,
                        const int *vertsperstrip, const int *facets) {
  int strip, t, v;
  int stripaddr[2][3] = { {0, 1, 2}, {1, 0, 2} };
  scenedef * scene = (scenedef *) voidscene;

  /* render triangle strips one triangle at a time 
   * triangle winding order is:
   *   v0, v1, v2, then v2, v1, v3, then v2, v3, v4, etc.
   */
  /* loop over all of the triangle strips */
  for (strip=0, v=0; strip < numstrips; strip++) {
    /* loop over all triangles in this triangle strip */
    for (t=0; t < (vertsperstrip[strip] - 2); t++) {
      apivector v0, v1, v2;
      apivector n0, n1, n2;
      apicolor  c0, c1, c2;
      int a0, a1, a2;
      list * lst;
      object * o;

      /* copy the original input texture to each of the triangles... */
      /* converting to a vcstri texture if it isn't already          */
      vcstri_texture * newtex=rt_texture_copy_vcstri(scene, tex);
  
      /* add texture to the scene texture list */
      lst = (list *) malloc(sizeof(list));
      lst->item = (void *) newtex;
      lst->next = scene->texlist;
      scene->texlist = lst;

      /* render one triangle, using lookup table to fix winding order */
      a0 = facets[v + (stripaddr[t & 0x01][0])] * 10;
      a1 = facets[v + (stripaddr[t & 0x01][1])] * 10;
      a2 = facets[v + (stripaddr[t & 0x01][2])] * 10;

      c0.r = cnv[a0 + 0];
      c0.g = cnv[a0 + 1];
      c0.b = cnv[a0 + 2];
      n0.x = cnv[a0 + 4];
      n0.y = cnv[a0 + 5];
      n0.z = cnv[a0 + 6];
      v0.x = cnv[a0 + 7];
      v0.y = cnv[a0 + 8];
      v0.z = cnv[a0 + 9];

      c1.r = cnv[a1 + 0];
      c1.g = cnv[a1 + 1];
      c1.b = cnv[a1 + 2];
      n1.x = cnv[a1 + 4];
      n1.y = cnv[a1 + 5];
      n1.z = cnv[a1 + 6];
      v1.x = cnv[a1 + 7];
      v1.y = cnv[a1 + 8];
      v1.z = cnv[a1 + 9];

      c2.r = cnv[a2 + 0];
      c2.g = cnv[a2 + 1];
      c2.b = cnv[a2 + 2];
      n2.x = cnv[a2 + 4];
      n2.y = cnv[a2 + 5];
      n2.z = cnv[a2 + 6];
      v2.x = cnv[a2 + 7];
      v2.y = cnv[a2 + 8];
      v2.z = cnv[a2 + 9];

      o = newvcstri(newtex, v0, v1, v2, n0, n1, n2, c0, c1, c2); 
      if (scene->normalfixupmode)
        vcstri_normal_fixup(o, scene->normalfixupmode);
      add_bounded_object((scenedef *) scene, o);
      v++; /* move on to next vertex */
    }
    v+=2; /* last two vertices are already used by last triangle */
  }
}
