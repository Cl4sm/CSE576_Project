  apivector a;
  int i;

  if ((points == NULL) || (numpts == 0)) {
    return;
  }

  if (numpts > 0) {
    rt_sphere(scene, tex, points[0], rad);
    
    if (numpts > 1) {
      for (i=1; i<numpts; i++) {
        a.x = points[i].x - points[i-1].x;
        a.y = points[i].y - points[i-1].y;
        a.z = points[i].z - points[i-1].z;
        
        rt_fcylinder(scene, tex, points[i-1], a, rad);
        rt_sphere(scene, tex, points[i], rad);
      }
    }
  }
}
