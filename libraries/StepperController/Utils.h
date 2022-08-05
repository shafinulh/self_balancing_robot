//Utils.h
#ifndef Utils
#define Utils
#endif

//Utility class to manage miscellaneous functions
class Utils{
public: 
  static double map(double min1, double max1, double min2, double max2, double val);
  static double bound(double val, double min, double max);
};