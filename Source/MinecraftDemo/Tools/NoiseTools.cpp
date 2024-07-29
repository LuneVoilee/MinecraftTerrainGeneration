#include "NoiseTools.h"
#include "MinecraftDemo/MinecraftDemo.h"
#include "MinecraftDemo/MinecraftDemo.h"

float NoiseTools::PerlinNoise2D(float x,float y) {
	
	int xi=(int) (x);
	int yi=(int) (y);
	float xf=x-xi;
	float yf=y-yi;
	xi = xi&255;
	yi = yi&255;
	float xtemp=(xf+1)/2;
	float ytemp=(yf+1)/2;
	float x_fade=fade(xtemp);
	float y_fade=fade(ytemp);

	int aa=p[p[xi]+yi];
	int ab=p[p[xi]+yi+1];
	int ba=p[p[xi+1]+yi];
	int bb=p[p[xi+1]+yi+1];

	
	angle1 = HashTools::DeterministicRandom(0,2 * PI);
	angle2 = HashTools::DeterministicRandom(0,2 * PI);
	
	float X1,X2,Y1;
	X1=lerp(grad(aa,xf,yf),grad(ba,xf-1,yf),x_fade);
	X2=lerp(grad(ab,xf,yf-1),grad(bb,xf-1,yf-1),x_fade);
	Y1=lerp(X1,X2,y_fade);
	return Y1;
}

float NoiseTools::PerlinNoise2D(FVector2D vec) {
	return PerlinNoise2D(vec.X,vec.Y);
}

//二阶bezier曲线 t应[0.0f~1.0f]
FVector2D NoiseTools::bezier2(FVector2D p0,FVector2D p1,FVector2D p2,float t){
	return ((1-t)*(1-t)*p0+t*(1-t)*p1+t*t*p2);
}

//三阶bezier曲线 t应[0.0f~1.0f]
FVector2D NoiseTools::bezier3(FVector2D p0,FVector2D p1,FVector2D p2,FVector2D p3,float t){
	return bezier2(p0,p1,p2,t)*(1-t)+bezier2(p1,p2,p3,t)*t;
}
FVector2D NoiseTools::bezier4(FVector2D p0,FVector2D p1,FVector2D p2,FVector2D p3,FVector2D p4,float t){
	return bezier3(p0,p1,p2,p3,t)*(1-t)+bezier3(p1,p2,p3,p4,t)*t;
}

float NoiseTools::grad(int flag, float x, float y) {
	
	switch (flag&7) {
		
	case 0x0: return +cos(angle1) * x + sin(angle1) * y;
	case 0x1: return -sin(angle1) * x + cos(angle1) * y;
	case 0x2: return -cos(angle1) * x - sin(angle1) * y;
	case 0x3: return +sin(angle1) * x - cos(angle1) * y;
	case 0x4: return +cos(angle2) * x + sin(angle2) * y;
	case 0x5: return -sin(angle2) * x + cos(angle2) * y;
	case 0x6: return -cos(angle2) * x - sin(angle2) * y;
	case 0x7: return +sin(angle2) * x - cos(angle2) * y;
	default: return 0;
	}
}
float NoiseTools::grad(int flag, FVector2d vec) {
	return NoiseTools::grad(flag,vec.X,vec.Y);
}

float NoiseTools::fade(float t) {
	return t * t * t * (t * (t * 6 - 15) + 10);
}

float NoiseTools::lerp(float a, float b, float t) {
	return a+t*(b-a);
}
float NoiseTools::clampGrad(float t) {
	if(t>1.) {
		return 1.;
	}
	else if(t<-1.) {
		return -1.;
	}
	else {
		return t;
	}
}

