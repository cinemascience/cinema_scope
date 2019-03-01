int main(int argc, char **argv) {
  
  double xstar[p];
  double ystar[neta];
  double ysd[neta];
  double wstar[peta], wvarstar[peta];
  double pointvar;
  double *ysamp = malloc(neta * samp * sizeof(double));

  int i, j, s;
  FILE *infile;
  FILE *outfile;
  FILE *outfileysd;
  FILE *outfilewstar;
  FILE *outfilewvarstar;
  FILE *outfilesamp;
  FILE *outfilepointvar;
  int colchars = 30;
  char instring[p * colchars];
  char outname[p * colchars];
  char *token;
  int good = 1;
  int ctr = 0;
  char ctrc[100]; 
  // Initialize and set up "random" variable
  gsl_rng *r = gsl_rng_alloc(gsl_rng_taus2); // GSL's Taus generator
  gsl_rng_set(r, time(NULL)); // Initialize the GSL generator with time
  
  // Read inputs from a file
  // File should be space delimited with p numbers on each line.
  if((infile = fopen("xstar.dat","r"))==NULL) {
    printf("Cannot find inputs.\n");
    exit(1);
  }
  
  // Save the outputs in file.
  // Each row is the output for the corresponding row in xstar.dat
  if ((outfile = fopen("ystar.dat","w"))==NULL) {
    printf("Cannot open ystar.dat.\n");
    exit(1);
  }
  
  // Save the std deviation of y outputs in file.
  // Each row is the output for the corresponding row in xstar.dat
  if ((outfileysd = fopen("ysd.dat","w"))==NULL) {
    printf("Cannot open ysd.dat.\n");
    exit(1);
  }
  
  if(samp != 0) {
    // Save the sample outputs in file.
    // Each row is the output for one sample of the corresponding row in xstar.dat (in order)
    if ((outfilesamp = fopen("ysamp.dat","w"))==NULL) {
      printf("Cannot open ysamp.dat.\n");
      exit(1);
    }
  }
  
  // Save the expected weight outputs in file.
  // Each row is the output the corresponding row in xstar.dat
  if ((outfilewstar = fopen("wstar.dat","w"))==NULL) {
    printf("Cannot open wstar.dat.\n");
    exit(1);
  }
  
  // Save the variance of weight outputs in file.
  // Each row is the output the corresponding row in xstar.dat
  if ((outfilewvarstar = fopen("wvarstar.dat","w"))==NULL) {
    printf("Cannot open wvarstar.dat.\n");
    exit(1);
  }
  
  // Save the pointvar (covariance matrix sum of diagonals) outputs in file.
  // Each row is the output the corresponding row in xstar.dat
  if ((outfilepointvar = fopen("ypointvar.dat","w"))==NULL) {
    printf("Cannot open ypointvar.dat.\n");
    exit(1);
  }
  
  // Read in the inputs and emulate the results.
  while(good == 1) {
    
    // Read each line
    if(fgets(instring, p * colchars, infile) != NULL) {
      token = strtok(instring, " ");
      
      // Parse each line, which is space delimited
      for(i=0; i<p; i++) {
        xstar[i] = atof(token);
        token = strtok(NULL, " ");
      }
      
      // Get the answers (expectation and samples).
      emu(r, xstar, ystar, ysd, wstar, wvarstar, ysamp); 
      
      // Write the answer (expectation) and standard deviation at each index point
      for(i=0; i<neta; i++) {
        fprintf(outfile, "%f ", ystar[i]);
        fprintf(outfileysd, "%f ", ysd[i]);
      }
      fprintf(outfile, "\n");
      fprintf(outfileysd, "\n");
      
      if(samp != 0) {
        // Write the answer (samples)
        // each row is its own sample, sampled samp times for each row of x
        for(s=0; s<samp; s++) {
          for(i=0; i<neta; i++) {
            fprintf(outfilesamp, "%f ", ysamp[i*samp+s]);
          }
          fprintf(outfilesamp, "\n");
        }
        fprintf(outfilesamp, "\n");
      }
      
      // Write the answer (basis expectation wstar and variance wvarstar)
      for(i=0; i<peta; i++) {
        fprintf(outfilewstar, "%f ", wstar[i]);
        fprintf(outfilewvarstar, "%f ", wvarstar[i]);
      }
      fprintf(outfilewstar, "\n");
      fprintf(outfilewvarstar, "\n");
      
      // Calculate variance point estimate
      pointvar = 0.0;
      for(i=0; i<neta; i++) {
        pointvar += pow(ysd[i], 2);
      }
      // Write the answer (variance estimate)
      fprintf(outfilepointvar, "%f ", pointvar);
      fprintf(outfilepointvar, "\n");
      
      ctr++;
    } else {
      good = 0;
    }
  }
  fclose(infile);
  fclose(outfile);
  fclose(outfileysd);
  if(samp != 0) { fclose(outfilesamp); }
  fclose(outfilewstar);
  fclose(outfilewvarstar);
  fclose(outfilepointvar);
  
  // Free memory
  gsl_rng_free(r);
  free(ysamp);
  
}

#endif
