#include <limits.h>
#include <iostream>
#include <Windows.h>

#include "CBigNumber.h"

using namespace std;

float matrixChainOrder(int *p, int num);
void printOptimalParens(int **s, int i, int j);
void LCSLength(char *X, char *Y, int m, int n);
void printLCS(int **b, char *X, int i, int j);

int **s;
int **b;

int main()
{
	LARGE_INTEGER lv;
	LARGE_INTEGER lv1;
	LARGE_INTEGER lv2;
	double totaltime,secondsPerTick;
	QueryPerformanceFrequency(&lv);
	secondsPerTick = 1000000.0/lv.QuadPart;

	//Generate dimensions of matrices randomly and search for best multiplication order.
	//Write dimensions, solution and time to file.
	if (!(freopen("MAXTRIX_MULTIPLY.txt", "w", stdout)))
		return -1;

	vector<float> total_times;
	int *p = NULL;
	for (int i = 4; i <= 10; i += 2)
	{
		int num = pow(2.0, i)+1;
		p = new int[num];
		for (int j = 0; j < num; j++)
		{
			p[j] = rand()%1024+1;
		}
		QueryPerformanceCounter(&lv1);
		float count = matrixChainOrder(p, num);
		QueryPerformanceCounter(&lv2);
		totaltime = secondsPerTick*(lv2.QuadPart-lv1.QuadPart);
		total_times.push_back(totaltime);

		cout << "multiply order:" << endl;
		printOptimalParens(s, 1, num-1);
		cout << endl;

		cout << "multiply times(*10^4):\t" << count << endl;

		cout << "matrix dimensions:" << endl;
		for (int i = 0; i < num; i++)
			cout << p[i] << '\t';

		cout << '.' << endl;

		delete p;
		for (int i = 0; i < num-1; i++)
		{
			delete s[i];
		}
	}

	//int p[7] = {30, 35, 15, 5, 10, 20, 25};
	//matrixChainOrder(p, 7);
	//printOptimalParens(s, 1, 6);

	cout << "time:" << endl;
	for (vector<float>::iterator time_it = total_times.begin(); time_it != total_times.end(); time_it++)
	{
		cout << 4+2*(time_it-total_times.begin()) << ":\t" << *time_it << endl;
	}
	total_times.clear();

	fclose(stdout);

	//Generate strings randomly and find the longest common substring.
	//Write strings and time to file.
	if (!(freopen("LCS_LENGTH.txt", "w", stdout)))
		return -1;

	char *X = NULL;
	char *Y = NULL;
	for (int i = 4; i <= 10; i += 2)
	{
		int m = pow(2.0, i);
		int n = 0;
		if (i == 4)
			n = pow(2.0, 3);
		else if (i == 6)
			n = pow(2.0, 4);
		else if (i == 8)
			n = pow(2.0, 5);
		else
			n = pow(2.0, 8);

		X = new char[m];
		Y = new char[n];
		for (int j = 0; j < m; j++)
		{
			X[j] = rand()%26+'A';
		}
		for (int j = 0; j < n; j++)
		{
			Y[j] = rand()%26+'A';
		}
		QueryPerformanceCounter(&lv1);
		LCSLength(X, Y, m, n);
		QueryPerformanceCounter(&lv2);
		totaltime = secondsPerTick*(lv2.QuadPart-lv1.QuadPart);
		total_times.push_back(totaltime);
		cout << "LCS:" << endl;
		printLCS(b, X, m-1, n-1);
		cout << endl;
		cout << "original strings:" << endl;
		for (int i = 0; i < m; i++)
			cout << X[i];
		cout << endl;
		for (int j = 0; j < n; j++)
			cout << Y[j];
		delete X;
		delete Y;
		for (int i = 0; i < m+1; i++)
			delete b[i];
	}
	//char X[7] = {'A', 'B', 'C', 'B', 'D', 'A', 'B'};
	//char Y[6] = {'B', 'D', 'C', 'A', 'B', 'A'};
	//LCSLength(X, Y, 7, 6);
	//printLCS(b, X, 6, 5);
	cout << endl;
	cout << "time:" << endl;
	for (vector<float>::iterator time_it = total_times.begin(); time_it != total_times.end(); time_it++)
	{
		cout << *time_it << endl;
	}	
	fclose(stdout);

	return 0;
}

//Solve matrix chain multiplication problem
float matrixChainOrder(int *p, int num)
{
	int n = num-1;
	float **m = new float *[n];
	s = new int *[n];
	for (int i = 0; i < n; i++)
	{
		m[i] = new float[n];
		s[i] = new int[n];
	}

	for (int i = 0; i < n; i++)
		m[i][i] = 0.0;
	for (int l = 2; l <= n; l++)
	{
		for (int i = 1; i <= n-l+1; i++)
		{
			int j = i+l-1;
			m[i-1][j-1] = (float)INT_MAX;
			for (int k = i; k <= j-1; k++)
			{
				float q = m[i-1][k-1]+m[k][j-1]+(float)p[i-1]*p[k]*p[j]/10000;	//Use 10000 as unit
				if (q < m[i-1][j-1])
				{
					m[i-1][j-1] = q;
					s[i-1][j-1] = k;
				}
			}
		}
	}

	return m[0][n-1];
}

//Print solution with proper parenthesis
void printOptimalParens(int **s, int i, int j)
{
	if (i == j)
		cout << 'A' << i;
	else
	{
		cout << '(';
		printOptimalParens(s, i, s[i-1][j-1]);
		printOptimalParens(s, s[i-1][j-1]+1, j);
		cout << ')';
	}
}

//Solve longest common substring problem
void LCSLength(char *X, char *Y, int m, int n)
{
	int **c = new int *[m+1];
	b = new int *[m+1];
	for (int i = 0; i < m+1; i++)
	{
		c[i] = new int[n+1];
		b[i] = new int[n+1];
	}

	for (int i = 1; i < m+1; i++)
		c[i][0] = 0;
	for (int j = 0; j < n+1; j++)
		c[0][j] = 0;

	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (X[i] == Y[j])
			{
				c[i+1][j+1] = c[i][j]+1;
				b[i+1][j+1] = 0;
			}
			else if (c[i][j+1] >= c[i+1][j])
			{
				c[i+1][j+1] = c[i][j+1];
				b[i+1][j+1] = 1;
			}
			else
			{
				c[i+1][j+1] = c[i+1][j];
				b[i+1][j+1] = 2;
			}
		}
	}
}

//Print longest common substring
void printLCS(int **b, char *X, int i, int j)
{
	if (i == -1 || j == -1)
		return;
	if (b[i+1][j+1] == 0)
	{
		printLCS(b, X, i-1, j-1);
		cout << X[i];
	}
	else if (b[i+1][j+1] == 1)
		printLCS(b, X, i-1, j);
	else
		printLCS(b, X, i, j-1);
}
