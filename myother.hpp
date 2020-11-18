#ifndef MYOTHER_HPP
#define MYOTHER_HPP
#include<opencv.hpp>
#include "opencv2/core/core.hpp"
#include <algorithm>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <map>
#include <set>
using namespace std;
using namespace cv;
class KeyBox
{
public:
    KeyBox(){}
    void set(vector<DMatch> &matches,vector<KeyPoint>&kpts)
    {
        int tx,ty;
        int minx=100000000,miny=100000000,maxx=0,maxy=0;
        for(auto it =matches.begin();it!=matches.end();it++)
        {
            tx = kpts[it->queryIdx].pt.x;
            ty = kpts[it->queryIdx].pt.y;
            cout<<tx<<" "<<ty<<endl;
            minx = min(minx,tx);
            miny = min(miny,ty);
            maxx = max(maxx,tx);
            maxy = max(maxy,ty);
        }
        x = minx;
        y = miny;
        w = maxx - minx;
        h = maxy - miny;
    }
    KeyBox(vector<DMatch> &matches,vector<KeyPoint>&kpts)
    {
        set(matches,kpts);
    }
    Rect getRect()const
    {
        cout<<"\n"<<x<<" "<<y<<" "<<w<<" "<<h<<" "<<endl;
        return(Rect(x,y,w,h));
    }
    int x;
    int y;
    int w;
    int h;
};

#endif