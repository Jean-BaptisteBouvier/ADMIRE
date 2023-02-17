double FUNC_NAME (struct STRUCTNAME *adt,double x,int look) {
	
	/* Interpolate in 1-dimensional data table              */
	/* Before using the function, adt->inrow must be set if */
	/* adt->dimension != 1.                                 */
	/* If the table is 1-dimensional, adt->block is         */
	/* automatically set to 0                               */
	/* The call is then made with lip1(&ctzero,M,look)      */
	/* Note that ctzero is just an example..                */
	/* When look is set to 1, an interpolation is           */
	/* made. If not, an extrapolation is made from the      */
	/* previous point.                                      */
	/*                                                      */
	/* Last change 1999-02-16 by Gunnar Hovmark.            */
	
	int row,firstrow,lastrow,column;
	int leftblock,block,OK;
	double newfunc;
	
	if (look == 1) { /* Perform interpolation */
		column = adt->dimension - 1;     /* Column for x-values */
		if (adt->dimension == 1) { 
			block = 0;
			row = adt->block0;
		} else {
			leftblock = adt->blockno[adt->inrow][column-1];
			block = adt->blockno[adt->inrow][column];
			row = adt->where[leftblock][column-1]; /* Row where latest hit was */
		}

		firstrow = adt->bstart[block][column]; /* First row in block */
		lastrow = firstrow + adt->bsize[block][column] - 1;
		if (row < firstrow) row = firstrow;
		/* Find values to interpolate between */
		OK = 0;
		if (x < adt->table[row][column]) { /* Check downwards */
			while (row > firstrow) {
				if (x > adt->table[row-1][column]) { /* A hit */
					OK=1;
					break;
				} else { /* Continue down. I.e. we go upwards in the table, but down in parameter value */
					--row;
				}
			} /* End of while loop */
		} else { /* Check upwards */
			while (row <= lastrow) {
				if (x <= adt->table[row][column]) { /* A hit */
					if (row > firstrow) OK=1;
					break;
				} else { /* Continue up */
					if (row < lastrow) {++row;} else { break; }
				}
			} /* End of while loop */
		} /* End of check both down & up */

		if (adt->dimension > 1) {
			adt->where[leftblock][column-1] = row;
		} else {
			adt->block0 = row;
		}

		if (OK == 1) { /* Interpolate */
			adt->pd[0] = (adt->table[row][column+1] - adt->table[row-1][column+1]) /
				(adt->table[row][column] - adt->table[row-1][column]); /* Partial derivative */
			newfunc = adt->table[row-1][column+1] + (x - adt->table[row-1][column]) * adt->pd[0];
			adt->func_base[0] = newfunc;
			adt->param_base[0] = x;
		} else { /* Take the limit value. This means that we will use the limit value */
			newfunc = adt->table[row][column+1];
			adt->func_base[0] = newfunc;
			adt->param_base[0] = x;
			adt->pd[0] = 0;
		} /* Limit value used. Do we need a warning for this? */
	} /* if (look), interpolation ready */ else { /* Extrapolate */
		newfunc = adt->func_base[0] + (x-adt->param_base[0])* adt->pd[0];
	}

	return(newfunc);
} 
/* End of lip1 */
