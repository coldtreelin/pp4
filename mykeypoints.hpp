#ifndef _MY_KEY_POINTS_
#define _MY_KEY_POINTS_

#include "opencv2/core/core.hpp"
#include <stdio.h>
#include <vector>
#include <iostream>
#include <map>
#include <set>
using namespace cv;
//using namespace xfeatures2d;
using namespace std;

KeyPoint a;

class MyKeyPoints
{
public:
MyKeyPoints();
void push(KeyPoint &k);
double search(int x,int y,int w,int h);
map<float,KeyPoint> data;
};
MyKeyPoints::MyKeyPoints(){}
void MyKeyPoints::push(KeyPoint &kp)
{
    data.insert(make_pair(kp.pt.x,kp));
}
double MyKeyPoints::search(int x,int y,int w,int h)
{
    set<float> rps;
	for(auto it = data.begin();it!=data.end();it++)
	{
		if(it->first>=x && it->first<=x+w )
		{
		    if(it->second.pt.y>=y && it->second.pt.y<= y+h)
            {
                rps.insert(it->second.response);
            }
		}
	}
    const int maxsz = 25;
    const int minsz = 10;
    int len = rps.size();

    cout<<"size:"<<rps.size()<<endl;
    if(len>maxsz) len = maxsz;
    else if(len <= minsz) len = 0;
    double sum=0,i =0;
    for(auto it = rps.begin();i<len;it++,i++)
    {
        sum+=*it;
    }
    sum/=double(len+1);
    cout<<"sum:"<<sum<<endl;
	return sum ;
}

#endif