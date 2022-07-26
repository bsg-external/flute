#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "flute.h"
#include <assert.h>

int main(int argc, char *argv[])
{
  if (argc != 4) {
    printf("Usage: flute-wcap <cpp_x_nm> <cell_height_y_nm> <fF per um>\n");
    exit(-1);
  }

  int x_nm,y_nm;
  double fF;

  int debug_parse = 0;
  
  assert(sscanf(argv[1],"%d",&x_nm)==1);
  assert(sscanf(argv[2],"%d",&y_nm)==1);
  assert(sscanf(argv[3],"%lf",&fF)==1);

  //printf("cpp=%dnm,height=%dnm,fF/um=%f\n",x_nm,y_nm,fF);

  // we parse files of the format:
  // # wcap ADDER_24_Z: ( 27 120 ) -> ( 21 127 ) ( 21 131 ) ( 23 136 ) 0.000239988
  // corresponding to a wire name, a source node, and dest nodes
  // the program does steiner routes on them to determine a total route distance in nm
  // and a capacitance number is outputted
  // the digits after the parens are ignored
  
    int d=0;
    int x[MAXD], y[MAXD];
    Tree flutetree;
    int flutewl;
    
#define BUF_LENGTH 1000
    char buf[BUF_LENGTH];

    char name[100];

    readLUT();
    
    while (fgets(buf,BUF_LENGTH,stdin) != NULL) {
      if (buf[0] == '\n')
	continue;

      d = 0;
      char *pch = strtok(buf,"# ");

      // skip line that does not start with
      if (strcmp(pch,"wcap") != 0)
	  continue;
	  
      pch = strtok(NULL," :(");
      if (debug_parse) printf("x%sx\n",pch);
      strcpy(name,pch);
      pch = strtok(NULL," :(");
      if (debug_parse) printf("#%s#\n",pch);
      sscanf(pch,"%d",&y[d]);
      pch = strtok(NULL," :)");
      if (debug_parse) printf("!%s!\n",pch);
      sscanf(pch,"%d",&x[d]);      
      d++;

      pch = strtok(NULL,")-> ");
      if (debug_parse) printf("*%s*\n",pch);
      pch = strtok(NULL,"( ");
      while (pch != NULL)
	{
	  if (debug_parse) printf("-%s-\n",pch);
	  assert(sscanf(pch,"%d",&y[d])==1);	  

	  pch = strtok(NULL," ");
	  if (debug_parse) printf(".%s.\n",pch);
	  
	  assert(sscanf(pch,"%d",&x[d])==1);	  
	  d++;
	  pch = strtok(NULL,") ");
	  if (debug_parse) printf("@%s@\n",pch);
	  if (*pch != '(')
	    break;
	  pch = strtok(NULL," ");	  
	  if (debug_parse) printf(",%s,\n",pch);
	}
      
      for (int i = 0; i < d; i++)
	{
	  y[i] *= y_nm;
	  x[i] *= x_nm;
	  //	  printf("%d %d\n",x[i],y[i]);
	}
	
      flutetree = flute(d, x, y, ACCURACY);

      printf("%s %f\n",name, /* flutetree.length,*/ ((double) flutetree.length) / 1000.0 * fF );
    }

    // python graph
    int debug = 0;
    if (debug)
      {
	plottree_matplotlib(flutetree);
	printf("plt.scatter([");
	for (int i = 0; i < d; i++)
	  {
	    if (i != 0) printf(",");
	    printf("%d",x[i]);
	  }
	printf("],[");
	for (int i = 0; i < d; i++)
	  {
	if (i != 0) printf(",");
	printf("%d",y[i]);
	  }
	printf("]);");

	printf("plt.show()\n");
      }
    
    //flutewl = flute_wl(d, x, y, ACCURACY);
    //printf("FLUTE wirelength (without RSMT construction) = %d\n", flutewl);
}
