
#include "string.h"
#include "math.h"
#include "matrix.h"
//#include <rtthread.h>


void cal_elimination(float p[][MAX_SIZE], float *constant_vector, int n)    //消去  
{  
	int i, j, t = 0, r = 0;  
	float temp[20] = {0};  
	float con_temp = 0;  
	float k1,k2 ;
	for(; r < n ; r++)  
	{  
		for(i = r; i < n - 1; i++)       //行坐标  
		{  
			 k1 = -(p[i + 1][r] / p[r][r]);   //计算每次要加的值，对于系数矩阵  
			 k2 = k1 * constant_vector[r];    //计算每次加到常向量上的值  
			for(j = 0; j < n; j++)  
			{  
				temp[j] = p[r][j] * k1;  
			}  
			t = i + 1;  
			for(j = 0; j < n; j++)  
			{  
				p[t][j] += temp[j];  
			}  
			constant_vector[t] += k2;       //计算常向量上的值  
		}  
	}  
}  
void cal_substitution(float p[][MAX_SIZE], float *constant_vector, float *matrix_result, int n)    //回代求解  
{  
	int i, j;    
	float temp;
	matrix_result[n - 1] = constant_vector[n - 1] / p[n - 1][n - 1];  
	for(i = n - 2; i >= 0; i--)  
	{  
		temp = 0;   
		for(j = i + 1; j < n; j++)  
		{  
			temp += p[i][j] * matrix_result[j];  
		}  
		matrix_result[i] = (constant_vector[i] - temp) / p[i][i];  
	}  
}  


int linear_equation_group(int mode, float a[][MAX_SIZE], float *b, int order, float *result)
{
	float (* pt)[MAX_SIZE];

	pt = (void *)a;  
	cal_elimination(a, b, order);    
	cal_substitution(a, b, result, order); 
	
	return 0;
}

