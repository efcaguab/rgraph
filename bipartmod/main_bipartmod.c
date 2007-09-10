#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "prng.h"
#include "tools.h"
#include "graph.h"
#include "modules.h"
#include "bipartite.h"

main(int argc, char **argv)
{
  FILE *outF, *inF;
  int seed = 1111;
  struct binet *binet = NULL;
  struct group *part = NULL;
  struct prng *randGen;
  double Ti, Tf, Ts, fac;
  char file_name[100];
  int invert;

  /*
    ------------------------------------------------------------
    Prompt for user-defined parameters
    ------------------------------------------------------------
  */
  printf("\n# Enter random number seed (POSITIVE integer): ");
  scanf("%d", &seed);

  printf("\n# Enter the name of the network file: ");
  scanf("%s", &file_name);

  printf("\n# Enter iteration factor: ");
  scanf("%lf", &fac);
  
  printf("\n# Enter the cooling factor (>~0 (fast); <~1 (slow)): ");
  scanf("%lf", &Ts);

  printf("\n# Find modules from first column (0) or second columnd (1): ");
  scanf("%d", &invert);

  /*
    ------------------------------------------------------------------
    Initialize the random number generator
    ------------------------------------------------------------------
  */
  randGen = prng_new("mt19937(1111)");
  prng_seed(randGen, seed);

  /*
    ------------------------------------------------------------------
    Build the network
    ------------------------------------------------------------------
  */
  fprintf(stderr, "Here\n");
  inF = fopen(file_name, "r");
  binet = FBuildNetworkBipart(inF, 0, 0);
  fclose(inF);
  fprintf(stderr, "Here\n");
  if (invert == 1)
    InvertBipart(binet);
  fprintf(stderr, "Here\n");

  /*
    ------------------------------------------------------------------
    Find the modules using the bipartite network
    ------------------------------------------------------------------
  */
  Ti = 1. / (double)CountNodes(binet->net1);
  Tf = 0.;

  part = SACommunityIdentBipart(binet,
				Ti, Tf, Ts, fac,
				0, 'o', 1, 'm',
				randGen);
  outF = fopen("modules_bipart.dat", "w");
  FPrintPartition(outF, part, 0);
  fclose(outF);
  RemovePartition(part);

  // Free memory
  // ------------------------------------------------------------
  RemoveBipart(binet);
}