/*
 *  DatumPoint.h
 *  
 *  Created by Mike Fuller on 04/02/08.
 *  Copyright 2008. All rights reserved.
 *
 *	PURPOSE:
 *	Base class for creating classes used for handling 
 *	quantitative data.
 *
 */

#ifndef DATUMPOINT_H
#define DATUMPOINT_H
#include "Datum.h"

namespace MMFdata
{
	class DatumPoint : public Datum
		{
		public:
			//identitynumber set to -1
			//species, value_integer, and value_decimal set to zero
			//x and y coordinates set to zero
			DatumPoint(); 
			
			//sets member variables to values of arguments
			DatumPoint(int ID, int specn, int value_int, double value_dec, 
						double dbln, double X, double Y);
			
			//copy constructor
			DatumPoint(const DatumPoint& datptObject);
			
			//destructor 
			virtual ~DatumPoint();
			
			/*******  Accessor Functions *******/
			
			//deliberately not declared virtual because
			//need to redefine using same signature in derived classes
			double get_Xcoord() const;
			
			//deliberately not declared virtual because
			//need to redefine using same signature in derived classes
			double get_Ycoord() const;
			
			/*******  Mutator Functions *******/
			
			//sets value of X_coordinate to Xn
			//deliberately not declared virtual because
			//need to redefine using same signature in derived classes
			void set_Xcoord(double Xn);
			
			//sets value of Y_coordinate to Yn
			//deliberately not declared virtual because
			//need to redefine using same signature in derived classes
			void set_Ycoord(double Yn);
			
			//argument points to 2-element array
			//first element = X; second element = Y
			void set_XYcoords(double XYdat[2]);	
			
			//Adds to value of X coordinate, effectively moving it to the right or left (if negative value)
			void shiftX(const double added_value);
			
			//Adds to value of Y coordinate, effectively moving it up or down (if negative value)
			void shiftY(const double added_value);
			
			/*******  Overloaded Operators *******/
			
			//overloads assignment operator to work with DatumPoint objects
			DatumPoint& operator =(const DatumPoint& rhs);
			
			//overloads comparison operator to work with DatumPoint objects
			bool operator ==(const DatumPoint& rhs) const;
			
		private:
			double X_coordinate;
			double Y_coordinate;
		};
	
} //MMFdata

#endif  //DATUMPOINT_H
