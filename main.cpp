#include "mykeypoints.hpp"
#include "myother.hpp"
#include "opencv2/opencv_modules.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "xfeatures2d\nonfree.hpp"
#include "opencv2/imgproc.hpp"

#include <stdio.h>
#include <vector>
#include <iostream>
#include <map>
using namespace cv;
using namespace xfeatures2d;
using namespace std;
struct node
	{
		//int x;
		int y;
		int value;
		node(int y1,int v):y(y1),value(v){}
	};
static double countxy( map<int,vector<node> > &xyMap,int x,int y,int w,int h);

int main( int argc, char** argv )
{ 
	Mat img_1 = imread( "baby.jpg", CV_LOAD_IMAGE_GRAYSCALE );
	Mat img_2 = imread( "tmp2.jpg", CV_LOAD_IMAGE_GRAYSCALE );
	//if( !img_1.data || !img_2.data )
	//{ printf(" --(!) Error reading images \n"); return -1; }
	
	//-- Step 1: Detect the keypoints using SURF Detector
	int minHessian = 400;
	typedef SURF Feature;
	auto detector = Feature::create(minHessian);
	
	std::vector<KeyPoint> keypoints_1, keypoints_2;
	
	detector->detect( img_1, keypoints_1 );
	detector->detect( img_2, keypoints_2 );
	
	//-- Step 2: Calculate descriptors (feature vectors)
	auto extractor =  Feature::create();
	
	Mat descriptors_1, descriptors_2;

	extractor->compute( img_1, keypoints_1, descriptors_1 );
	extractor->compute( img_2, keypoints_2, descriptors_2 );
  // cout<<descriptors_1;
  /* auto t = keypoints_1[0];
   for(int i=0;i<keypoints_1.size();i++)
   {
      t = keypoints_1[i];
      cout<<i+1<<":"<<"pt:"<<t.pt<<"angle:"<<t.angle<<" "<<"id:"<<t.class_id<<" "<<"octave:"<<t.octave<<" "<<"reps:"<<t.response<<" "<<"size:"<<t.size<<endl;
   }*/

	//-- Step 3: Matching descriptor vectors using FLANN matcher
	FlannBasedMatcher matcher;
//	Ptr<DescriptorMatcher> pmatcher = DescriptorMatcher::create("BruteForce");
	std::vector< DMatch > matches;
	matcher.match( descriptors_1, descriptors_2, matches );
	
	double max_dist = 0; double min_dist = 100;
	
	//-- Quick calculation of max and min distances between keypoints
	for( int i = 0; i < descriptors_1.rows; i++ )
	//for( int i = 0; i <matches.size(); i++ )
	{ 
		double dist = matches[i].distance;
		if( dist < min_dist ) min_dist = dist;
		if( dist > max_dist ) max_dist = dist;
	}

	//cout<<matches.size()<<" "<<descriptors_1.rows<<" "<<descriptors_1.cols<<endl;
	printf("-- Max dist : %f \n", max_dist );
	printf("-- Min dist : %f \n", min_dist );
	
	//-- Draw only "good" matches (i.e. whose distance is less than 2*min_dist,
	//-- or a small arbitary value ( 0.02 ) in the event that min_dist is very
	//-- small)
	//-- PS.- radiusMatch can also be used here.
	std::vector< DMatch > good_matches;
	
	//map<int,vector<node> > xyMap;
	MyKeyPoints mkps;
	int num=0;
	for( int i = 0; i < descriptors_1.rows; i++ )
	{ 
		if( matches[i].distance <= max(2*min_dist, 0.02) )
		{ 
			//mkps.push(keypoints_1[matches[i].queryIdx]);
			//num++;
			good_matches.push_back( matches[i]); 
			/*auto kp = keypoints_1[matches[i].queryIdx];
			auto temp =kp.pt;
			cout<<"x:y"<<temp.x<<" "<<temp.y<<endl;
			auto it = xyMap.find(temp.x);
			if(it==xyMap.end())
			{
				auto v = vector<node>();
				v.push_back(node(temp.y,matches[i].distance));
				xyMap.insert(make_pair(temp.x,v));
			}
			else
			{
				it->second.push_back(node(temp.y,kp.response));
			}
			*/
		}
	}
	//cout<<"good:"<<num<<endl;
	/*for(auto it = xyMap.begin();it!=xyMap.end();it++)
	{
		cout<<"x"<<it->first<<":";
		for(auto it2 = it->second.begin();it2!=it->second.end();it2++)
		{
			cout<<it2->y<<" ";
		}
		cout<<endl;
	}*/
	int tmpwidth = img_2.cols;
	int tmpheight = img_2.rows;
	int imgWidth = img_1.cols;
	int imgHeight = img_1.rows;

	/*int xstart=0, ystart=0;
	int xend=imgWidth-tmpwidth,yend = imgHeight-tmpheight;
	int xstep=4,ystep=4;
	double maxv = 0.0,tempv = 0.0;
	int bestx,besty;
	int x;
	for(int y = ystart; y <yend; y += ystep){

	//	cout<<"y"<<y<<endl;
		for(x = xstart; x < xend;x += xstep)
		{
			tempv= mkps.search(x,y,tmpwidth,tmpheight);
			//cout<<"max"<<ntemp<<endl;
			if(tempv>maxv)
			{
				//cout<<"max"<<tempv<<endl;
				maxv = tempv;
				bestx = x;
				besty = y;
			}
		}
	}//endfor
	*/
	//cout<<"x:"<<bestx<<"y:"<<besty<<endl;
	
	//Rect rec(bestx,besty,tmpwidth,tmpwidth);
	KeyBox kb(good_matches,keypoints_1);
	Mat mat_src = imread("baby.jpg", CV_LOAD_IMAGE_GRAYSCALE );
	rectangle(mat_src,kb.getRect(), Scalar(255, 0, 0),1, LINE_8,0);
	imshow("image", mat_src);
	waitKey(0);
	system("pause");
	//for( int i = 0; i < (int)good_matches.size(); i++ )
	//{ printf( "-- Good Match [%d] Keypoint 1: %d  -- Keypoint 2: %d  \n", i, good_matches[i].queryIdx, good_matches[i].trainIdx ); }
	return 0;
}
static double countxy(map<int,vector<node> > &xyMap,int x,int y,int w,int h)
{
	double ret =0.0;
	map<int,vector<node> >::iterator it;
	for(int i = x;i<=x+w;i++)
	{
		it = xyMap.find(x);
		if(it!=xyMap.end())
		{
			for(auto it2 = it->second.begin();it2!= it->second.end();it2++)
			{
				if(it2->y >= y&&it2->y <= y+h)ret+=it2->value;
			}
		}
	}
	return ret;
}