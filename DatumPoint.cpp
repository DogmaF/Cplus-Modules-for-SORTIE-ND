/*
 *  DatumPoint.cpp
 *  
 *  Created by Mike Fuller on 04/02/08.
 *  Copyright 2008. All rights reserved.
 *
 *	Implementation of class DatumPoint. 
 *
 */

#include <iostream>
#include "DatumPoint.h"
using namespace std;
namespace MMFdata
{
	
	DatumPoint::DatumPoint() : Datum(), X_coordinate(0), Y_coordinate(0)
	{
		//deliberately empty
	}
	
	DatumPoint::DatumPoint(int ID, int specn, int value_int, double value_dec, 
						   double dbln, double X, double Y)
						:	Datum(ID, specn, value_int, value_dec, dbln), X_coordinate(X), 
							Y_coordinate(Y)
	{
		//deliberately empty
	}
	
	DatumPoint::DatumPoint(const DatumPoint& datptObject) : Datum(datptObject),
	X_coordinate(datptObject.X_coordinate), Y_coordinate(datptObject.Y_coordinate)
	{
		//deliberately empty
	}
	
	DatumPoint::~DatumPoint()
	{
		//deliberately empty
	}
	
	double DatumPoint::get_Xcoord() const {
		return X_coordinate;
	}
	double DatumPoint::get_Ycoord() const {
		return Y_coordinate;
	}
	
	void DatumPoint::set_Xcoord(double Xn) {
		X_coordinate = Xn;
	}
	void DatumPoint::set_Ycoord(double Yn) {
		Y_coordinate = Yn;
	}
	
	void DatumPoint::shiftX(const double added_value) {
		X_coordinate += added_value;
	}
	
	void DatumPoint::shiftY(const double added_value) {
		Y_coordinate += added_value;
	}
	
	void DatumPoint::set_XYcoords(double XYdat[2]) {
		X_coordinate = XYdat[0];
		Y_coordinate = XYdat[1];
	}
	
	DatumPoint& DatumPoint::operator =(const DatumPoint& rhs) {
		//check for self assignment
		if(this == &rhs) return *this;
		else {
			Datum::operator =(rhs);
			X_coordinate = rhs.X_coordinate;
			Y_coordinate = rhs.Y_coordinate;
			return *this;
		}
	}
	
	bool DatumPoint::operator ==(const DatumPoint& rhs) const {
		return	(Datum::operator ==(rhs)
				 && (X_coordinate == rhs.X_coordinate)
				 && (Y_coordinate == rhs.Y_coordinate)
				 );
	}
	
} //MMFdata

	
