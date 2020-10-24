void * rt_directional_light(SceneHandle voidscene, void * tex, apivector dir) {
  directional_light * li;
  scenedef * scene = (scenedef *) voidscene;
  list * lst;

  VNorm(&dir);
  li=newdirectionallight(tex, dir);

  /* add light to the scene lightlist */
  lst = (list *) malloc(sizeof(list));
  lst->item = (void *) li;
  lst->next = scene->lightlist;
  scene->lightlist = lst;
  scene->numlights++;

  /* don't add to the object list since it's at infinity */
  /* XXX must loop over light list and deallocate these  */
  /*     specially since they aren't in the obj list.    */
 
  return li;
}
