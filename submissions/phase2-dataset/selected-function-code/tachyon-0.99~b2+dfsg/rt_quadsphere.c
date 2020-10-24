void rt_quadsphere(SceneHandle scene, void * tex, apivector ctr, flt rad) {
  quadric * q;
  flt factor;
  q=(quadric *) newquadric();
  factor= 1.0 / (rad*rad);
  q->tex=tex;
  q->ctr=ctr;
 
  q->mat.a=factor;
  q->mat.b=0.0;
  q->mat.c=0.0;
  q->mat.d=0.0;
  q->mat.e=factor;
  q->mat.f=0.0;
  q->mat.g=0.0;
  q->mat.h=factor;
  q->mat.i=0.0;
  q->mat.j=-1.0;
 
  add_unbounded_object((scenedef *) scene, (object *)q);
}
