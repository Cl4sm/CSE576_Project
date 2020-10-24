     XPoint P1, Q1, P2, Q2;
     
{
  XPoint *result;
  int vert1 = 0, vert2 = 0, horiz1 = 0, horiz2 = 0;
  double alpha1, beta1, alpha2, beta2;
  double x, y;

  if (P1.x != Q1.x) /* not vertical */
    {
      alpha1 = (Q1.y*1. - P1.y*1.) / (Q1.x*1. - P1.x*1.);
      beta1 = P1.y*1. - alpha1 * P1.x*1.;
      if (Q1.y == P1.y) horiz1 = 1;
    }
  else /* vertical */
    {
      vert1 = 1;
      /* check line1 not point */
      if (P1.y == Q1.y) return(NULL);
      alpha1 = 0.;
      beta1 = P1.x;
    }

  if (P2.x != Q2.x) /* not vertical */
    {
      if (P2.y == Q2.y) horiz2 = 1;
      if (!vert1)
	{
	  alpha2 = (Q2.y*1. - P2.y*1.) / (Q2.x*1. - P2.x*1.);
	  beta2 = P2.y*1. - alpha2 * P2.x*1.;
	}
      else if (!horiz2) /* and vert1 implicitly */
	{
	  alpha2 = (Q2.x*1. - P2.x*1.) / (Q2.y*1. - P2.y*1.);
	  beta2 = P2.x*1. - alpha2 * P2.y*1.;
	}
      else /* vert1 and horiz2 */
	{
	  alpha2 = 0.;
	  beta2 = P2.y;
	}
    }
  else /* vertical */
    {
      vert2 = 1;
      /* check line2 not point */
      if (P2.y == Q2.y) return(NULL);
      alpha2 = 0.;
      beta2 = P2.x;
      if ((!vert1) && (!horiz1))
	{
	  alpha1 = (Q1.x*1. - P1.x*1.) / (Q1.y*1. - P1.y*1.);
	  beta1 = P1.x*1. - alpha1 * P1.y*1.;
	}	  
    }
  
#ifdef DEBUG
  printf("D1 vert %d horiz %d, D2 vert %d horiz %d\n", vert1, horiz1, 
	 vert2, horiz2);
  printf("alpha1 %lf beta1 %lf, alpha2 %lf beta2 %lf\n", alpha1, beta1,
	 alpha2, beta2);
#endif /* DEBUG */

  if ((vert1 && vert2) || (horiz1 && horiz2))
    {
      if (!fabs(beta1 - beta2) > PRECISION) return(NULL);
      x = P1.x;
      y = P1.y;
    }
  else if ((vert1 && horiz2) || (vert2 && horiz1))
    {
      if (vert1)
	  x = P1.x , y = P2.y;
      else 
	  x = P2.x , y = P1.y;
    }
  else if ((vert1 && !vert2 && !horiz2) || (vert2 && !vert1 && !horiz1))
    {
      if (vert1)
	  x = P1.x , y = (x - beta2) / alpha2;
      else x = P2.x , y = (x - beta1) / alpha1;
    }
  else if ((horiz1 && !vert2 && !horiz2) || (horiz2 && !vert1 && !horiz1))
    {
      if (horiz1)
	y = P1.y , x = (y - beta2) / alpha2;
      else y = P2.y , x = (y - beta1) / alpha1;
    }
  else if (!vert1 && !horiz1 && !vert2 && !horiz2)
    {
      if (fabs(alpha1 - alpha2) > PRECISION) /* alpha1 != alpha2 */
	{
	  x = (beta2 - beta1) / (alpha1 - alpha2);
	  y = alpha1 * x + beta1;
	}
      else if (fabs(beta1 - beta2) > PRECISION) /* alpha1 = alpha2
						   && beta1 != beta2 */
	return(NULL);
      else /* alpha1 = alpha2 && beta1 = beta2 */
	x = P1.x , y = P1.y;
    }

  /* now check if solution in between P1, Q1 and P2, Q2, else return NULL */
  if (((Position)roundint(x) < min(P1.x, Q1.x)) || 
      ((Position)roundint(x) > max(P1.x, Q1.x)) ||
      ((Position)roundint(y) < min(P1.y, Q1.y)) ||
      ((Position)roundint(y) > max(P1.y, Q1.y)) ||
      ((Position)roundint(x) < min(P2.x, Q2.x)) || 
      ((Position)roundint(x) > max(P2.x, Q2.x)) ||
      ((Position)roundint(y) < min(P2.y, Q2.y)) ||
      ((Position)roundint(y) > max(P2.y, Q2.y)))
    return(NULL);
  else
    {
      result = (XPoint *) XtMalloc(sizeof(XPoint));
      result->x = (Position) roundint(x);
      result->y = (Position) roundint(y);
      return(result);
    }
}
