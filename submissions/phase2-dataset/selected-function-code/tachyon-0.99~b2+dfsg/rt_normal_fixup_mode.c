void rt_normal_fixup_mode(SceneHandle voidscene, int mode) {
  scenedef * scene = (scenedef *) voidscene;
  switch (mode) {
    /* RT_NORMAL_FIXUP_MODE_GUESS */
    case 2:
      scene->normalfixupmode = 2; /* accept any normal/winding order combo   */
                                  /* and suffer the consequences, since this */
                                  /* leaves an unhandled case where surface  */
                                  /* normals on poorly tessellated objects   */
                                  /* will cause black edges.                 */
      break;

    /* RT_NORMAL_FIXUP_MODE_FLIP */
    case 1:
      scene->normalfixupmode = 1; /* reverse the surface normal     */
      break;

    /* RT_NORMAL_FIXUP_MODE_OFF */
    case 0:
    default: 
      scene->normalfixupmode = 0; /* use strict winding order rules */
      break;
  }
}
