#include <stdio.h>
#include <omp.h>

#ifndef N
#define N (1<<27) //need 1GB in total
#endif

int main()
{
  float *x = new float[N];
  float *y = new float[N];

  for (int i = 0; i < N; i++) {
    x[i] = i;
  }

  #pragma omp target teams distribute parallel for map(to:x[0:N]) map(from:y[0:N])
  for (int i = 0; i < N; i++) {
	  y[i] = x[i];
  }

  bool valid = true;
  for (int i = 0; i < N; i++) {
    if (x[i] != y[i] ) {
      printf("x[%d] is not equal to y[%d]\n", i, i);
	  valid = false;
    }
  }
  if (valid) {
	printf("correct\n");
	return 0;
  } else {
	printf("Incorrect\n");
	return 0;
  }
}

