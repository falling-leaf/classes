/*write at the front:
 *
 *ATTENTION! please first read the report/chapter 1/previous in the report to know how to use the program
 *
 *this is a test file, with the aim to show the time of different methods to cope with the question
 *you need to input N, and choose the method to solve it
 *the program will output the result and the time used to solve the question
 *the further information can be gotten in the report->chapter 1->previous
 */
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#define range 10
clock_t start, stop;
double duration;

void method1(int N, int** matrix)//the method with O(N^6)
{
    int K, k;
    if (N <= 5)
        K = 100000;
    else if (N <= 10)
        K = 10000;
    else if (N <= 30)
        K = 100;
    else if (N <= 50)
        K = 10;
    else K = 1;
    k = K;
    start = clock();
    int n, m, i, j, x, y, sum, summax;
    summax = sum = 0;
    while (k--)
    {
        for (m = 1; m <= N; m++)//y data of the start point(x, y)
        {
            for (n = 1; n <= N; n++)//x data of the start point(x, y)
            {
                for (y = 0; y < (N - m + 1); y++)//y data of the end point(x, y)
                {
                    for (x = 0; x < (N - n + 1); x++)//x data of the end point(x, y)
                    {
                        sum = 0;
                        for (i = y; (i - y) < m; i++)//calculation of column
                        {
                            for (j = x; (j - x) < n; j++)//calculation of row
                            {
                                sum += matrix[i][j];
                            }
                        }
                        if (sum < 0)
                            sum = 0;
                        if (sum > summax)
                            summax = sum;//renew the result
                    }
                }
            }
        }
    }
    stop = clock();
    duration = ((double)(stop - start) / CLK_TCK);
    printf("the result of method 1 is: %d\n", summax);
    printf("the value of K is: %d\n", K);
    printf("the ticks are:%lf\n", double (stop - start));
    printf("the time used by method 1 is: %lf\n", duration);
}

void method2(int N, int** matrix)//the method with O(n^4)
{
    int K, k;
    if (N <= 5)
        K = 100000;
    else if (N <= 10)
        K = 10000;
    else if (N <= 30)
        K = 1000;
    else if (N <= 50)
        K = 100;
    else if (N <= 80)
        K = 10;
    else if (N <= 100)
        K = 10;
    else K = 1;
    k = K;
    start = clock();
    int n, m, i, j, x, y, sum, summax, counter;
    int *linesave = (int*)malloc(sizeof(int) * N);// use linesave to store the temp result
    while (k--)
    {
        for (n = 0; n < N; n++)//y data of the start point(x, y)
        {
            for (m = 0; m < N; m++)//x data of the start point(x, y)
            {
                for (counter = 0; counter < N; counter++)//initialize the linesave
                    linesave[counter] = 0;
                for (y = n; y < N; y++)//y data of the end point(x, y)
                {
                    counter = 0;
                    sum = 0;
                    for (x = m; x < N; x++)//x data of the end point(x, y)
                    {
                        sum = sum + matrix[x][y] + linesave[counter];//subsequently calculation
                        linesave[counter] += matrix[x][y];//renew the linesave
                        counter++;
                        if (summax < sum)
                            summax = sum;//renew the result
                    }
                }
            }
        }
    }
    stop = clock();
    duration = ((double)(stop - start) / CLK_TCK);
    printf("the result of method 2 is: %d\n", summax);
    printf("the value of K is: %d\n", K);
    printf("the ticks are:%lf\n", double (stop - start));
    printf("the time used by method 2 is: %lf\n", duration);
}

void method3(int N, int** matrix)//the method with O(n^3)
{
    int K, k;
    if (N <= 5)
        K = 1000000;
    else if (N <= 10)
        K = 100000;
    else if (N <= 30)
        K = 10000;
    else if (N <= 50)
        K = 1000;
    else if (N <= 80)
        K = 100;
    else if (N <= 100)
        K = 100;
    else K = 1;
    k = K;
    start = clock();
    int n, m, i, j, line, sum, summax, counter;
    sum = summax = 0;
    int **squaresave = (int**)malloc(sizeof(int*) * N);// use squaresave to store the temp result
    for (line = 0; line < N; line++)
    squaresave[line] = (int*)malloc(sizeof(int) * N * (N + 1) / 2);
    while (k--)
    {
        for (line = 0; line < N; line++)
        {
            counter = 0;
            for (i = 0; i < N; i++)
            {
                sum = 0;
                for (j = i; j < N; j++)
                {
                    sum += matrix[line][j];
                    squaresave[line][counter] = sum;//recored all situations for a line
                    counter++;
                    if (sum > summax)
                        summax = sum;
                }
            }
        }
        for (counter = 0; counter < (N * (N + 1)) / 2; counter++)
        {
            sum = 0;
            for (line = 0; line < N; line++)
            {
                sum += squaresave[line][counter];//consider all the situations for a column
                if (sum > summax)
                    summax = sum;
                else if (sum < 0)
                    sum = 0;
            }
        }
    }   
    stop = clock();
    duration = ((double)(stop - start) / CLK_TCK);
    printf("the result of method 3 is: %d\n", summax);
    printf("the value of K is: %d\n", K);
    printf("the ticks are:%lf\n", double (stop - start));
    printf("the time used by method 3 is: %lf\n", duration);
}

int main()
{
//input the data
    int N, method, mode;
    int i = 0, j = 0;
    srand(time(NULL));//form the random digits
    printf("please input N\n");
    scanf("%d", &N);
    printf("do you want to set a random mode or model mode?(random mode: 1; model mode: 0)");
    scanf("%d", &mode);
    printf("please choose the method: (input \"1\" or \"2\" or \"3\", other inputs will do all functions)");
    scanf("%d", &method);

//initialize the matrix
    //set dynamic space for matrix    
    int **matrix;
    matrix = (int**)malloc(sizeof(int*) * N);
    for (i = 0; i < N; i++)
    {
        matrix[i] = (int*)malloc(sizeof(int) * N);
    }
    //use random digit to test
    if (mode == 1)
    {
        for (i = 0; i < N; i++)
        {
            for (j = 0; j < N; j++)
            {
                matrix[i][j] = rand()%(range * 2)-range;
            }
        }
    }
    else if (mode == 0)
    {
        FILE *fp;
        if (N == 5)//for different choice, read different data files.
            fp = fopen("document\\N=5.txt", "r");
        else if (N == 10)
            fp = fopen("document\\N=10.txt", "r");
        else if (N == 30)
            fp = fopen("document\\N=30.txt", "r");
        else if (N == 50)
            fp = fopen("document\\N=50.txt", "r");
        else if (N == 80)
            fp = fopen("document\\N=80.txt", "r");
        else if (N == 100)
            fp = fopen("document\\N=100.txt", "r");
        else 
        {
            printf("wrong N!doesn't find the model.");//not the existed file
            exit(1);
        }
        if (fp == NULL)
        {
            printf("open file failed.");
            exit(1);
        }
        for (i = 0; i < N; i++)
        {
            for (j = 0; j < N; j++)
            {
                fscanf(fp, "%d", &matrix[i][j]);//read the file
            }
        }
        fclose(fp);
    }
    else 
    {
        printf("wrong model!");//no the existed file
        exit(1);
    }
    //output the matrix
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            printf("%d ", matrix[i][j]);//output the random/test matrix
        }
        printf("\n");
    }

//test start
    if (method == 1)
        method1(N, matrix);
    else if (method == 2)
        method2(N, matrix);
    else if (method == 3)
        method3(N, matrix);
    else
    {
        method1(N, matrix);
        method2(N, matrix);
        method3(N, matrix);
    }
}