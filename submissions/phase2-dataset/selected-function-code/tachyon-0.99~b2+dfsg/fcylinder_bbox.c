  cylinder * c = (cylinder *) obj;
  vector mintmp, maxtmp;

  mintmp.x = c->ctr.x;
  mintmp.y = c->ctr.y;
  mintmp.z = c->ctr.z;
  maxtmp.x = c->ctr.x + c->axis.x;
  maxtmp.y = c->ctr.y + c->axis.y;
  maxtmp.z = c->ctr.z + c->axis.z;

  min->x = MYMIN(mintmp.x, maxtmp.x);
  min->y = MYMIN(mintmp.y, maxtmp.y);
  min->z = MYMIN(mintmp.z, maxtmp.z);
  min->x -= c->rad;
  min->y -= c->rad;
  min->z -= c->rad;

  max->x = MYMAX(mintmp.x, maxtmp.x);
  max->y = MYMAX(mintmp.y, maxtmp.y);
  max->z = MYMAX(mintmp.z, maxtmp.z);
  max->x += c->rad;
  max->y += c->rad;
  max->z += c->rad;

  return 1;
}
