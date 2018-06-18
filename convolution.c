//gcc -g -Wall -fopenmp -o matrix matrix.c

#include<stdio.h>
#include<stdlib.h>
#include<omp.h>

#define PAUSA scanf("%d",&pausa);


int main(int argc,char* argv[])
{
	int thread_count=strtol(argv[1],NULL,10),i,j,p,q,sum,count,s_matrix=2000,s_filter=5,complete_filter=s_filter*s_filter,middle_filter=s_filter/2,core_matrix=s_matrix-middle_filter;//,pausa;

	int **matrix_o = (int **)malloc(s_matrix*sizeof(int*));
	int **matrix_d = (int **)malloc(s_matrix*sizeof(int*));
	int **filter = (int **)malloc(s_filter*sizeof(int*));

	for(i=0;i<s_matrix;i++)
	{
		matrix_o[i]=(int *)malloc(s_matrix*sizeof(int));
		matrix_d[i]=(int *)malloc(s_matrix*sizeof(int));
		for(j=0;j<s_matrix;j++)
			//matrix_o[i][j]=rand()%256;
			//matrix_o[i][j]=j+1;
			matrix_o[i][j]=1;
			//matrix_o[i][j]=1;
	}

//	for(i=0;i<s_matrix;i++){printf("[ ");for(j=0;j<s_matrix;j++){printf(" %05d",matrix_o[i][j]);}printf("]\n");}

	for(i=0;i<s_filter;i++)
	{
		filter[i]=(int *)malloc(s_filter*sizeof(int));
		for(j=0;j<s_filter;j++)
			filter[i][j]=1;
	}

//	for(i=0;i<s_filter;i++){printf("[ ");for(j=0;j<s_filter;j++){printf(" %05d",filter[i][j]);}printf("]\n");}

	double start_time=omp_get_wtime();
#	pragma omp parallel num_threads(thread_count) private(sum,i,j,p,q,count) shared(matrix_d,matrix_o)
	{
	#	pragma omp for schedule(dynamic)
		for(i=middle_filter;i<core_matrix;i++)
		{
			for(j=middle_filter;j<core_matrix;j++)
			{
				sum=0;
				for(p=0;p<s_filter;p++)
				{
					for(q=0;q<s_filter;q++)
					{
						sum+=filter[p][q]*matrix_o[i-middle_filter+p][j-middle_filter+q];
					}
				}
				matrix_d[i][j]=sum/complete_filter;
			}
		}

		//printf("Elapsed time with %d threads is %f\n",thread_count,omp_get_wtime()-start_time);

		//#	pragma omp parallel for num_threads(thread_count) private(sum,i,j,p,q,count) shared(matrix_d,matrix_o) schedule(dynamic)

	#	pragma omp for schedule(dynamic)
		for(i=0;i<s_matrix;i++)
			for(j=0;j<s_matrix;j++)
			{
				//if(i<middle_filter || i>=core_matrix || (i>=middle_filter && i<core_matrix && (j<middle_filter || j>=core_matrix)))
				if(i<middle_filter || i>=core_matrix || j<middle_filter || j>=core_matrix)
				{
					//printf("\n%d: Probando para i=%d,j=%d",omp_get_thread_num(),i,j);//PAUSA
					sum=0,count=0;
					for(p=0;p<s_filter;p++)
						for(q=0;q<s_filter;q++)
						{
							//printf("\n\t%d: Probando para i=%d,j=%d, p=%d, q=%d",omp_get_thread_num(),i,j,p,q);PAUSA
							if((i-middle_filter+p>=0 && i-middle_filter+p<s_matrix) && (j-middle_filter+q>=0 && j-middle_filter+q<s_matrix))
							{
								//printf("\n\t%d: Calculando para i=%d, j=%d, p=%d, q=%d",omp_get_thread_num(),i,j,p,q);PAUSA
								count++;
								sum+=filter[p][q]*matrix_o[i-middle_filter+p][j-middle_filter+q];
							}
						}
					matrix_d[i][j]=sum/count;
				}
			}
	}

	printf("Elapsed time with %d threads is %f\n",thread_count,omp_get_wtime()-start_time);

	//for(i=0;i<s_matrix;i++){printf("[ ");for(j=0;j<s_matrix;j++){printf(" %05d",matrix_d[i][j]);}printf("]\n");}
	return 0;
}
