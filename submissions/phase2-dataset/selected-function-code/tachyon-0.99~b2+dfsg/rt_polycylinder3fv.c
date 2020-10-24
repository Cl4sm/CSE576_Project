                        const float *points, int numpts, float rad) {
  float a[3];
  int i;

  if ((points == NULL) || (numpts == 0)) {
    return;
  }

  if (numpts > 0) {
    rt_sphere3fv(scene, tex, points, rad);
    
    if (numpts > 1) {
      for (i=1; i<numpts; i++) {
        int addr = i*3;
        a[0] = points[addr    ] - points[addr - 3];
        a[1] = points[addr + 1] - points[addr - 2];
        a[2] = points[addr + 2] - points[addr - 1];
        
        rt_fcylinder3fv(scene, tex, &points[addr-3], a, rad);
        rt_sphere3fv(scene, tex, &points[addr], rad);
      }
    }
  }
}
