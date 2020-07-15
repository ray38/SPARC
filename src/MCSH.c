# include <stdio.h>
# include <stdlib.h>
# include <string.h>
#include <assert.h>
#include <time.h> 
#include <mpi.h>
# include <math.h>
/* BLAS, LAPACK, LAPACKE routines */
#ifdef USE_MKL
    // #define MKL_Complex16 double complex
    #include <mkl.h>
#else
    #include <cblas.h>
#endif

#include "MCSHHelper.h"
#include "MCSH.h"


void MaxwellCartesianSphericalHarmonics(const double *x, const double *y, const double *z, const int l, const char *n, const double rCutoff, double *result, const int size)
{
	double *r = calloc( size, sizeof(double));
	double *x_hat = calloc( size, sizeof(double));
	double *y_hat = calloc( size, sizeof(double));
	double *z_hat = calloc( size, sizeof(double));


	getRArray(x, y, z, r, size);
	divideVector(x, r, x_hat, size);
	divideVector(y, r, y_hat, size);
	divideVector(z, r, z_hat, size);

	// double *uncutResult = malloc( size * sizeof(double));
	// double *result = malloc( size * sizeof(double));

	int i;
	// printf("\n============\n");
	// for (i = 0; i < size; i++)
	// {
	// 	printf("x: %10f \t y: %10f \t z: %10f \t x_hat: %10f \t y_hat: %10f \t z_hat: %10f \t r: %10f\n", x[i],y[i],z[i],x_hat[i],y_hat[i],z_hat[i], r[i]);
	// }

	// for (i = 0; i < size; i++)
	// {
	// 	double r_calc = sqrt(x[i] * x[i] + y[i] * y[i] + z[i] * z[i]);
	// 	printf("x: %10f \t y: %10f \t z: %10f \t r: %10f \t r_calc: %10f \t rCutoff: %10f \t r>rCutoff%d\n", x[i],y[i],z[i], r[i],  r_calc, rCutoff, r[i]>rCutoff);
	// }
	switch (l) 
	{
		case 0:
			//uncutResult = 1;
			// printf(" l = 0\n");
			
			for ( i = 0; i < size; i++)
			{
				result[i] = 1.0;
			}
			break;

		case 1:
			// printf(" l = 1 \t");
			if (strcmp(n, "100") == 0) 
			{
				// printf(" n = 100 \n");
				for ( i = 0; i < size; i++)
				{
					result[i] = x_hat[i];

				}
			} 
			else if (strcmp(n, "010") == 0)
			{
				for ( i = 0; i < size; i++)
				{
					result[i] = y_hat[i];
				}
			}
			else if (strcmp(n, "001") == 0)
			{
				for ( i = 0; i < size; i++)
				{
					result[i] = z_hat[i];
				}
			}
			else
			{
				printf("\nWARNING: n is not valid %s \n", n);
			}
			break;

		case 2:
			if (strcmp(n, "200") == 0) 
			{
				// result = 3.0 * x_hat * x_hat - 1.0;
				double *temp = malloc( size * sizeof(double));
				polyXYZArray(x_hat, y_hat, z_hat, 2, 0, 0, 3.0, temp, size);
				addScalarVector(temp, -1.0, result, size);
				free(temp);
			} 
			else if (strcmp(n, "020") == 0)
			{
				// result = 3.0 * y_hat * y_hat - 1.0;
				double *temp = malloc( size * sizeof(double));
				polyXYZArray(x_hat, y_hat, z_hat, 0, 2, 0, 3.0, temp, size);
				addScalarVector(temp, -1.0, result, size);
				free(temp);
			}
			else if (strcmp(n, "002") == 0)
			{
				// result = 3.0 * z_hat * z_hat - 1.0;
				double *temp = malloc( size * sizeof(double));
				polyXYZArray(x_hat, y_hat, z_hat, 0, 0, 2, 3.0, temp, size);
				addScalarVector(temp, -1.0, result, size);
				free(temp);
			}
			else if (strcmp(n, "110") == 0)
			{
				// result = 3.0 * x_hat * y_hat;
				polyXYZArray(x_hat, y_hat, z_hat, 1, 1, 0, 3.0, result, size);
			}
			else if (strcmp(n, "101") == 0)
			{
				// result = 3.0 * x_hat * z_hat;
				polyXYZArray(x_hat, y_hat, z_hat, 1, 0, 1, 3.0, result, size);
			}
			else if (strcmp(n, "011") == 0)
			{
				// result = 3.0 * y_hat * z_hat;
				polyXYZArray(x_hat, y_hat, z_hat, 0, 1, 1, 3.0, result, size);
			}
			else
			{
				printf("\nWARNING: n is not valid %s \n", n);
			}
			break;

		case 3:
			if (strcmp(n, "300") == 0) 
			{
				// result = 15.0 * x_hat * x_hat * x_hat - 9.0 * x_hat;
				double *temp1 = malloc( size * sizeof(double));
				double *temp2 = malloc( size * sizeof(double));
				polyXYZArray(x_hat, y_hat, z_hat, 3, 0, 0, 15.0, temp1, size);
				polyXYZArray(x_hat, y_hat, z_hat, 1, 0, 0, 9.0, temp2, size);
				subtractVector(temp1, temp2, result, size);
				free(temp1);
				free(temp2);
			} 
			else if (strcmp(n, "030") == 0)
			{
				// result = 15.0 * y_hat * y_hat * y_hat - 9.0 * y_hat;
				double *temp1 = malloc( size * sizeof(double));
				double *temp2 = malloc( size * sizeof(double));
				polyXYZArray(x_hat, y_hat, z_hat, 0, 3, 0, 15.0, temp1, size);
				polyXYZArray(x_hat, y_hat, z_hat, 0, 1, 0, 9.0, temp2, size);
				subtractVector(temp1, temp2, result, size);
				free(temp1);
				free(temp2);
			}
			else if (strcmp(n, "003") == 0)
			{
				// result = 15.0 * z_hat * z_hat * z_hat - 9.0 * z_hat;
				double *temp1 = malloc( size * sizeof(double));
				double *temp2 = malloc( size * sizeof(double));
				polyXYZArray(x_hat, y_hat, z_hat, 0, 0, 3, 15.0, temp1, size);
				polyXYZArray(x_hat, y_hat, z_hat, 0, 0, 1, 9.0, temp2, size);
				subtractVector(temp1, temp2, result, size);
				free(temp1);
				free(temp2);
			}
			else if (strcmp(n, "210") == 0)
			{
				// result = 15.0 * x_hat * x_hat * y_hat - 3.0 * y_hat;
				double *temp1 = malloc( size * sizeof(double));
				double *temp2 = malloc( size * sizeof(double));
				polyXYZArray(x_hat, y_hat, z_hat, 2, 1, 0, 15.0, temp1, size);
				polyXYZArray(x_hat, y_hat, z_hat, 0, 1, 0, 3.0, temp2, size);
				subtractVector(temp1, temp2, result, size);
				free(temp1);
				free(temp2);
			}
			else if (strcmp(n, "120") == 0)
			{
				// result = 15.0 * x_hat * y_hat * y_hat - 3.0 * x_hat;
				double *temp1 = malloc( size * sizeof(double));
				double *temp2 = malloc( size * sizeof(double));
				polyXYZArray(x_hat, y_hat, z_hat, 1, 2, 0, 15.0, temp1, size);
				polyXYZArray(x_hat, y_hat, z_hat, 1, 0, 0, 3.0, temp2, size);
				subtractVector(temp1, temp2, result, size);
				free(temp1);
				free(temp2);
			}
			else if (strcmp(n, "201") == 0)
			{
				// result = 15.0 * x_hat * x_hat * z_hat - 3.0 * z_hat;
				double *temp1 = malloc( size * sizeof(double));
				double *temp2 = malloc( size * sizeof(double));
				polyXYZArray(x_hat, y_hat, z_hat, 2, 0, 1, 15.0, temp1, size);
				polyXYZArray(x_hat, y_hat, z_hat, 0, 0, 1, 3.0, temp2, size);
				subtractVector(temp1, temp2, result, size);
				free(temp1);
				free(temp2);
			}
			else if (strcmp(n, "102") == 0)
			{
				// result = 15.0 * x_hat * z_hat * z_hat - 3.0 * x_hat;
				double *temp1 = malloc( size * sizeof(double));
				double *temp2 = malloc( size * sizeof(double));
				polyXYZArray(x_hat, y_hat, z_hat, 1, 0, 2, 15.0, temp1, size);
				polyXYZArray(x_hat, y_hat, z_hat, 1, 0, 0, 3.0, temp2, size);
				subtractVector(temp1, temp2, result, size);
				free(temp1);
				free(temp2);
			}
			else if (strcmp(n, "021") == 0)
			{
				// result = 15.0 * y_hat * y_hat * z_hat - 3.0 * z_hat;
				double *temp1 = malloc( size * sizeof(double));
				double *temp2 = malloc( size * sizeof(double));
				polyXYZArray(x_hat, y_hat, z_hat, 0, 2, 1, 15.0, temp1, size);
				polyXYZArray(x_hat, y_hat, z_hat, 0, 0, 1, 3.0, temp2, size);
				subtractVector(temp1, temp2, result, size);
				free(temp1);
				free(temp2);
			}
			else if (strcmp(n, "012") == 0)
			{
				// result = 15.0 * y_hat * z_hat * z_hat - 3.0 * y_hat;
				double *temp1 = malloc( size * sizeof(double));
				double *temp2 = malloc( size * sizeof(double));
				polyXYZArray(x_hat, y_hat, z_hat, 0, 1, 2, 15.0, temp1, size);
				polyXYZArray(x_hat, y_hat, z_hat, 0, 1, 0, 3.0, temp2, size);
				subtractVector(temp1, temp2, result, size);
				free(temp1);
				free(temp2);
			}
			else if (strcmp(n, "111") == 0)
			{
				// result = 15.0 * x_hat * y_hat * z_hat;
				polyXYZArray(x_hat, y_hat, z_hat, 1, 1, 1, 15.0, result, size);
			}
			else
			{
				printf("\nWARNING: n is not valid %s \n", n);
			}
			break;

		case 4:
			if (strcmp(n, "400") == 0) 
			{
				// result = 105.0 * x_hat * x_hat * x_hat * x_hat - 90.0 * x_hat * x_hat + 9.0;
				double *temp1 = malloc( size * sizeof(double));
				double *temp2 = malloc( size * sizeof(double));
				double *temp3 = malloc( size * sizeof(double));
				polyXYZArray(x_hat, y_hat, z_hat, 4, 0, 0, 105.0, temp1, size);
				polyXYZArray(x_hat, y_hat, z_hat, 2, 0, 0, 90.0, temp2, size);
				subtractVector(temp1, temp2, temp3, size);
				addScalarVector(temp3, 9.0, result, size);
				free(temp1);
				free(temp2);
				free(temp3);
			} 
			else if (strcmp(n, "040") == 0)
			{
				// result = 105.0 * y_hat * y_hat * y_hat * y_hat - 90.0 * y_hat * y_hat + 9.0;
				double *temp1 = malloc( size * sizeof(double));
				double *temp2 = malloc( size * sizeof(double));
				double *temp3 = malloc( size * sizeof(double));
				polyXYZArray(x_hat, y_hat, z_hat, 0, 4, 0, 105.0, temp1, size);
				polyXYZArray(x_hat, y_hat, z_hat, 0, 2, 0, 90.0, temp2, size);
				subtractVector(temp1, temp2, temp3, size);
				addScalarVector(temp3, 9.0, result, size);
				free(temp1);
				free(temp2);
				free(temp3);
			}
			else if (strcmp(n, "004") == 0)
			{
				// result = 105.0 * z_hat * z_hat * z_hat * z_hat - 90.0 * z_hat * z_hat + 9.0;
				double *temp1 = malloc( size * sizeof(double));
				double *temp2 = malloc( size * sizeof(double));
				double *temp3 = malloc( size * sizeof(double));
				polyXYZArray(x_hat, y_hat, z_hat, 0, 0, 4, 105.0, temp1, size);
				polyXYZArray(x_hat, y_hat, z_hat, 0, 0, 2, 90.0, temp2, size);
				subtractVector(temp1, temp2, temp3, size);
				addScalarVector(temp3, 9.0, result, size);
				free(temp1);
				free(temp2);
				free(temp3);
			}
			else if (strcmp(n, "310") == 0)
			{
				// result = 105.0 * x_hat * x_hat * x_hat * y_hat - 45.0 * x_hat * y_hat;
				double *temp1 = malloc( size * sizeof(double));
				double *temp2 = malloc( size * sizeof(double));
				polyXYZArray(x_hat, y_hat, z_hat, 3, 1, 0, 105.0, temp1, size);
				polyXYZArray(x_hat, y_hat, z_hat, 1, 1, 0, 45.0, temp2, size);
				subtractVector(temp1, temp2, result, size);
				free(temp1);
				free(temp2);
			}
			else if (strcmp(n, "130") == 0)
			{
				// result = 105.0 * x_hat * y_hat * y_hat * y_hat - 45.0 * x_hat * y_hat;
				double *temp1 = malloc( size * sizeof(double));
				double *temp2 = malloc( size * sizeof(double));
				polyXYZArray(x_hat, y_hat, z_hat, 1, 3, 0, 105.0, temp1, size);
				polyXYZArray(x_hat, y_hat, z_hat, 1, 1, 0, 45.0, temp2, size);
				subtractVector(temp1, temp2, result, size);
				free(temp1);
				free(temp2);
			}
			else if (strcmp(n, "301") == 0)
			{
				// result = 105.0 * x_hat * x_hat * x_hat * z_hat - 45.0 * x_hat * z_hat;
				double *temp1 = malloc( size * sizeof(double));
				double *temp2 = malloc( size * sizeof(double));
				polyXYZArray(x_hat, y_hat, z_hat, 3, 0, 1, 105.0, temp1, size);
				polyXYZArray(x_hat, y_hat, z_hat, 1, 0, 1, 45.0, temp2, size);
				subtractVector(temp1, temp2, result, size);
				free(temp1);
				free(temp2);
			}
			else if (strcmp(n, "103") == 0)
			{
				// result = 105.0 * x_hat * z_hat * z_hat * z_hat - 45.0 * x_hat * z_hat;
				double *temp1 = malloc( size * sizeof(double));
				double *temp2 = malloc( size * sizeof(double));
				polyXYZArray(x_hat, y_hat, z_hat, 1, 0, 3, 105.0, temp1, size);
				polyXYZArray(x_hat, y_hat, z_hat, 1, 0, 1, 45.0, temp2, size);
				subtractVector(temp1, temp2, result, size);
				free(temp1);
				free(temp2);
			}
			else if (strcmp(n, "031") == 0)
			{
				// result = 105.0 * y_hat * y_hat * y_hat * z_hat - 45.0 * y_hat * z_hat;
				double *temp1 = malloc( size * sizeof(double));
				double *temp2 = malloc( size * sizeof(double));
				polyXYZArray(x_hat, y_hat, z_hat, 0, 3, 1, 105.0, temp1, size);
				polyXYZArray(x_hat, y_hat, z_hat, 0, 1, 1, 45.0, temp2, size);
				subtractVector(temp1, temp2, result, size);
				free(temp1);
				free(temp2);
			}
			else if (strcmp(n, "013") == 0)
			{
				// result = 105.0 * y_hat * z_hat * z_hat * z_hat - 45.0 * y_hat * z_hat;
				double *temp1 = malloc( size * sizeof(double));
				double *temp2 = malloc( size * sizeof(double));
				polyXYZArray(x_hat, y_hat, z_hat, 0, 1, 3, 105.0, temp1, size);
				polyXYZArray(x_hat, y_hat, z_hat, 0, 1, 1, 45.0, temp2, size);
				subtractVector(temp1, temp2, result, size);
				free(temp1);
				free(temp2);
			}
			else if (strcmp(n, "220") == 0)
			{
				// result = 105.0 * x_hat * x_hat * y_hat * y_hat - 15.0 * x_hat * x_hat - 15.0 * y_hat * y_hat + 3.0;
				double *temp1 = malloc( size * sizeof(double));
				double *temp2 = malloc( size * sizeof(double));
				double *temp3 = malloc( size * sizeof(double));
				double *temp4 = malloc( size * sizeof(double));
				double *temp5 = malloc( size * sizeof(double));
				polyXYZArray(x_hat, y_hat, z_hat, 2, 2, 0, 105.0, temp1, size);
				polyXYZArray(x_hat, y_hat, z_hat, 2, 0, 0, 15.0, temp2, size);
				polyXYZArray(x_hat, y_hat, z_hat, 0, 2, 0, 15.0, temp3, size);
				subtractVector(temp1, temp2, temp4, size);
				subtractVector(temp4, temp3, temp5, size);
				addScalarVector(temp5, 3.0, result, size);
				free(temp1);
				free(temp2);
				free(temp3);
				free(temp4);
				free(temp5);
			}
			else if (strcmp(n, "202") == 0)
			{
				// result = 105.0 * x_hat * x_hat * z_hat * z_hat - 15.0 * x_hat * x_hat - 15.0 * z_hat * z_hat + 3.0;
				double *temp1 = malloc( size * sizeof(double));
				double *temp2 = malloc( size * sizeof(double));
				double *temp3 = malloc( size * sizeof(double));
				double *temp4 = malloc( size * sizeof(double));
				double *temp5 = malloc( size * sizeof(double));
				polyXYZArray(x_hat, y_hat, z_hat, 2, 0, 2, 105.0, temp1, size);
				polyXYZArray(x_hat, y_hat, z_hat, 2, 0, 0, 15.0, temp2, size);
				polyXYZArray(x_hat, y_hat, z_hat, 0, 0, 2, 15.0, temp3, size);
				subtractVector(temp1, temp2, temp4, size);
				subtractVector(temp4, temp3, temp5, size);
				addScalarVector(temp5, 3.0, result, size);
				free(temp1);
				free(temp2);
				free(temp3);
				free(temp4);
				free(temp5);
			}
			else if (strcmp(n, "022") == 0)
			{
				// result = 105.0 * y_hat * y_hat * z_hat * z_hat - 15.0 * y_hat * y_hat - 15.0 * z_hat * z_hat + 3.0;
				double *temp1 = malloc( size * sizeof(double));
				double *temp2 = malloc( size * sizeof(double));
				double *temp3 = malloc( size * sizeof(double));
				double *temp4 = malloc( size * sizeof(double));
				double *temp5 = malloc( size * sizeof(double));
				polyXYZArray(x_hat, y_hat, z_hat, 0, 2, 2, 105.0, temp1, size);
				polyXYZArray(x_hat, y_hat, z_hat, 0, 2, 0, 15.0, temp2, size);
				polyXYZArray(x_hat, y_hat, z_hat, 0, 0, 2, 15.0, temp3, size);
				subtractVector(temp1, temp2, temp4, size);
				subtractVector(temp4, temp3, temp5, size);
				addScalarVector(temp5, 3.0, result, size);
				free(temp1);
				free(temp2);
				free(temp3);
				free(temp4);
				free(temp5);
			}
			else if (strcmp(n, "211") == 0)
			{
				// result = 105.0 * x_hat * x_hat * y_hat * z_hat - 15.0 * y_hat * z_hat;
				double *temp1 = malloc( size * sizeof(double));
				double *temp2 = malloc( size * sizeof(double));
				polyXYZArray(x_hat, y_hat, z_hat, 2, 1, 1, 105.0, temp1, size);
				polyXYZArray(x_hat, y_hat, z_hat, 0, 1, 1, 15.0, temp2, size);
				subtractVector(temp1, temp2, result, size);
				free(temp1);
				free(temp2);
			}
			else if (strcmp(n, "121") == 0)
			{
				// result = 105.0 * x_hat * y_hat * y_hat * z_hat - 15.0 * x_hat * z_hat;
				double *temp1 = malloc( size * sizeof(double));
				double *temp2 = malloc( size * sizeof(double));
				polyXYZArray(x_hat, y_hat, z_hat, 1, 2, 1, 105.0, temp1, size);
				polyXYZArray(x_hat, y_hat, z_hat, 1, 0, 1, 15.0, temp2, size);
				subtractVector(temp1, temp2, result, size);
				free(temp1);
				free(temp2);
			}
			else if (strcmp(n, "112") == 0)
			{
				// result = 105.0 * x_hat * y_hat * z_hat * z_hat - 15.0 * x_hat * y_hat;
				double *temp1 = malloc( size * sizeof(double));
				double *temp2 = malloc( size * sizeof(double));
				polyXYZArray(x_hat, y_hat, z_hat, 1, 1, 2, 105.0, temp1, size);
				polyXYZArray(x_hat, y_hat, z_hat, 1, 1, 0, 15.0, temp2, size);
				subtractVector(temp1, temp2, result, size);
				free(temp1);
				free(temp2);
			}
			else
			{
				printf("\nWARNING: n is not valid %s \n", n);
			}
			break;

		default:
			printf("\nWARNING: l is not valid %d \n", l);
	}

	//int i;
	for (i = 0; i < size; i++)
	{	
		//printf("before: %10f \t",result[i]);
		if (r[i] > rCutoff)
		{
			result[i] = 0.0;
		}
		//printf("%10f \t %10f \n", uncutResult[i], result[i]);
		//printf("after: %10f \n",result[i]);
	}


	//free(uncutResult);
	free(r);
	free(x_hat);
	free(y_hat);
	free(z_hat);

	// return result;
}


void calculateStencil(const int stencilDimX, const int stencilDimY, const int stencilDimZ, const double hx, const double hy, const double hz, 
					  const double rCutoff, const int l, const char *n, const double *U, const int accuracy, double *stencil)
{
	int pixelEvalArrSize = accuracy * accuracy * accuracy;

	double dv = calcDv(hx, hy, hz, accuracy,U);

	double *refX = calloc( pixelEvalArrSize, sizeof(double));
	double *refY = calloc( pixelEvalArrSize, sizeof(double));
	double *refZ = calloc( pixelEvalArrSize, sizeof(double));

	getCentralCoords(hx, hy, hz, accuracy, refX, refY, refZ);

	// printf("after central coords\n");

	// printf("----------X-----------\n");
	// printArr(refX, pixelEvalArrSize);
	// printf("----------Y-----------\n");
	// printArr(refY, pixelEvalArrSize);
	// printf("----------Z-----------\n");
	// printArr(refZ, pixelEvalArrSize);


	int centerX = (stencilDimX - 1)/2;
    int centerY = (stencilDimY - 1)/2;
    int centerZ = (stencilDimZ - 1)/2;

    //double *stencil = calloc( stencilDimX * stencilDimY * stencilDimZ, sizeof(double));

	double *tempXArr = calloc( pixelEvalArrSize, sizeof(double));
	double *tempYArr = calloc( pixelEvalArrSize, sizeof(double));
	double *tempZArr = calloc( pixelEvalArrSize, sizeof(double));
	double *tempMCSHResult = calloc( pixelEvalArrSize, sizeof(double));
	double xOffset, yOffset, zOffset;
	int i, j, k, index = 0, m;
	for (k = 0; k < stencilDimZ; k++){
		for ( j = 0; j < stencilDimY; j++) {
			for ( i = 0; i < stencilDimX; i++) {
				// printf("start %d %d %d\n", i,j,k);
				xOffset = (i-centerX) * hx;
				yOffset = (j-centerY) * hy;
				zOffset = (k-centerZ) * hz;
				//index = k * stencilDimX * stencilDimY + j * stencilDimX + i;


				addScalarVector(refX, xOffset, tempXArr, pixelEvalArrSize);
				addScalarVector(refY, yOffset, tempYArr, pixelEvalArrSize);
				addScalarVector(refZ, zOffset, tempZArr, pixelEvalArrSize);

				// printf("\n\nX_offset: %10f \t Y_offset: %10f \t Z_offset: %10f \n",xOffset,yOffset,zOffset);
				// for (m = 0; m < pixelEvalArrSize; m++)
				// {
				// 	printf("refx: %10f \t refy: %10f \t refz: %10f \t x: %10f \t y: %10f \t z: %10f \n", refX[m],refY[m],refZ[m],tempXArr[m],tempYArr[m],tempZArr[m]);
				// }

				applyU2(tempXArr, tempYArr, tempZArr, U, pixelEvalArrSize);

				// for (m = 0; m < pixelEvalArrSize; m++)
				// {
				// 	printf("refx: %10f \t refy: %10f \t refz: %10f \t Ux: %10f \t Uy: %10f \t Uz: %10f \n", refX[m],refY[m],refZ[m],tempXArr[m],tempYArr[m],tempZArr[m]);
				// }
				

				MaxwellCartesianSphericalHarmonics(tempXArr, tempYArr, tempZArr, l, n, rCutoff, tempMCSHResult, pixelEvalArrSize);

				//stencil[index] = cblas_dasum(pixelEvalArrSize, tempMCSHResult, 1) * dv;
				stencil[index] = sumArr(tempMCSHResult, pixelEvalArrSize) * dv;
				index++;
			}
		}
	}
	free(refX);
	free(refY);
	free(refZ);
	free(tempXArr);
	free(tempYArr);
	free(tempZArr);
	free(tempMCSHResult);
}


void calcStencilAndConvolveAndAddResult(const double *image, const int imageDimX, const int imageDimY, const int imageDimZ, const double hx, const double hy, const double hz, 
										const double rCutoff, const int l, const char *n, const double *U, const int accuracy, double *convolveResult)
{
	double start_t, end_stencil_t, end_convolve_t, end_convolve2_t; 
	//time(&start_t); 
	start_t = MPI_Wtime();

	int pixelEvalArrSize = accuracy * accuracy * accuracy;

	int stencilDimX, stencilDimY, stencilDimZ;
	GetDimensionsPlane(hx, hy, hz, rCutoff, U, &stencilDimX, &stencilDimY, &stencilDimZ);

	double *stencil = calloc( stencilDimX * stencilDimY * stencilDimZ, sizeof(double));
	calculateStencil(stencilDimX, stencilDimY, stencilDimZ, hx, hy, hz, 
					 rCutoff, l, n, U, accuracy, stencil);

	// printf("end stencil calculation \n");
	// time(&end_stencil_t); 
	end_stencil_t  = MPI_Wtime();

	// char DensFilename[128];

	// snprintf(DensFilename, 128, "Sparc_stencil_%f_%d_%s.csv", rCutoff, l,n);
	// FILE *output_fp_stencil = fopen(DensFilename,"w");

	// int i, j, k, index;
	// for (k = 0; k < stencilDimZ; k++){
	// 	// printf("\n\n------------k = %d ------------\n", k);
	// 	for ( j = 0; j < stencilDimY; j++) {
	// 		for ( i = 0; i < stencilDimX; i++) {
	// 			index = k * stencilDimX * stencilDimY + j * stencilDimX + i;
	// 			fprintf(output_fp_stencil,"%d,%d,%d,%.15f\n",i,j,k,stencil[index]);
	// 			// printf("%10.8f\t",stencil[index]);
	// 		}
	// 		// printf("\n");
	// 	}

	// }

	// fclose(output_fp_stencil);



	// int imageSize = imageDimX * imageDimY * imageDimZ;
	// double *convolveResult2 = malloc( imageDimX * imageDimY * imageDimZ * sizeof(double));
	// for ( i = 0; i < imageSize; i++)
	// {
	// 	convolveResult2[i] = 0.0;
	// }


	convolve3(image, stencil, imageDimX, imageDimY, imageDimZ, stencilDimX, stencilDimY, stencilDimZ, convolveResult);
	// printf("end convolve1 \n");
	// time(&end_convolve_t);
	end_convolve_t  = MPI_Wtime();

	printf("\n r: %f \t l: %d \t n: %s \t total_time: %f \t stencil: %f \t convolve: %f \n",rCutoff, l, n,end_convolve_t - start_t, end_stencil_t - start_t, end_convolve_t - end_stencil_t);

	// convolve2(image, stencil, imageDimX, imageDimY, imageDimZ, stencilDimX, stencilDimY, stencilDimZ, convolveResult2);
	// time(&end_convolve2_t); 
	// printf("end convolve2 \n");
	// printf("\n total_time: %ld \t stencil: %ld \t convolve: %ld \t convolve2: %ld \n",end_convolve_t - start_t, end_stencil_t - start_t, end_convolve_t - end_stencil_t, end_convolve2_t - end_convolve_t);

	// for ( i = 0; i < 100; i++)
	// {
	// 	printf("result1: %15f \t result2: %15f \n", convolveResult[i], convolveResult2[i]);
	// }

	free(stencil);
}


void calcStencilAndConvolveAndSave(const double *image, const int imageDimX, const int imageDimY, const int imageDimZ, const double hx, const double hy, const double hz, 
								   const double rCutoff, const int l, const char *n, const double *U, const int accuracy, const int stencilIndex)
{
	int pixelEvalArrSize = accuracy * accuracy * accuracy;

	// double dv = calcDv(hx, hy, hz, accuracy,U);

	int stencilDimX, stencilDimY, stencilDimZ;
	GetDimensionsPlane(hx, hy, hz, rCutoff, U, &stencilDimX, &stencilDimY, &stencilDimZ);

	double *stencil = calloc( stencilDimX * stencilDimY * stencilDimZ, sizeof(double));
	calculateStencil(stencilDimX, stencilDimY, stencilDimZ, hx, hy, hz, 
					 rCutoff, l, n, U, accuracy, stencil);

	

	char stencilFilename[128];

	snprintf(stencilFilename, 128, "%s_%d.csv", "stencil", stencilIndex);
	printf(stencilFilename);

	writeMatToFile(stencilFilename, stencil, stencilDimX, stencilDimY, stencilDimZ);

	// FILE *output_fp_stencil = fopen(DensFilename,"w");
	// if (output_fp_stencil == NULL) {
	// 	printf("\nCannot open file \"%s\"\n",DensFilename);
	// 	die("cant open file");
	// }

	// int i, j, k, index;
	// for (k = 0; k < stencilDimZ; k++){
	// 	// printf("\n\n------------k = %d ------------\n", k);
	// 	for ( j = 0; j < stencilDimY; j++) {
	// 		for ( i = 0; i < stencilDimX; i++) {
	// 			index = k * stencilDimX * stencilDimY + j * stencilDimX + i;
	// 			fprintf(output_fp_stencil,"%d,%d,%d,%22f\n",i,j,k,stencil[index]);
	// 			// printf("%10.8f\t",stencil[index]);
	// 		}
	// 		// printf("\n");
	// 	}

	// }

	// fclose(output_fp_stencil);





	// printf("end calculating stencil\n");

	int imageSize = imageDimX * imageDimY * imageDimZ;
	double *convolveResult = calloc( imageDimX * imageDimY * imageDimZ, sizeof(double));


	convolve3(image, stencil, imageDimX, imageDimY, imageDimZ, stencilDimX, stencilDimY, stencilDimZ, convolveResult);

	char convolveResultFilename[128];
	snprintf(convolveResultFilename, 128, "%s_%d.csv", "convolve_result", stencilIndex);

	writeMatToFile(convolveResultFilename, convolveResult, imageDimX, imageDimY, imageDimZ);

	// FILE *output_fp = fopen(convolveResultFilename,"w");
	// if (output_fp == NULL) {
	// 	printf("\nCannot open file \"%s\"\n",convolveResultFilename);
	// 	die("cant open file");
	// }

	// index = 0;
	// for (k = 0; k < imageDimZ; k++){
	// 	for ( j = 0; j < imageDimY; j++) {
	// 		for ( i = 0; i < imageDimX; i++) {
	// 			// index = k * imageDimX * imageDimY + j * imageDimX + i;
	// 			fprintf(output_fp,"%d,%d,%d,%10f,%10f\n",i,j,k,convolveResult[index],image[index]);
	// 			index ++;
	// 			//printf("write line %d  %d  %d\n", i, j, k, convolveResult);
	// 		}
	// 	}
	// }

	// fclose(output_fp);

	free(convolveResult);
}


void calcStencilAndSave(const double hx, const double hy, const double hz, const double rCutoff, const int l, const char *n, const double *U, const int accuracy, const int stencilIndex)
{	
	int pixelEvalArrSize = accuracy * accuracy * accuracy;

	// double dv = calcDv(hx, hy, hz, accuracy,U);

	int stencilDimX, stencilDimY, stencilDimZ;
	GetDimensionsPlane(hx, hy, hz, rCutoff, U, &stencilDimX, &stencilDimY, &stencilDimZ);

	double *stencil = calloc( stencilDimX * stencilDimY * stencilDimZ, sizeof(double));
	calculateStencil(stencilDimX, stencilDimY, stencilDimZ, hx, hy, hz, 
					 rCutoff, l, n, U, accuracy, stencil);


	// printf("after stencil calculation");
	char stencilFilename[128];
	snprintf(stencilFilename, 128, "%s_%d.csv", "test_output", stencilIndex);
	writeMatToFile(stencilFilename, stencil, stencilDimX, stencilDimY, stencilDimZ);
	// printf(DensFilename);

	// FILE *output_fp = fopen(DensFilename,"w");
	// if (output_fp == NULL) {
	// 	printf("\nCannot open file \"%s\"\n",DensFilename);
	// 	die("cant open file");
	// }

	// int i, j, k, index = 0;
	// for (k = 0; k < stencilDimZ; k++){
	// 	printf("\n\n------------k = %d ------------\n", k);
	// 	for ( j = 0; j < stencilDimY; j++) {
	// 		for ( i = 0; i < stencilDimX; i++) {
	// 			index = k * stencilDimX * stencilDimY + j * stencilDimX + i;
	// 			fprintf(output_fp,"%d,%d,%d,%22f\n",i,j,k,stencil[index]);
	// 			printf("%10.8f\t",stencil[index]);
	// 		}
	// 		printf("\n");
	// 	}
	// }

	// fclose(output_fp);
	free(stencil);
}







void prepareMCSHFeatureAndSave(const double *image, const int imageDimX, const int imageDimY, const int imageDimZ, const double hx, const double hy, const double hz, 
							   const double rCutoff, const int l, const int group, const double *U, const int accuracy)
{	
	int imageSize = imageDimX * imageDimY * imageDimZ;
	double *featureVector = calloc( imageSize, sizeof(double));

	int i, j, k;
	switch (l) 
	{
		case 0:
			if (group == 1){
				calcStencilAndConvolveAndAddResult(image, imageDimX, imageDimY, imageDimZ, hx, hy, hz, rCutoff, l, "000", U, accuracy, featureVector);
			}
			else{
				die("\nERROR: group number is not valid\n");
			}

			break;

		case 1:
			if (group == 1)
			{	
				double *component1 = calloc( imageSize, sizeof(double));
				double *component2 = calloc( imageSize, sizeof(double));
				double *component3 = calloc( imageSize, sizeof(double));

				calcStencilAndConvolveAndAddResult(image, imageDimX, imageDimY, imageDimZ, hx, hy, hz, rCutoff, l, "100", U, accuracy, component1);
				calcStencilAndConvolveAndAddResult(image, imageDimX, imageDimY, imageDimZ, hx, hy, hz, rCutoff, l, "010", U, accuracy, component2);
				calcStencilAndConvolveAndAddResult(image, imageDimX, imageDimY, imageDimZ, hx, hy, hz, rCutoff, l, "001", U, accuracy, component3);

				// printf("\n-------- components before power ------------\n")
				// for ( i = 0; i < imageSize; i++)
				// {
				// 	printf("")
				// }

				powVector(component1, 2, component1, imageSize);
				powVector(component2, 2, component2, imageSize);
				powVector(component3, 2, component3, imageSize);

				addVector(component1, component2, featureVector, imageSize);
				addVector(featureVector, component3, featureVector, imageSize);

				sqrtVector(featureVector, featureVector, imageSize);

				free(component1);
				free(component2);
				free(component3);
			}
			else
			{
				die("\nERROR: group number is not valid\n");
			}

			break;

		case 2:
			if (group == 1)
			{	
				double *component1 = calloc( imageSize, sizeof(double));
				double *component2 = calloc( imageSize, sizeof(double));
				double *component3 = calloc( imageSize, sizeof(double));

				calcStencilAndConvolveAndAddResult(image, imageDimX, imageDimY, imageDimZ, hx, hy, hz, rCutoff, l, "200", U, accuracy, component1);
				calcStencilAndConvolveAndAddResult(image, imageDimX, imageDimY, imageDimZ, hx, hy, hz, rCutoff, l, "020", U, accuracy, component2);
				calcStencilAndConvolveAndAddResult(image, imageDimX, imageDimY, imageDimZ, hx, hy, hz, rCutoff, l, "002", U, accuracy, component3);

				powVector(component1, 2, component1, imageSize);
				powVector(component2, 2, component2, imageSize);
				powVector(component3, 2, component3, imageSize);

				addVector(component1, component2, featureVector, imageSize);
				addVector(featureVector, component3, featureVector, imageSize);

				sqrtVector(featureVector, featureVector, imageSize);

				free(component1);
				free(component2);
				free(component3);
			}
			else if (group == 2)
			{	
				double *component1 = calloc( imageSize, sizeof(double));
				double *component2 = calloc( imageSize, sizeof(double));
				double *component3 = calloc( imageSize, sizeof(double));

				calcStencilAndConvolveAndAddResult(image, imageDimX, imageDimY, imageDimZ, hx, hy, hz, rCutoff, l, "110", U, accuracy, component1);
				calcStencilAndConvolveAndAddResult(image, imageDimX, imageDimY, imageDimZ, hx, hy, hz, rCutoff, l, "101", U, accuracy, component2);
				calcStencilAndConvolveAndAddResult(image, imageDimX, imageDimY, imageDimZ, hx, hy, hz, rCutoff, l, "011", U, accuracy, component3);

				powVector(component1, 2, component1, imageSize);
				powVector(component2, 2, component2, imageSize);
				powVector(component3, 2, component3, imageSize);

				addVector(component1, component2, featureVector, imageSize);
				addVector(featureVector, component3, featureVector, imageSize);

				sqrtVector(featureVector, featureVector, imageSize);

				free(component1);
				free(component2);
				free(component3);
			}
			else
			{
				die("\nERROR: group number is not valid\n");
			}

			break;

		case 3:
			if (group == 1)
			{	
				double *component1 = calloc( imageSize, sizeof(double));
				double *component2 = calloc( imageSize, sizeof(double));
				double *component3 = calloc( imageSize, sizeof(double));

				calcStencilAndConvolveAndAddResult(image, imageDimX, imageDimY, imageDimZ, hx, hy, hz, rCutoff, l, "300", U, accuracy, component1);
				calcStencilAndConvolveAndAddResult(image, imageDimX, imageDimY, imageDimZ, hx, hy, hz, rCutoff, l, "030", U, accuracy, component2);
				calcStencilAndConvolveAndAddResult(image, imageDimX, imageDimY, imageDimZ, hx, hy, hz, rCutoff, l, "003", U, accuracy, component3);

				powVector(component1, 2, component1, imageSize);
				powVector(component2, 2, component2, imageSize);
				powVector(component3, 2, component3, imageSize);

				addVector(component1, component2, featureVector, imageSize);
				addVector(featureVector, component3, featureVector, imageSize);

				sqrtVector(featureVector, featureVector, imageSize);

				free(component1);
				free(component2);
				free(component3);
			}
			else if (group == 2)
			{	
				double *component1 = calloc( imageSize, sizeof(double));
				double *component2 = calloc( imageSize, sizeof(double));
				double *component3 = calloc( imageSize, sizeof(double));
				double *component4 = calloc( imageSize, sizeof(double));
				double *component5 = calloc( imageSize, sizeof(double));
				double *component6 = calloc( imageSize, sizeof(double));

				calcStencilAndConvolveAndAddResult(image, imageDimX, imageDimY, imageDimZ, hx, hy, hz, rCutoff, l, "210", U, accuracy, component1);
				calcStencilAndConvolveAndAddResult(image, imageDimX, imageDimY, imageDimZ, hx, hy, hz, rCutoff, l, "201", U, accuracy, component2);
				calcStencilAndConvolveAndAddResult(image, imageDimX, imageDimY, imageDimZ, hx, hy, hz, rCutoff, l, "021", U, accuracy, component3);
				calcStencilAndConvolveAndAddResult(image, imageDimX, imageDimY, imageDimZ, hx, hy, hz, rCutoff, l, "120", U, accuracy, component4);
				calcStencilAndConvolveAndAddResult(image, imageDimX, imageDimY, imageDimZ, hx, hy, hz, rCutoff, l, "102", U, accuracy, component5);
				calcStencilAndConvolveAndAddResult(image, imageDimX, imageDimY, imageDimZ, hx, hy, hz, rCutoff, l, "012", U, accuracy, component6);

				powVector(component1, 2, component1, imageSize);
				powVector(component2, 2, component2, imageSize);
				powVector(component3, 2, component3, imageSize);
				powVector(component4, 2, component4, imageSize);
				powVector(component5, 2, component5, imageSize);
				powVector(component6, 2, component6, imageSize);

				addVector(component1, component2, featureVector, imageSize);
				addVector(featureVector, component3, featureVector, imageSize);
				addVector(featureVector, component4, featureVector, imageSize);
				addVector(featureVector, component5, featureVector, imageSize);
				addVector(featureVector, component6, featureVector, imageSize);

				sqrtVector(featureVector, featureVector, imageSize);

				free(component1);
				free(component2);
				free(component3);
				free(component4);
				free(component5);
				free(component6);
			}
			else if (group == 3)
			{
				calcStencilAndConvolveAndAddResult(image, imageDimX, imageDimY, imageDimZ, hx, hy, hz, rCutoff, l, "111", U, accuracy, featureVector);
			}
			else
			{
				die("\nERROR: group number is not valid\n");
			}

			break;

		case 4:
			if (group == 1)
			{	
				double *component1 = calloc( imageSize, sizeof(double));
				double *component2 = calloc( imageSize, sizeof(double));
				double *component3 = calloc( imageSize, sizeof(double));

				calcStencilAndConvolveAndAddResult(image, imageDimX, imageDimY, imageDimZ, hx, hy, hz, rCutoff, l, "400", U, accuracy, component1);
				calcStencilAndConvolveAndAddResult(image, imageDimX, imageDimY, imageDimZ, hx, hy, hz, rCutoff, l, "040", U, accuracy, component2);
				calcStencilAndConvolveAndAddResult(image, imageDimX, imageDimY, imageDimZ, hx, hy, hz, rCutoff, l, "004", U, accuracy, component3);

				powVector(component1, 2, component1, imageSize);
				powVector(component2, 2, component2, imageSize);
				powVector(component3, 2, component3, imageSize);

				addVector(component1, component2, featureVector, imageSize);
				addVector(featureVector, component3, featureVector, imageSize);

				sqrtVector(featureVector, featureVector, imageSize);

				free(component1);
				free(component2);
				free(component3);
			}
			else if (group == 2)
			{	
				double *component1 = calloc( imageSize, sizeof(double));
				double *component2 = calloc( imageSize, sizeof(double));
				double *component3 = calloc( imageSize, sizeof(double));
				double *component4 = calloc( imageSize, sizeof(double));
				double *component5 = calloc( imageSize, sizeof(double));
				double *component6 = calloc( imageSize, sizeof(double));

				calcStencilAndConvolveAndAddResult(image, imageDimX, imageDimY, imageDimZ, hx, hy, hz, rCutoff, l, "310", U, accuracy, component1);
				calcStencilAndConvolveAndAddResult(image, imageDimX, imageDimY, imageDimZ, hx, hy, hz, rCutoff, l, "301", U, accuracy, component2);
				calcStencilAndConvolveAndAddResult(image, imageDimX, imageDimY, imageDimZ, hx, hy, hz, rCutoff, l, "031", U, accuracy, component3);
				calcStencilAndConvolveAndAddResult(image, imageDimX, imageDimY, imageDimZ, hx, hy, hz, rCutoff, l, "130", U, accuracy, component4);
				calcStencilAndConvolveAndAddResult(image, imageDimX, imageDimY, imageDimZ, hx, hy, hz, rCutoff, l, "103", U, accuracy, component5);
				calcStencilAndConvolveAndAddResult(image, imageDimX, imageDimY, imageDimZ, hx, hy, hz, rCutoff, l, "013", U, accuracy, component6);

				powVector(component1, 2, component1, imageSize);
				powVector(component2, 2, component2, imageSize);
				powVector(component3, 2, component3, imageSize);
				powVector(component4, 2, component4, imageSize);
				powVector(component5, 2, component5, imageSize);
				powVector(component6, 2, component6, imageSize);

				addVector(component1, component2, featureVector, imageSize);
				addVector(featureVector, component3, featureVector, imageSize);
				addVector(featureVector, component4, featureVector, imageSize);
				addVector(featureVector, component5, featureVector, imageSize);
				addVector(featureVector, component6, featureVector, imageSize);

				sqrtVector(featureVector, featureVector, imageSize);

				free(component1);
				free(component2);
				free(component3);
				free(component4);
				free(component5);
				free(component6);
			}
			else if (group == 3)
			{
				double *component1 = calloc( imageSize, sizeof(double));
				double *component2 = calloc( imageSize, sizeof(double));
				double *component3 = calloc( imageSize, sizeof(double));

				calcStencilAndConvolveAndAddResult(image, imageDimX, imageDimY, imageDimZ, hx, hy, hz, rCutoff, l, "220", U, accuracy, component1);
				calcStencilAndConvolveAndAddResult(image, imageDimX, imageDimY, imageDimZ, hx, hy, hz, rCutoff, l, "202", U, accuracy, component2);
				calcStencilAndConvolveAndAddResult(image, imageDimX, imageDimY, imageDimZ, hx, hy, hz, rCutoff, l, "022", U, accuracy, component3);

				powVector(component1, 2, component1, imageSize);
				powVector(component2, 2, component2, imageSize);
				powVector(component3, 2, component3, imageSize);

				addVector(component1, component2, featureVector, imageSize);
				addVector(featureVector, component3, featureVector, imageSize);

				sqrtVector(featureVector, featureVector, imageSize);

				free(component1);
				free(component2);
				free(component3);
			}
			else if (group == 4)
			{
				double *component1 = calloc( imageSize, sizeof(double));
				double *component2 = calloc( imageSize, sizeof(double));
				double *component3 = calloc( imageSize, sizeof(double));

				calcStencilAndConvolveAndAddResult(image, imageDimX, imageDimY, imageDimZ, hx, hy, hz, rCutoff, l, "211", U, accuracy, component1);
				calcStencilAndConvolveAndAddResult(image, imageDimX, imageDimY, imageDimZ, hx, hy, hz, rCutoff, l, "121", U, accuracy, component2);
				calcStencilAndConvolveAndAddResult(image, imageDimX, imageDimY, imageDimZ, hx, hy, hz, rCutoff, l, "112", U, accuracy, component3);

				powVector(component1, 2, component1, imageSize);
				powVector(component2, 2, component2, imageSize);
				powVector(component3, 2, component3, imageSize);

				addVector(component1, component2, featureVector, imageSize);
				addVector(featureVector, component3, featureVector, imageSize);

				sqrtVector(featureVector, featureVector, imageSize);

				free(component1);
				free(component2);
				free(component3);
			}
			else
			{
				die("\nERROR: group number is not valid\n");
			}

			break;
			

		default:
			die("\nERROR: l is not valid\n");
			break;
	}

	char convolveResultFilename[128];

	snprintf(convolveResultFilename, 128, "%s_%d_%d_%f.csv", "MCSH_feature", l, group, rCutoff);
	// printf(DensFilename);
	writeMatToFile(convolveResultFilename, featureVector, imageDimX, imageDimY, imageDimZ);

	// FILE *output_fp = fopen(DensFilename,"w");
	// if (output_fp == NULL) {
	// 	printf("\nCannot open file \"%s\"\n",DensFilename);
	// 	die("cant open file");
	// }
	// int index;
	// for (k = 0; k < imageDimZ; k++){
	// 	for ( j = 0; j < imageDimY; j++) {
	// 		for ( i = 0; i < imageDimX; i++) {
	// 			index = k * imageDimX * imageDimY + j * imageDimX + i;
	// 			fprintf(output_fp,"%d,%d,%d,%.12f\n",i,j,k,featureVector[index]);
	// 		}
	// 	}
	// }

	// fclose(output_fp);

}


double scoreTask(const double rCutoff, const int l, const int group)
{
	double result;
	switch (l) 
	{
		case 0:
			if (group == 1)
			{
				result = 1;
			}
			else
			{
				printf("\n***** WARNING: group number not valid *****\n");
			}
			break;

		case 1:
			if (group == 1)
			{
				result = 3;
			}
			else
			{
				printf("\n***** WARNING: group number not valid *****\n");
				result = 1;
			}
			break;

		case 2:
			if (group == 1)
			{
				result = 3;
			}
			else if (group == 2)
			{
				result = 3;
			}
			else
			{
				printf("\n***** WARNING: group number not valid *****\n");
				result = 1;
			}
			break;


		case 3:
			if (group == 1)
			{
				result = 3;
			}
			else if (group == 2)
			{
				result = 6;
			}
			else if (group == 3)
			{
				result = 1;
			}
			else
			{
				printf("\n***** WARNING: group number not valid *****\n");
				result = 1;
			}
			break;



		case 4:
			if (group == 1)
			{
				result = 3;
			}
			else if (group == 2)
			{
				result = 6;
			}
			else if (group == 3)
			{
				result = 3;
			}
			else if (group == 4)
			{
				result = 3;
			}
			else
			{
				printf("\n***** WARNING: group number not valid *****\n");
				result = 1;
			}
			break;


		default:
			printf("\n***** WARNING: l not valid *****\n");
			result = 1;
			break;

	}

	result *= rCutoff * rCutoff * rCutoff;
	return result;
}











