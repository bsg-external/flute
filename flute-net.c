#include <stdio.h>
#include <stdlib.h>
#include "flute.h"

int main()
{
    int d=0;
    int x[MAXD], y[MAXD];
    Tree flutetree;
    int flutewl;
    
    while (!feof(stdin)) {
        scanf("%d %d\n", &x[d], &y[d]);
        d++;
    }
    readLUT();

    flutetree = flute(d, x, y, ACCURACY);
    printf("# FLUTE wirelength = %d\n", flutetree.length);

    //printtree(flutetree);

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
    //plottree(flutetree);

    
    //flutewl = flute_wl(d, x, y, ACCURACY);
    //printf("FLUTE wirelength (without RSMT construction) = %d\n", flutewl);
}
