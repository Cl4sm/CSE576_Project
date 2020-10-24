                    apivector dir, flt start, flt end) {
  flt fallstart, fallend;
  point_light * li;
  scenedef * scene = (scenedef *) voidscene;
  list * lst;

  fallstart = start * 3.1415926 / 180.0;
  fallend   = end   * 3.1415926 / 180.0;
  VNorm(&dir);
  li = newspotlight(tex, ctr, rad, dir, fallstart, fallend);

  /* add light to the scene lightlist */
  lst = (list *) malloc(sizeof(list));
  lst->item = (void *) li;
  lst->next = scene->lightlist;
  scene->lightlist = lst;
  scene->numlights++;
 
  /* add light as an object as well... */
  add_bounded_object(scene, (object *) li);

  return li;
}
