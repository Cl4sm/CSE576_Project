                  const float *v0, const float *v1, const float *v2, 
                  const float *n0, const float *n1, const float *n2, 
                  const float *c0, const float *c1, const float *c2) {
  scenedef * scene = (scenedef *) voidscene;
  vector vv0, vv1, vv2, vn0, vn1, vn2;
  color cc0, cc1, cc2;
  object * o;

  vv0.x = v0[0]; vv0.y = v0[1]; vv0.z = v0[2];
  vv1.x = v1[0]; vv1.y = v1[1]; vv1.z = v1[2]; 
  vv2.x = v2[0]; vv2.y = v2[1]; vv2.z = v2[2];
  vn0.x = n0[0]; vn0.y = n0[1]; vn0.z = n0[2];
  vn1.x = n1[0]; vn1.y = n1[1]; vn1.z = n1[2]; 
  vn2.x = n2[0]; vn2.y = n2[1]; vn2.z = n2[2];
  cc0.r = c0[0]; cc0.g = c0[1]; cc0.b = c0[2];
  cc1.r = c1[0]; cc1.g = c1[1]; cc1.b = c1[2]; 
  cc2.r = c2[0]; cc2.g = c2[1]; cc2.b = c2[2];

  o = newvcstri(tex, vv0, vv1, vv2, vn0, vn1, vn2, cc0, cc1, cc2);
  /* don't add degenerate triangles */
  if (o != NULL) {
    if (scene->normalfixupmode)
      vcstri_normal_fixup(o, scene->normalfixupmode);
    add_bounded_object(scene, o);
  }
} 
