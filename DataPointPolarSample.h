/*
 *  DataPointPolarSample.v3.h
 *  
 *  Created by Mike Fuller.
 *  Copyright 2008. All rights reserved.
 *
 *	Class DataPointPolarSample can be used to track
 *	and manipulate groups of DatumPointPolar objects. It includes functions
 *	for extracting a quadangular cookie of points . This class was created 
 *	in part to provide methods for constructing randomized sample plots for 
 *	use as a null model in statistical tests. 
 *
 *	MODIFICATIONS
 *	Uses constant MAXDATAPOINTS instead of call to new for ALL member arrays.
 *
 */

#ifndef DATAPOINTPOLARSAMPLE_H
#define DATAPOINTPOLARSAMPLE_H

#include <vector>
#include <string>
#include "DatumPointPolar.h"

using namespace std;

namespace MMFdata
{
	class DataPointPolarSample 
		{
		public:
			//sampleidentity set to -1
			//plotradius set to -1
			//rotationangle set to zero
			DataPointPolarSample();
			
			//sampleidentity set to sampleID
			//plotradius set to rad
			//rotationangle set to zero
			//inserts pointdat into datappoints array
			DataPointPolarSample(vector<DatumPointPolar> pointdat, int sampleID, double rad);
			
			//Copy constructor. Deep copies datappoints
			//DOES NOT COPY quadcoordinate data
			DataPointPolarSample(const DataPointPolarSample& dsppObject); 
			
			//destructor
			~DataPointPolarSample();
			
			//  ACCESSOR FUNCTIONS
			
			//returns sampleidentity
			int get_sampleidentity() const;
			
			//returns numberofpoints
			int get_numberofpoints() const;
			
			//returns plotradius
			double get_plotradius() const;
			
			//returns rotationangle
			double get_rotationangle() const;
			
			//returns identity of DatumPolarPoint held at index of polarpoints vector
			int get_datapt_ID(const int index) const;
			
			//returns species of DatumPointPolar held at index of polarpoints vector
			int get_datapt_species(const int index) const;
			
			//returns value_integer of DatumPointPolar held at index of polarpoints vector
			int get_datapt_valint(const int index) const;
			
			//returns X coordinate of DatumPointPolar held at index of polarpoints vector
			double get_datapt_X(const int index) const;
			
			//returns Y coordinate of DatumPointPolar held at index of polarpoints vector
			double get_datapt_Y(const int index) const;
			
			//returns value_decimal of DatumPointPolar held at index of polarpoints vector
			double get_datapt_valdec(const int index) const;
			
			//returns length of square cookie cut by cutcookie function
			double get_quadlength() const;
			
			//returns number of points in square cookie cut by cutcookie function
			int get_quadsize() const;
			
			//returns identity of DatumPolarPoint held at index of quadpoints vector
			int get_quadpt_ID(const int index) const;
			
			//returns species of DatumPointPolar held at index of quadpoints vector
			int get_quadpt_species(const int index) const;
			
			//returns value_integer of DatumPointPolar held at index of quadpoints vector
			int get_quadpt_valint(const int index) const;
			
			//returns X coordinate of DatumPointPolar held at index of quadpoints vector
			double get_quadpt_X(const int index) const;
			
			//returns Y coordinate of DatumPointPolar held at index of quadpoints vector
			double get_quadpt_Y(const int index) const;
			
			//returns value_decimal of DatumPointPolar held at index of quadpoints vector
			double get_quadpt_valdec(const int index) const;
			
			//returns an individual point from point array at index given
			DatumPointPolar get_point(int index) const;
			
			//inserts into argument the coordinates for all points
			void get_coordinates(vector<DatumPoint>& coords);
			
			//for retrieving individual x and y coordinates of square cookie
			//coordinates returned have been rescaled to unit circle (not > 1)
			//checks for existance of quad coord data
			//if no quad coord data exist, returns -999
			double get_quadpt_xcoord(const int m) const;
			double get_quadpt_ycoord(const int n) const;
		
			//inserts contents of points into array of DatumPoint objects
			//checks for existance of polar point data (i.e. non-empty datappoints)
			//if datappoints is empty, does nothing to argument and prints error message
			void get_allpointspolar(vector<DatumPointPolar>& pointscopy);
				
			//inserts into argument the original angles of points
			//checks for existance of polar point data (i.e. non-empty datappoints)
			//if datappoints is empty, does nothing to argument and prints error message
			void get_angles(vector<double>& angle_vector);
			
			//inserts into argument distance to center values of datappoints
			//checks for existance of polar point data (i.e. non-empty datappoints)
			//if datappoints is empty, does nothing to argument and prints error message
			void get_alld2c(vector <double>& rad_vector);
			
			//get polar points that occur within square plot cut by cutcookie
			void get_quadpoints(vector<DatumPoint>& qpoints_copy);
					
			//   MUTATOR FUNCTIONS
			
			//sets value of sampleidentity to sampleID
			void set_sampleidentity(const int sampleID);
			
			//No function for setting numberofpoints because that requires also 
			//changing the internal data coordinates, angles, and dist2ctr of each point.  
			//Instead, use assignment operator or copy constructor.
			
			//sets value of plotradius to rad_n
			void set_plotradius(const double rad_n);
			
			//sets value of rotationangle to rot_angle
			void set_rotationangle(const double rot_angle);
			
			//point angles are assigned to new values in order of previous point assignment
			//For each point, sets value of angle and changes values of X,Y 
			//coordinates to account for change in angle on unit circle.
			//checks for existance of polar point data (i.e. non-empty datappoints)
			//if datappoints is empty, does nothing to argument and prints error message
			void set_angles(const vector<double> angleinput);
				
			//Replaces existing polar point coordinates with new pairs
			//Coordinates are assigned to datappoints in order of previous assignment
			//NOTE: changes angle and dist2ctr of each point to reflect new polar coordinates
			//Does not change distance to center of points.
			//checks for existance of polar point data (i.e. non-empty datappoints)
			//if datappoints is empty, does nothing to argument and prints error message
			void set_coordinates(vector<DatumPoint> coordsreset);
			
			//uses vector assign to insert values of argument dpp into datappoints.
			//assumes datappoints is empty before assignment
			void set_ppoints(vector<DatumPointPolar> dpp);
		
			//rotates all points on the circular plot by the same degree angle
			//USER prompted for angle
			//checks for existance of polar point data (i.e. non-empty datappoints)
			//if datappoints is empty, does nothing to argument and prints error message
			void rotatecircplot();
				
			//same as rotatecircplot() but passes argument to rotationangle
			//without prompting USER
			void rotatecircplot(const double r_angle);
			
			/**
			 CUTCOOKIE() 
			 This function extracts a square plot from the inside of a circular plot.
			 The square plot is oriented on the same North-South axis as
			 the original circular plot (e.g. not tilted). The corners of
			 the square are set using the sine and cosine of the theta 
			 angles of points on the circumference of a circle. The angles
			 are 45, 135, 225, and 315 degrees (1/4Pi,3/4Pi,5/4Pi,7/4Pi).
			 The coordinates of points located inside the square are placed 
			 in the member array, quadcoordinates. The coordinates of each point are 
			 also shifted so that the resulting quadrat has it's origin at X = 0, Y = 0.
			 Checks for existance of polar point data (i.e. non-empty datappoints)
			 If datappoints is empty, does nothing to data and prints error message
			 **/
			void cutsquarecookie();
			
			//Adds to value of the X coordinate of the quadpoint located at index
			//effectively moving the point to the right or left (if negative value)
			//calls DatumPoint::shiftX(const double added_value) 
			void shift_quadX(const int index, const double amount);
			
			//Adds to value of the Y coordinate of the quadpoint located at index
			//effectively moving the point up or down (if negative value)
			//calls DatumPoint::shiftY(const double added_value)
			void shift_quadY(const int index, const double amount);
			
			//   OVERLOADED OPERATORS
			
			//every member value and member array value of the left hand side (lhs)
			//is reset to the values of the right hand side (rhs)
			//Note: lhs sampleidentity is also set to rhs value
			DataPointPolarSample& operator =(const DataPointPolarSample& rhs);
			
			//Does NOT compare sample identity values
			//Compares all other scalars and vectors
			bool operator ==(const DataPointPolarSample& rhs) const;
			
		private:
			
			//identifies sample; i.e. the "plot number"
			int sampleidentity;	
			
			//length of radius of circular plot
			double plotradius;		
			
			//angle in degrees used to rotate plot
			double rotationangle;	
			
			//the length of the square plot cut using cutcookie()
			double quad_length;
			
			//Holds member data for each polar point object
			vector<DatumPointPolar> datappoints;
			
			//Holds subset of data found in square cookie
			//set by cutcookie function
			//Stored as DatumPoint objects which are more appropriate for square plots
			//and which facilitate long range shifts of coordinates in the plane
			vector<DatumPoint> quadpoints;
			
			//resets x,y values of coordinates to reflect change in point angles
			//calculates x,y values of rotated point coordinates using trig functions 
			void set_rotatedcoordinates();
			
			//calculates length of square cookie plot and sets value of quad_length
			void set_quadlength();
		};
	
}// namespace MMFdata

#endif  //DATAPOINTPOLARSAMPLE_H
