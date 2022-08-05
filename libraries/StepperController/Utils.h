#ifndef Utils
#define Utils

class Utils{
public: 
  static double map(double min1, double max1, double min2, double max2, double val){
    double percent = (val - min2)/(max2 - min2);
    return min1 + (max1-min1)*percent;
  }
  static double bound(double val, double min, double max){
    if (val > max) return max; 
    if (val < min) return min; 
    return val;
  }
};