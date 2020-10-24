  scenedef * scene;
  SceneHandle voidscene;
  apicolor bgcolor = rt_color(0.0, 0.0, 0.0);
  apicolor ambcolor = rt_color(1.0, 1.0, 1.0);

  scene = (scenedef *) malloc(sizeof(scenedef));
  memset(scene, 0, sizeof(scenedef));             /* clear all valuas to 0  */

  voidscene = (SceneHandle) scene;

  rt_outputfile(voidscene, "/tmp/outfile.tga");   /* default output file    */
  rt_crop_disable(voidscene);                     /* disable cropping */
  rt_outputformat(voidscene, RT_FORMAT_TARGA);    /* default iamge format   */
  rt_resolution(voidscene, 512, 512);             /* 512x512 resolution     */
  rt_verbose(voidscene, 0);                       /* verbose messages off   */

  rt_image_gamma(voidscene, 2.2f);                /* set default gamma */
  rt_image_clamp(voidscene);                      /* clamp image colors */

#if 1
  rt_rawimage_rgb96f(voidscene, NULL);            /* raw image output off   */
#else
  rt_rawimage_rgb24(voidscene, NULL);             /* raw image output off   */
#endif

  rt_boundmode(voidscene, RT_BOUNDING_ENABLED);   /* spatial subdivision on */
  rt_boundthresh(voidscene, BOUNDTHRESH);         /* default threshold      */
  rt_camera_setup(voidscene, 1.0, 1.0, 0, 6,
                  rt_vector(0.0, 0.0, 0.0),
                  rt_vector(0.0, 0.0, 1.0),
                  rt_vector(0.0, 1.0, 0.0));
  rt_camera_dof(voidscene, 1.0, 0.0);
  rt_shadermode(voidscene, RT_SHADER_AUTO);
  rt_rescale_lights(voidscene, 1.0);
  rt_phong_shader(voidscene, RT_SHADER_BLINN);

  rt_background(voidscene, bgcolor);
  rt_background_sky_sphere(voidscene, rt_vector(0.0, 1.0, 0.0), 0.3, 0, 
                           rt_color(0.0, 0.0, 0.0), rt_color(0.0, 0.0, 0.5));
  rt_background_mode(voidscene, RT_BACKGROUND_TEXTURE_SOLID);

  rt_ambient_occlusion(voidscene, 0, ambcolor);    /* disable AO by default  */
  rt_fog_rendering_mode(voidscene, RT_FOG_NORMAL); /* radial fog by default  */
  rt_fog_mode(voidscene, RT_FOG_NONE);             /* disable fog by default */
  rt_fog_parms(voidscene, bgcolor, 0.0, 1.0, 1.0);

  /* use max positive integer for max transparent surface limit by default */
  rt_trans_max_surfaces(voidscene,((((int)1) << ((sizeof(int) * 8) - 2))-1)*2);

  rt_trans_mode(voidscene, RT_TRANS_ORIG);         /* set transparency mode  */
  rt_normal_fixup_mode(voidscene, 0);              /* disable normal fixup   */
  rt_shadow_filtering(voidscene, 1);               /* shadow filtering on    */

  scene->objgroup.boundedobj = NULL;
  scene->objgroup.unboundedobj = NULL;
  scene->objgroup.numobjects = 0;

  scene->texlist = NULL;
  scene->lightlist = NULL;
  scene->cliplist = NULL;
  scene->numlights = 0;
  scene->scenecheck = 1;
  scene->parbuf = NULL;
  scene->threads = NULL;
  scene->threadparms = NULL;
  scene->flags = RT_SHADE_NOFLAGS;
 
  rt_set_numthreads(voidscene, -1);         /* auto determine num threads */ 

  /* number of distributed memory nodes, fills in array of node/cpu info */
  scene->nodes = rt_getcpuinfo(&scene->cpuinfo);
  scene->mynode = rt_mynode();

  return scene;
}
