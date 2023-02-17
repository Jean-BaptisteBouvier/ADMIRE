/* Program to convert .AER files to C header files */
/* Change infilename, outfilename and incfilename to decide which */
/* file should be converted. */
/* Last change by Gunnar Hovmark 1999-08-05 */

#include <stdio.h>
#include <string.h>
#include <ctype.h> /* See 7.8.2 in Kernighan & Ritchie */

#define MAXROWS 2000
#define MAXCOLS 5
/* #define DEBUG */

int fgetline(char s[], int lim,FILE *fp) { /* Read a line into s, return the length */
  int c, i;
  /* c=getchar(); Flush the latest '\n', sometimes needed, sometimes not... */
  for (i=0;i<lim-1 && (c=fgetc(fp)) !='\n' && c != EOF;++i) s[i] = c;
  s[i] = '\0';
  if (c == EOF) {i=EOF;} else {i=1;}
  return(i);
}

void main() {
  FILE *in;
  FILE *out;
  FILE *incout;
  short i, j, read_result, new_table, nbmax;
/* Count the number of tables, total and of the various sizes. */
  int tables = 0;
  int tables1d = 0;
  int tables2d = 0;
  int tables3d = 0;
  int tables4d = 0;
  int tablesnd = 0;
  int maxdim = 0;
  int go_on = 1;

  int dimension;
  int tab_rows;
  short tab_columns;
  float table[MAXROWS][MAXCOLS];
  short nb[MAXCOLS];
  int bsize[MAXROWS][MAXCOLS];
  int bstart[MAXROWS][MAXCOLS];
  short blockno[MAXROWS][MAXCOLS];
  char structname[12];
  char varname[12];
  char commentline[76];
  char startcomment[4]="/* ";
  char endcomment[4]=" */";
  char incomment[70];
  char dataline[80];

  char infilename[40];
  char outfilename[40];
  char incfilename[40];
/*   strcpy(infilename,"chigh.aer"); */
/*   strcpy(outfilename,"aerodata_high.h"); */
/*   strcpy(incfilename,"setup_aerodata_high.h"); */
  strcpy(infilename,"asym_mod.aer");
  strcpy(outfilename,"aerodata_asym_ext.h");
  strcpy(incfilename,"setup_aerodata_asym_ext.h");

  in = fopen(infilename,"r");
  out = fopen(outfilename,"w");
  incout = fopen(incfilename,"w");

/* Start reading */
  do { /* Read until we reach EOF */
    if (new_table==1) {
      if (sscanf(dataline,"%s\n",structname) != 1) { read_result=EOF; break;}
    } else {
      if (fscanf(in,"%s\n",structname) == EOF) { read_result=EOF; break;}
    }
    fprintf(out,"struct %s {\n",structname);
    printf("Structname: %s\n",structname);
    for (i=0;i<2;++i){
      fgetline(incomment,70,in);
      strcpy(commentline,startcomment);
      strcat(commentline,incomment);
      strcat(commentline,endcomment);
      fprintf(out,"  %s\n",commentline);
    }
    fscanf(in,"%d\n",&dimension);
    ++tables;
    if (maxdim < dimension) {maxdim = dimension;}
    switch (dimension) {
      case 1:
	++tables1d;
	break;
      case 2:
	++tables2d;
	break;
      case 3:
	++tables3d;
	break;
      case 4:
	++tables4d;
	break;
      default:
	++tablesnd;
	break;
    }
    tab_columns = dimension+1;
    fprintf(out,"  int dimension; /* %d */\n",dimension);
    for (i=0;i<dimension;++i){
      fgetline(incomment,74,in);
      strcpy(commentline,startcomment);
      strcat(commentline,incomment);
      strcat(commentline,endcomment);
      fprintf(out,"  %s\n",commentline);
    }
    /* And now it's time taking in the table... */
    tab_rows = 0;
    do { /* Read values until we encounter a new structname */
      if (fgetline(dataline,80,in)== EOF) {
	read_result=EOF;
      } else {
	read_result = 1;
      }
#ifdef DEBUG
     printf("%s\n",dataline);
#endif
      /* Convert to numbers */
      switch (dimension) {
	case 1:
	  if (sscanf(dataline,"%f %f\n",&table[tab_rows][0],&table[tab_rows][1])!=2) { /* Error */
	    new_table=1;
	  } else {
	    new_table = 0;
	  }
	  break;
	case 2:
	  if (sscanf(dataline,"%f %f %f\n",&table[tab_rows][0],&table[tab_rows][1],&table[tab_rows][2])!=3) { /* Error */
	    new_table = 1;
	  } else {
	    new_table = 0;
	  }
	  break;
	case 3:
	  if (sscanf(dataline,"%f %f %f %f\n",&table[tab_rows][0],&table[tab_rows][1],&table[tab_rows][2],&table[tab_rows][3])!=4) { /* Error */
	    new_table = 1;
	  } else {
	    new_table = 0;
	  }
	  break;
	case 4:
	  if (sscanf(dataline,"%f %f %f %f %f\n",&table[tab_rows][0],&table[tab_rows][1],&table[tab_rows][2],&table[tab_rows][3],&table[tab_rows][4])!=5) { /* Error */
	    new_table = 1;
	  } else {
	    new_table = 0;
	  }
	  break;
      }   /* End of switch */
      ++tab_rows;
    } while(new_table == 0);
    --tab_rows;
    /* OK, we reached the end of that table, now it's time to print it out */
    /* First a few declarations */
    if(tab_rows > MAXROWS) {
      printf("No of rows in table %s: %d, exceeds MAXROWS\n",structname,tab_rows);
      scanf("Continue? (0/1) %d\n",go_on);
    }
    if(tab_columns > MAXCOLS) {
      printf("No of columns in table %s: %d, exceeds MAXCOLS\n",structname,tab_columns);
      scanf("Continue? (0/1) %d\n",go_on);
    }
    fprintf(out,"  int tab_rows;\n");
    fprintf(out,"  double table[%d][%d];\n",tab_rows,tab_columns);
    fprintf(out,"  short block;\n");
    fprintf(out,"  short block0;\n");
    fprintf(out,"  int inrow;\n");

    fprintf(out,"  short nb[%d]; /* No of blocks in each column */\n",dimension);
    fprintf(out,"  double pd[%d]; /* Partial derivatives, used for interpolation and extrapolation */\n",dimension);
    fprintf(out,"  double param_base[%d]; /* Parameter base values for extrapolation */\n",dimension);
    fprintf(out,"  double func_base[%d]; /* Function base values for extrapolation */\n",dimension);
    /* Then come a number of matrixes that I don't know the dimensions of yet */
    /* Figure that out here... */
    /* Check how many blocks there are in each column, and where they begin */
    /* Check which column has the maximum number of blocks.. */
    nbmax = 0;
    for (j=0;j<dimension;++j) {
      for(i=0;i<tab_rows;++i) {
	if (i==0) { /* First block starts here, no doubt about that. */
	  nb[j] = 1;
	  bstart[nb[j]-1][j] = i;
	} else { /* Find the border to the next block */
	  if (j==dimension-1) {
	    if (table[i-1][j]>table[i][j]){/* Not monotonically inreasing */
	      bsize[nb[j]-1][j] = i - bstart[nb[j]-1][j];
	      nb[j] = nb[j] + 1;
	      bstart[nb[j]-1][j] = i;
	    }
	  } else { /* j < dimension-1, check where values change */
	    if (table[i-1][j] != table[i][j]){/* Value in column changed */
	      bsize[nb[j]-1][j] = i - bstart[nb[j]-1][j];
	      nb[j] = nb[j] + 1;
	      bstart[nb[j]-1][j] = i;
	    }
	  }
	  if (i==tab_rows-1) { /* End of column */
	    bsize[nb[j]-1][j] = i - bstart[nb[j]-1][j] + 1;
	  }
	} /* Block border found */
	blockno[i][j] = nb[j]-1;  /* Save this for the blockno matrix */
      } /* End of i loop */
      if(nbmax<nb[j]) { nbmax = nb[j]; }
#ifdef DEBUG
      printf("J: %d\t Nbmax: %d\n",j ,nbmax);
#endif
    } /* End of j loop */
    /* Now print the declarations */
    fprintf(out,"  int where [%d][%d];\n",nbmax,dimension);
    fprintf(out,"  short bsize [%d][%d];\n",nbmax,dimension);
    fprintf(out,"  int bstart [%d][%d];\n",nbmax,dimension);
    fprintf(out,"  short blockno [%d][%d];\n",tab_rows,dimension);
    /* Next, get a varname out of the structname, */
    /* i.e. varname is structname in lower case */
    i=-1;
    do {
      ++i;
      varname[i] = tolower(structname[i]);
    } while(structname[i] != '\0');
    fprintf(out,"} %s = { \n",varname);
    fprintf(out,"   %d /* dimension */\n",dimension);
    fprintf(out,"  ,%d /* tab_rows */\n",tab_rows);
    i=0;
    do { /* Print out the table data */
      switch (dimension) {
	case 1:
	  if (i==0) {
	    fprintf(out,"  ,%15.6f ,%15.6f /* Table data */\n",table[i][0],table[i][1]);
	  } else {
	    fprintf(out,"  ,%15.6f ,%15.6f\n",table[i][0],table[i][1]);
	  }
	  break;
	case 2:
	  if (i==0) {
	    fprintf(out,"  ,%15.6f ,%15.6f ,%15.6f /* Table data */\n",table[i][0],table[i][1],table[i][2]);
	  } else {
	    fprintf(out,"  ,%15.6f ,%15.6f ,%15.6f\n",table[i][0],table[i][1],table[i][2]);
	  }
	  break;
	case 3:
	  if (i==0) {
	    fprintf(out,"  ,%15.6f ,%15.6f ,%15.6f ,%15.6f /* Table data */\n",table[i][0],table[i][1],table[i][2],table[i][3]);
	  } else {
	    fprintf(out,"  ,%15.6f ,%15.6f ,%15.6f ,%15.6f\n",table[i][0],table[i][1],table[i][2],table[i][3]);
	  }
	  break;
	case 4:
	  if (i==0) {
	    fprintf(out,"  ,%15.6f ,%15.6f ,%15.6f ,%15.6f ,%15.6f /* Table data */\n",table[i][0],table[i][1],table[i][2],table[i][3],table[i][4]);
	  } else {
	    fprintf(out,"  ,%15.6f ,%15.6f ,%15.6f ,%15.6f ,%15.6f\n",table[i][0],table[i][1],table[i][2],table[i][3],table[i][4]);
	  }
	  break;
      }   /* End of switch */
      ++i;
    } while(i<tab_rows);
    fprintf(out,"  ,0    /* block */\n");
    fprintf(out,"  ,0    /* block0 */\n");
    fprintf(out,"  ,0    /* inrow */\n");

    switch (dimension) {
      case 1:
	fprintf(out,"  ,%d   /* nb */\n",nb[0]);
	fprintf(out,"  ,%d   /* pd */\n",0);
	fprintf(out,"  ,%d   /* param_base */\n",0);
	fprintf(out,"  ,%d   /* func_base */\n",0);
	for (j=0;j<nbmax;++j) {
	  fprintf(out,"  ,%d  /* where */\n",0);
	}
	for (j=0;j<nbmax;++j) {
	  fprintf(out,"  ,%d  /* bsize */\n",bsize[j][0]);
	}
	for (j=0;j<nbmax;++j) {
	  fprintf(out,"  ,%d  /* bstart */\n",bstart[j][0]);
	}
	for (j=0;j<tab_rows;++j) {
	  fprintf(out,"  ,%d  /* blockno */\n",blockno[j][0]);
	}
	break;
      case 2:
	fprintf(out,"  ,%d ,%d  /* nb */\n",nb[0],nb[1]);
	fprintf(out,"  ,%d ,%d  /* pd */\n",0,0);
	fprintf(out,"  ,%d ,%d   /* param_base */\n",0,0);
	fprintf(out,"  ,%d ,%d   /* func_base */\n",0,0);

	for (j=0;j<nbmax;++j) {
	  fprintf(out,"  ,%d ,%d  /* where */\n",bstart[j][0],bstart[j][1]);
	}
	for (j=0;j<nbmax;++j) {
	  fprintf(out,"  ,%d ,%d  /* bsize */\n",bsize[j][0],bsize[j][1]);
	}
	for (j=0;j<nbmax;++j) {
	  fprintf(out,"  ,%d ,%d  /* bstart */\n",bstart[j][0],bstart[j][1]);
	}
	for (j=0;j<tab_rows;++j) {
	  fprintf(out,"  ,%d ,%d  /* blockno */\n",blockno[j][0],blockno[j][1]);
	}
	break;
      case 3:
	fprintf(out,"  ,%d ,%d ,%d  /* nb */\n",nb[0],nb[1],nb[2]);
	fprintf(out,"  ,%d ,%d ,%d  /* pd */\n",0,0,0);
	fprintf(out,"  ,%d ,%d ,%d  /* param_base */\n",0,0,0);
	fprintf(out,"  ,%d ,%d ,%d  /* func_base */\n",0,0,0);

	for (j=0;j<nbmax;++j) {
	  fprintf(out," ,%d ,%d ,%d /* where */\n",bstart[j][0],bstart[j][1],bstart[j][2]);
	}
	for (j=0;j<nbmax;++j) {
	  fprintf(out,"  ,%d ,%d ,%d  /* bsize */\n",bsize[j][0],bsize[j][1],bsize[j][2]);
	}
	for (j=0;j<nbmax;++j) {
	  fprintf(out,"  ,%d ,%d ,%d  /* bstart */\n",bstart[j][0],bstart[j][1],bstart[j][2]);
	}
	for (j=0;j<tab_rows;++j) {
	  fprintf(out,"  ,%d ,%d ,%d  /* blockno */\n",blockno[j][0],blockno[j][1],blockno[j][2]);
	}
	break;
      case 4:
	fprintf(out,"  ,%d ,%d ,%d ,%d  /* nb */\n",nb[0],nb[1],nb[2],nb[3]);
	fprintf(out,"  ,%d ,%d ,%d ,%d  /* pd */\n",0,0,0,0);
	fprintf(out,"  ,%d ,%d ,%d ,%d  /* param_base */\n",0,0,0,0);
	fprintf(out,"  ,%d ,%d ,%d ,%d  /* func_base */\n",0,0,0,0);

	for (j=0;j<nbmax;++j) {
	  fprintf(out," ,%d ,%d ,%d ,%d /* where */\n",bstart[j][0],bstart[j][1],bstart[j][2],bstart[j][3]);
	}
	for (j=0;j<nbmax;++j) {
	  fprintf(out,"  ,%d ,%d ,%d ,%d  /* bsize */\n",bsize[j][0],bsize[j][1],bsize[j][2],bsize[j][3]);
	}
	for (j=0;j<nbmax;++j) {
	  fprintf(out,"  ,%d ,%d ,%d ,%d  /* bstart */\n",bstart[j][0],bstart[j][1],bstart[j][2],bstart[j][3]);
	}
	for (j=0;j<tab_rows;++j) {
	  fprintf(out,"  ,%d ,%d ,%d ,%d /* blockno */\n",blockno[j][0],blockno[j][1],blockno[j][2],blockno[j][3]);
	}
	break;
    } /* End of switch */
    fprintf(out,"};\n");
    fprintf(incout,"#define STRUCTNAME %s\n",structname);
    fprintf(incout,"#define FUNC_NAME %s1\n",varname);
    fprintf(incout,"#define FUNC_LABEL \"%s1\"\n",varname);
    fprintf(incout,"#include \"lip1.c\"\n");
    if (dimension > 1) {
      fprintf(incout,"#define FUNC_NAME_2 %s2\n",varname);
	fprintf(incout,"#define FUNC_LABEL_2 \"%s2\"\n",varname);
      fprintf(incout,"#include \"lip2.c\"\n");
    }
    if (dimension > 2) {
      fprintf(incout,"#define FUNC_NAME_3 %s3\n",varname);
	fprintf(incout,"#define FUNC_LABEL_3 \"%s3\"\n",varname);
      fprintf(incout,"#include \"lip3.c\"\n");
    }
    if (dimension > 3) {
      fprintf(incout,"#define FUNC_NAME_4 %s4\n",varname);
	fprintf(incout,"#define FUNC_LABEL_4 \"%s4\"\n",varname);
      fprintf(incout,"#include \"lip4.c\"\n");
    }
/* Undefine to get ready for next struct */
    fprintf(incout,"#undef STRUCTNAME\n");
    fprintf(incout,"#undef FUNC_NAME\n");
    fprintf(incout,"#undef FUNC_LABEL\n");
    if (dimension > 1) {
      fprintf(incout,"#undef FUNC_NAME_2\n");
      fprintf(incout,"#undef FUNC_LABEL_2\n");
    }
    if (dimension > 2) {
      fprintf(incout,"#undef FUNC_NAME_3\n");
	fprintf(incout,"#undef FUNC_LABEL_3\n");
    }
    if (dimension > 3) {
      fprintf(incout,"#undef FUNC_NAME_4\n");
	fprintf(incout,"#undef FUNC_LABEL_4\n");
    }
/* Time for next table */
  } while (read_result != EOF); /* End of outer loop */
  fclose(out);
  fclose(in);
  fclose(incout);
  printf("Files closed. Number of tables:\n");
  printf("Total:\t%d\t1-dim:\t%d\t2-dim:\t%d\t3-dim:\t%d\n",tables,tables1d,tables2d,tables3d);
  printf("4-dim:\t%d\n",tables4d);
  printf("More than 4-dim:\t%d\tMaximum dimension:\t%d\n",tablesnd, maxdim);
}
