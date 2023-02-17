double FUNC_NAME_3(struct STRUCTNAME *adt,double x,double y,double z,int look) { 
				 /* Interpolate in 3-dimensional data table */
				 /* Before using the function, adt->inrow must be set if adt->dimension != 3 */
				 /* The call is then made with lip3(&cndnde,dn,de,alpha,look) */
				 /* Note that cndnde is just an example.. */
				 /* When look is set to 1, an interpolation is */
				 /* made. If not, an extrapolation is made from the */
				 /* previous point. */
  #define DEBUG
  int block;
  int row;
  int firstrow;
  int lastrow;
  int column;
  double newfunc,lip2high,lip2low;
  int OK;
  int leftblock;

  column = adt->dimension - 3;     /* Column for x-values */
  if (adt->dimension == 3) { 
    leftblock = 0;
    block = adt->block0; /* The place where we had the last hit. */
  } else { /* Never mind this for the moment */
    leftblock = adt->blockno[adt->inrow][column-1];
    block = adt->blockno[adt->inrow][column];
  }
  row = adt->bstart[block][column];

/* Check which area we want to move within */
  if (look == 1) { /* Interpolate */
    if (adt->dimension == 3) {
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
	} else { /* Continue down, i.e. downwards in parameter value. */
	  block = block - 1;
	  row = row - adt->bsize[block][column];
	}
      } /* End of while loop */
    } else { /* Check upwards */
      while (row < lastrow) {
	if (x < adt->table[row][column]) { /* A hit */
	  if (row > firstrow) OK=1; /* Otherwise it's no use to interpolate */
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

    if (adt->dimension == 3) { /* Save the position */
      adt->block0 = block;
    } else {
      adt->where[leftblock][column-1] = row; /* Save position till the next time */
    }
    adt->inrow = adt->where[block][column];

    if (OK == 1) { /* Interpolate */
      lip2high = FUNC_NAME_2(adt,y,z,1);
      block = block - 1;
      adt->inrow = adt->where[block][column];
      lip2low = FUNC_NAME_2(adt,y,z,1);
	adt->pd[2] = (lip2high - lip2low)/(adt->table[row][column] - adt->table[row-1][column]);
      newfunc = lip2low + (x - adt->table[row-1][column]) * adt->pd[2];
      adt->func_base[2] = newfunc;
      adt->param_base[2] = x;
    } else {
      newfunc = FUNC_NAME_2(adt,y,z,1);
      adt->func_base[2] = newfunc;
      adt->param_base[2] = x;
      adt->pd[2] = 0;
    }
  } /* look==1, interpolation ready */ else /* Extrapolate */ {
    newfunc = adt->func_base[2] + (x-adt->param_base[2])* adt->pd[2]
	  + (y-adt->param_base[1])* adt->pd[1]
	  + (z-adt->param_base[0])* adt->pd[0];
  } /* newfunc value ready */
  return(newfunc);
} /* End of lip3 */
