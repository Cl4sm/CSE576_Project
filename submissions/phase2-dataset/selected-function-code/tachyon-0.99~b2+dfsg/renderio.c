static void renderio(scenedef * scene) {
  flt iotime;
  char msgtxt[256];
  rt_timerhandle ioth; /* I/O timer handle */

  ioth=rt_timer_create();
  rt_timer_start(ioth);

  if (scene->imgbufformat == RT_IMAGE_BUFFER_RGB96F) {
    if (scene->imgprocess & RT_IMAGE_NORMALIZE) {
      normalize_rgb96f(scene->hres, scene->vres, (float *) scene->img);
      rt_ui_message(MSG_0, "Post-processing: normalizing pixel values.");
    }

    if (scene->imgprocess & RT_IMAGE_GAMMA) {
      gamma_rgb96f(scene->hres, scene->vres, (float *) scene->img, 
                   scene->imggamma);
      rt_ui_message(MSG_0, "Post-processing: gamma correcting pixel values.");
    }
  } else if (scene->imgbufformat == RT_IMAGE_BUFFER_RGB24) {
    if (scene->imgprocess & (RT_IMAGE_NORMALIZE | RT_IMAGE_GAMMA))
      rt_ui_message(MSG_0, "Can't post-process 24-bit integer image data");
  }

  /* support cropping of output images for SPECMPI benchmarks */
  if (scene->imgcrop.cropmode == RT_CROP_DISABLED) {
    writeimage(scene->outfilename, scene->hres, scene->vres, 
               scene->img, scene->imgbufformat, scene->imgfileformat);
  } else {
    /* crop image before writing if necessary */
    if (scene->imgbufformat == RT_IMAGE_BUFFER_RGB96F) {
      float *imgcrop;
      imgcrop = image_crop_rgb96f(scene->hres, scene->vres, scene->img,
                                  scene->imgcrop.xres, scene->imgcrop.yres, 
                                  scene->imgcrop.xstart, scene->imgcrop.ystart);
      writeimage(scene->outfilename, scene->imgcrop.xres, scene->imgcrop.yres,
                 imgcrop, scene->imgbufformat, scene->imgfileformat);
      free(imgcrop);
    } else if (scene->imgbufformat == RT_IMAGE_BUFFER_RGB24) {
      unsigned char *imgcrop;
      imgcrop = image_crop_rgb24(scene->hres, scene->vres, scene->img,
                                 scene->imgcrop.xres, scene->imgcrop.yres, 
                                 scene->imgcrop.xstart, scene->imgcrop.ystart);
      writeimage(scene->outfilename, scene->imgcrop.xres, scene->imgcrop.yres,
                 imgcrop, scene->imgbufformat, scene->imgfileformat);
      free(imgcrop);
    }
  }

  rt_timer_stop(ioth);
  iotime = rt_timer_time(ioth);
  rt_timer_destroy(ioth);

  sprintf(msgtxt, "    Image I/O Time: %10.4f seconds", iotime);
  rt_ui_message(MSG_0, msgtxt);
}
