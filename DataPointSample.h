/*
 *  DataPointSample.h
 *  
 *  Created by Mike Fuller.
 *  Copyright 2008. All rights reserved.
 *
 *	Class DataPointSample can be used to track
 *	and manipulate groups of DatumPoint objects. 
 *
 *
 */

#ifndef DATAPOINTSAMPLE_H
#define DATAPOINTSAMPLE_H

#include <vector>

using namespace std;

namespace MMFdata 
{
	
	class DataPointSample 
		{
		public:
			//sampleidentity set to -1
			//numberofpoints set to one
			//initializes points array to size 1
			//sets identity of first point to -1
			//species, value_integer, and value_decimal of first point set to zero
			//x and y coordinates of first point set to zero
			DataPointSample();
			
			//sampleidentity set to sampleID
			//numberofpoints set to size
			//inserts pointdata into datapoints array
			DataPointSample(DatumPoint* pointdata, int sampleID, int size);
			
			//copy constructor
			DataPointSample(const DataPointSample& dspObject); 
			
			//destructor
			~DataPointSample();
			
			//  ACCESSOR FUNCTIONS
			//returns sampleidentity
			int get_sampleidentity() const;
			
			//returns numberofpoints
			int get_numberofpoints() const;
			
			//returns an individual point from point array at index given
			DatumPoint get_point(int index) const;
			
			//inserts contents of points into pointer to vector of DatumPoint objects
			//PRECONDITION: Assumes vector pointscopy is empty and of zero size!
			//if points is empty, does nothing to argument
			void get_allpoints(vector<DatumPoint>& pointscopy);
			
			//inserts into argument the coordinates for all points
			void get_coordinates(double** coords);
			
			//   MUTATOR FUNCTIONS
			//sets the sample identity number
			void set_sampleidentity(const int sampleID);
			
			//No function for setting numberofpoints because that entails changing
			//the internal data arrays. Instead, use assignment operator or copy
			//constructor.
			
			//replaces existing point coordinates with new set
			//coordinates are assigned to points in order of previous assignment
			void set_coordinates(double** coordsreset, int resetsize);
			
			//   OVERLOADED OPERATORS
			DataPointSample& operator =(const DataPointSample& rhs);
			bool operator ==(const DataPointSample& rhs) const;
			
		private:
			int sampleidentity;				//identifies sample; i.e. the "plot number"
			int numberofpoints;				//size of member arrays
			
		protected:
			//points to array of point data
			DatumPoint* datapoints;	
			
			
		};
	
} // namespace MMFdata

#endif  //DATAPOINTSAMPLE_H