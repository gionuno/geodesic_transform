#include <algorithm>
#include <iostream>
#include <vector>
#include <deque>
#include <cmath>

using namespace std;

typedef pair<int,int> point;

double clamp(double x,double a,double b)
{
	return x < a? a: (x > b? b:x);
}

vector<vector<double> > dist_trans_cpp(vector<vector<vector<double> > > & im, const vector<vector<double>> & sd, double alpha)
{
	int    di[8] = { -1, -1,  0,  1,  1,  1,  0, -1};
	int    dj[8] = {  0, -1, -1, -1,  0,  1,  1,  1};
	double df[8] = {1.0,2.0,1.0,2.0,1.0,2.0,1.0,2.0};
	
	int I = im.size();
	int J = im[0].size();
	int C = im[0][0].size();
	
	vector<double> im_min(C, 0.0);
	vector<double> im_max(C, 0.0);
	for(int c=0;c<C;c++)
		im_min[c] = im_max[c] = im[0][0][c];

	double F_max = -1e10;
	
	double tmp  = alpha/(C*(1.0-alpha));
	double a2 = tmp*tmp;
	 
	for(int i=0;i<I;i++)
	for(int j=0;j<J;j++)
	{
		for(int d=0;d<8;d++)
		{
			int k = i+di[d];
			int l = j+dj[d];
			if(k < 0||l < 0||I <= k||J <= l) continue;
			
			double F = 0.0;
			for(int c=0;c<C;c++)
				F += (im[i][j][c]-im[k][l][c])*(im[i][j][c]-im[k][l][c]);
			F = sqrt(df[d]+a2*F);
			
			F_max = F_max < F ? F : F_max;
		}
	}
	
	int B = 2048;
	double F_del = (F_max+1e-10)/B;
	
	vector<deque<point>> Q(B);
	
	vector<vector<double> > D(I,vector<double>(J,1e10));
	vector<vector<   int> > P(I,vector<   int>(J,  -1));
	vector<vector<   int> > A(I,vector<   int>(J,  -1));

	//far -1
	//narrow 0
	//done 1
	
	int    b0 =   0;
	int    sQ =   0;	
	
	for(int i=0;i<I;i++)
	for(int j=0;j<J;j++)
	{
		if(sd[i][j]==0.0) continue;

		D[i][j] = 0.0;
		P[i][j] =   0;
		A[i][j]++;

		Q[0].push_back(point(i,j));
		sQ++;
	}
	
	while(sQ>0)
	{
		while(Q[b0].empty()) b0 = (1+b0)%B;
		
		point p = Q[b0].front();
	
		int i = p.first;
		int j = p.second;
		
		A[i][j]++;
		
		Q[b0].pop_front();
		sQ--;
		
		if(A[i][j]==1)
		for(int d=0;d<8;d++)
		{			
			int k = i+di[d];
			int l = j+dj[d];
			
			if(k < 0||l < 0||I <= k||J <= l) continue;
			if(A[k][l] == 1) continue;
			
			double F = 0.0;
			for(int c=0;c<C;c++)
				F += (im[i][j][c]-im[k][l][c])*(im[i][j][c]-im[k][l][c]);
			F = sqrt(df[d]+a2*F);
			
			if(D[k][l] > D[i][j] + F)
			{
				if(A[k][l] == -1)
				{
					A[k][l]++;
					D[k][l] = D[i][j] + F;
					
					int b = ((int)clamp(F/F_del,0.0,B-1.0)+b0+B)%B;

					P[k][l] = b;
					
					Q[b].push_back(point(k,l));
					sQ++;
				}
				else
				{
					int b = P[k][l];
					
					deque<point>::iterator it = Q[b].begin();
					for(;it != Q[b].end();it++)
						if((*it).first == k && (*it).second == l)
							break;
					if(it != Q[b].end())
						Q[b].erase(it);
					
					D[k][l] = D[i][j] + F;
					
					b = ((int)clamp(F/F_del,0.0,B-1.0)+b0+B)%B;
					P[k][l] = b;
					
					Q[b].push_back(point(k,l));
				}
			}
		}
	}
	return D;
}
