//
//  util.cpp
//  Satellites
//
//  Created by Alex Simmons on 23/11/18.
//  Copyright © 2018 Alex Simmons. All rights reserved.
//

#include "util.hpp"
#include <vector>
#include <iostream>
#include <string.h>
#include <ctime>
#include <stddef.h>
#include "pch.h"


#define M_PI = 3.1415926
using namespace std;

#define earthRadiusKm 6371.0

inline bool file_exists(const string &name)
{
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}

// This function converts decimal degrees to radians
double deg2rad(double deg) {
  return (deg * 3.14 / 180);
}

//  This function converts radians to decimal degrees
double rad2deg(double rad) {
  return (rad * 180 / 3.14);
}



void runTLE2(vector<vector<string>>* results, string startTimeStr, string endTimeStr, double lat0, double long0, double inc, double radius) {
    string line1, line2, dt, rowID;
    struct tm tleEpoch, desiredStartEpoch, desiredEndEpoch;
    double startMins, endMins, curr, start, end, dist;
  
}

