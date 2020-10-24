void rt_clip_dv(SceneHandle voidscene, int numplanes, const double *planes) {
  list * lst;
  clip_group * clip; 
  int i;
  scenedef * scene = (scenedef *) voidscene;

  /* copy clipping info into structure */
  clip = (clip_group *) malloc(sizeof(clip_group));
  clip->numplanes = numplanes;
  clip->planes = (flt *) malloc(numplanes * sizeof(flt) * 4);
  for (i=0; i<(numplanes*4); i++) {
    clip->planes[i] = planes[i];
  }  

  /* add clipping info to the scene clip list */
  lst = (list *) malloc(sizeof(list));
  lst->item = (void *) clip;
  lst->next = scene->cliplist;
  scene->cliplist = lst;

  /* all objects added from this point on are added with this clip group */
  scene->curclipgroup = clip;
}
