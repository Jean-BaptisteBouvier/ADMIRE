double FUNC_NAME_2 (struct STRUCTNAME *adt,double x,double y,int look) { 
				 /* Interpolate in 2-dimensional data table */
				 /* Before using the function, adt->inrow must be set if adt->dimension != 2 */
				 /* If the table is 1-dimensional, adt->block is */
				 /* automatically set to 0 */
				 /* The call is then made with lip2(&cnadam,ALPHA,M,look) */
				 /* Note that cnadam is just an example.. */
					   /* When look is set to 1, an interpolation is */
				 /* made. If not, an extrapolation is made from the */
					   /* previous point. */
  int row;
  int firstrow;
  int lastrow;
  int column;
  double newfunc,lip1high,lip1low;
  int OK;
  int block;
  int leftblock; /* If the table is 3-dimensional, this is the number of the current */
		 /* block to the left. */
  column = adt->dimension - 2;     /* Column for x-values */
  if (adt->dimension == 2) { 
    leftblock = 0;
    block = adt->block0; /* The place where we had the last hit. */
  } else { /* Never mind this for the moment */
    leftblock = adt->blockno[adt->inrow][column-1];
    block = adt->blockno[adt->inrow][column];
  }
  row = adt->bstart[block][column];

  /* Check which area we want to move within */
  if (look == 1) { /* Interpolate */
    if (adt->dimension == 2) {
      firstrow = 0;
      lastrow  = adt->bstart[adt->nb[column]-1][column] + 
		   adt->bsize[adt->nb[column]-1][column] - 1;
    } else {
      firstrow = adt->bstart[leftblock][column-1]; /* First row in leftblock */
      lastrow = adt->bstart[leftblock][column-1] + adt->bsize[leftblock][column-1] - 1;
    }

    /* Find values to interpolate between */
    OK = 0;
    if (x < adt->table[row][column]) { /* Check downwards i.e. towards lower row numbers */
      while (row > firstrow) {
	if (x > adt->table[row-1][column]) { /* A hit */
	  OK=1;
	  break;
	} else { /* Continue down */
	  block = block - 1;
	  row = row - adt->bsize[block][column];
	}
      } /* End of while loop */
    } else { /* Check upwards */
      while (row < lastrow) {
	if (x < adt->table[row][column]) { /* A hit */
	  if (row > firstrow) OK=1;
	  break;
	} else { /* Continue up */
	  row = row + adt->bsize[block][column];
	  block = block + 1;
	  if (row > lastrow) {
	    block = block - 1;
	    row = row - adt->bsize[block][column];
	    break;
	  }
	}
      }   /* End of while loop */
    } /* End of check both down & up */

    if (adt->dimension == 2) { /* Save the position */
      adt->block0 = block;
    } else {
      adt->where[leftblock][column-1] = row; /* Save position till the next time */
    }
    adt->inrow = adt->where[block][column];

    if (OK == 1) { /* Interpolate */
      lip1high = FUNC_NAME(adt,y,1);
      block = block - 1;
      adt->inrow = adt->where[block][column];
      lip1low = FUNC_NAME(adt,y,1); /* This also gives the values for adt->param_base[0] and adt->pd[0] */
	adt->pd[1] = (lip1high - lip1low)
	     / (adt->table[row][column] - adt->table[row-1][column]);
      newfunc = lip1low + (x - adt->table[row-1][column]) * adt->pd[1];
	adt->func_base[1] = newfunc;
	adt->param_base[1] = x;
    } else { /* Use endvalue */
      newfunc = FUNC_NAME(adt,y,1);
	adt->func_base[1] = newfunc;
      adt->param_base[1] = x;
	adt->pd[1] = 0;
    }
  } /* if (look = 1) */ else { /* Use old values and extrapolate */
    newfunc = adt->func_base[1] + (x-adt->param_base[1])* adt->pd[1]
	    + (y-adt->param_base[0])* adt->pd[0];
  }
  return(newfunc);
} /* End of lip2 */
