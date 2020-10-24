void camera_init(scenedef *scene) {
  flt sx, sy;
  vector newupvec;
  vector newviewvec;
  vector newrightvec;

  /* recompute the camera vectors */
  VCross(&scene->camera.upvec, &scene->camera.viewvec, &newrightvec);
  VNorm(&newrightvec);

  VCross(&scene->camera.viewvec, &newrightvec, &newupvec);
  VNorm(&newupvec);

  newviewvec=scene->camera.viewvec;
  VNorm(&newviewvec);
  scene->camera.rightvec=newrightvec;
  scene->camera.upvec=newupvec;

  sx = (flt) scene->hres; 
  sy = (flt) scene->vres;

  /* calculate the width and height of the image plane in world coords */
  /* given the aspect ratio, image resolution, and zoom factor */
  scene->camera.px=((sx / sy) / scene->aspectratio) / scene->camera.camzoom;
  scene->camera.py=1.0 / scene->camera.camzoom;
  scene->camera.psx = scene->camera.px / sx;
  scene->camera.psy = scene->camera.py / sy;

  if (scene->camera.frustumcalc == RT_CAMERA_FRUSTUM_AUTO) {
    scene->camera.left   = -0.5 * scene->camera.px;
    scene->camera.right  =  0.5 * scene->camera.px;
    scene->camera.bottom = -0.5 * scene->camera.py;
    scene->camera.top    =  0.5 * scene->camera.py;
  }
  
  /* setup function pointer for camera ray generation */
  switch (scene->camera.projection) {
    case RT_PROJECTION_PERSPECTIVE:
      if (scene->antialiasing > 0) {
        scene->camera.cam_ray = (color (*)(void *,flt,flt)) cam_aa_perspective_ray;
      } else {
        scene->camera.cam_ray = (color (*)(void *,flt,flt)) cam_perspective_ray;
      }
      break;

    case RT_PROJECTION_PERSPECTIVE_DOF:
      scene->camera.cam_ray = (color (*)(void *,flt,flt)) cam_aa_dof_ray;
      break;

    case RT_PROJECTION_ORTHOGRAPHIC:
      if (scene->antialiasing > 0) {
        scene->camera.cam_ray = (color (*)(void *,flt,flt)) cam_aa_orthographic_ray;
      } else {
        scene->camera.cam_ray = (color (*)(void *,flt,flt)) cam_orthographic_ray;
      }
      break;

    case RT_PROJECTION_FISHEYE:
      if (scene->antialiasing > 0) {
        scene->camera.cam_ray = (color (*)(void *,flt,flt)) cam_aa_fisheye_ray;
      } else {
        scene->camera.cam_ray = (color (*)(void *,flt,flt)) cam_fisheye_ray;
      }
      break;
  }


  /* assuming viewvec is a unit vector, then the center of the */
  /* image plane is the camera center + vievec                 */
  switch (scene->camera.projection) { 
    case RT_PROJECTION_ORTHOGRAPHIC:
      scene->camera.projcent = scene->camera.center;

      /* assuming viewvec is a unit vector, then the lower left    */
      /* corner of the image plane is calculated below             */
      scene->camera.lowleft.x = scene->camera.projcent.x +
        (scene->camera.left   * scene->camera.rightvec.x) +
        (scene->camera.bottom * scene->camera.upvec.x);
      scene->camera.lowleft.y = scene->camera.projcent.y +
        (scene->camera.left   * scene->camera.rightvec.y) +
        (scene->camera.bottom * scene->camera.upvec.y);
      scene->camera.lowleft.z = scene->camera.projcent.z +
        (scene->camera.left   * scene->camera.rightvec.z) +
        (scene->camera.bottom * scene->camera.upvec.z);
      break;
  
    case RT_PROJECTION_PERSPECTIVE_DOF:
      scene->camera.projcent.x = scene->camera.center.x + 
                        (scene->camera.focallength * scene->camera.viewvec.x);
      scene->camera.projcent.y = scene->camera.center.y + 
                        (scene->camera.focallength * scene->camera.viewvec.y);
      scene->camera.projcent.z = scene->camera.center.z + 
                        (scene->camera.focallength * scene->camera.viewvec.z);

      /* assuming viewvec is a unit vector, then the lower left    */
      /* corner of the image plane is calculated below             */
      scene->camera.lowleft.x = scene->camera.projcent.x +
        (scene->camera.left   * scene->camera.rightvec.x) +
        (scene->camera.bottom * scene->camera.upvec.x);
      scene->camera.lowleft.y = scene->camera.projcent.y +
        (scene->camera.left   * scene->camera.rightvec.y) +
        (scene->camera.bottom * scene->camera.upvec.y);
      scene->camera.lowleft.z = scene->camera.projcent.z +
        (scene->camera.left   * scene->camera.rightvec.z) +
        (scene->camera.bottom * scene->camera.upvec.z);
      break;

    case RT_PROJECTION_FISHEYE:
      scene->camera.projcent.x = scene->camera.center.x + 
                        (scene->camera.focallength * scene->camera.viewvec.x);
      scene->camera.projcent.y = scene->camera.center.y + 
                        (scene->camera.focallength * scene->camera.viewvec.y);
      scene->camera.projcent.z = scene->camera.center.z + 
                        (scene->camera.focallength * scene->camera.viewvec.z);
      break;

    case RT_PROJECTION_PERSPECTIVE:
    default:
      scene->camera.projcent.x = scene->camera.center.x + 
                        (scene->camera.focallength * scene->camera.viewvec.x);
      scene->camera.projcent.y = scene->camera.center.y + 
                        (scene->camera.focallength * scene->camera.viewvec.y);
      scene->camera.projcent.z = scene->camera.center.z + 
                        (scene->camera.focallength * scene->camera.viewvec.z);

      /* assuming viewvec is a unit vector, then the lower left    */
      /* corner of the image plane is calculated below             */
      /* for normal perspective rays, we are really storing the    */
      /* direction to the lower left, not the lower left itself,   */
      /* since this allows us to eliminate a subtraction per pixel */
      scene->camera.lowleft.x = scene->camera.projcent.x +
        (scene->camera.left   * scene->camera.rightvec.x) +
        (scene->camera.bottom * scene->camera.upvec.x)
        - scene->camera.center.x;
      scene->camera.lowleft.y = scene->camera.projcent.y +
        (scene->camera.left   * scene->camera.rightvec.y) +
        (scene->camera.bottom * scene->camera.upvec.y)
        - scene->camera.center.y;
      scene->camera.lowleft.z = scene->camera.projcent.z +
        (scene->camera.left   * scene->camera.rightvec.z) +
        (scene->camera.bottom * scene->camera.upvec.z)
        - scene->camera.center.z;
      break;
  }

  /* size of image plane */
  scene->camera.px = scene->camera.right - scene->camera.left; 
  scene->camera.py = scene->camera.top - scene->camera.bottom; 
  scene->camera.psx = scene->camera.px / scene->hres;
  scene->camera.psy = scene->camera.py / scene->vres;

  scene->camera.iplaneright.x = scene->camera.px * scene->camera.rightvec.x / sx;
  scene->camera.iplaneright.y = scene->camera.px * scene->camera.rightvec.y / sx;
  scene->camera.iplaneright.z = scene->camera.px * scene->camera.rightvec.z / sx;
  
  scene->camera.iplaneup.x = scene->camera.py * scene->camera.upvec.x / sy;
  scene->camera.iplaneup.y = scene->camera.py * scene->camera.upvec.y / sy;
  scene->camera.iplaneup.z = scene->camera.py * scene->camera.upvec.z / sy;
}
