// testcpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <boost/date_time/gregorian/gregorian.hpp>
#include "boost/date_time/posix_time/posix_time.hpp"
#include "nlohmann/json.h"
#include <string>
#include <libsgp4/Tle.h>
#include <libsgp4/SGP4.cc>

using namespace std;
namespace bt = boost::posix_time;

vector<vector<double>> RunTle(Tle tle, double start, double end, double inc, bool quiet)
{
	double current = start;
	SGP4 model(tle);
	bool running = true;
	bool first_run = true;
	CoordGeodetic geo;
	vector<vector<double>> results;
	while (running)
	{
		bool error = false;
		Vector position;
		Vector velocity;
		double tsince;

		try
		{
			if (first_run && current != 0.0)
			{
				/*
				 * make sure first run is always as zero
				 */
				tsince = 0.0;
			}
			else
			{
				/*
				 * otherwise run as normal
				 */
				tsince = current;
			}

			Eci eci = model.FindPosition(tsince);

			position = eci.Position();
			geo = eci.ToGeodetic();
			velocity = eci.Velocity();
		}
		catch (SatelliteException& e)
		{
			std::cerr << e.what() << std::endl;
			error = true;
			running = false;
		}
		catch (DecayedException& e)
		{
			std::cerr << e.what() << std::endl;

			position = e.Position();
			velocity = e.Velocity();

			if (!first_run)
			{
				// print out position on first run
				error = true;
			}

			running = false;
		}

		if ((!error) and (quiet == false))
		{
			results.push_back(vector<double> {tsince, geo.latitude, geo.longitude, geo.altitude});
		}

		if ((first_run && current == 0.0) || !first_run)
		{
			if (current == end)
			{
				running = false;
			}
			else if (current + inc > end)
			{
				current = end;
			}
			else
			{
				current += inc;
			}
		}
		first_run = false;
	}
	return(results);
}



int main(int argc, char *argv[])
{
	/* 
	argv[1] Time Start "%Y-%m%d %H:%M:%S" 
	argv[2] Time End "%Y-%m%d %H:%M:%S" 
	argv[3] Epoch "%Y-%m%d %H:%M:%S" 
	argv[4] TLE Line 1
	argv[5] TLE Line2
	*/

	boost::posix_time::ptime t1(boost::posix_time::time_from_string(argv[1]));
	boost::posix_time::ptime t2(boost::posix_time::time_from_string(argv[2]));
	boost::posix_time::ptime e(boost::posix_time::time_from_string(argv[3]));
	std::string line1 = argv[4];
	std::string line2 = argv[5];

	boost::posix_time::time_duration tStart = (t1 - e);
	boost::posix_time::time_duration tEnd = (t2 - e);

	Tle tle("Test", line1, line2);
	vector<vector<double>> results = RunTle(tle, tStart.total_seconds() / 60., tEnd.total_seconds() / 60., 1.0, false);
}

