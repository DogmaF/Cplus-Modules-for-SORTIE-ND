/*
 *  DatumPointPolar.cpp
 *  
 *  Created by Mike Fuller on 04/02/08.
 *  Copyright 2008. All rights reserved.
 *
 *	Implementation of class DatumPointPolar. 
 *
 */

#include "/Users/KarmaYeshe/Documents/ACADEMIC_PROJECTS/UTOR_Manuscripts_Projects/SORTIE_RELATED_PROJECTS/FULLER_CPP_PROGRAMS/DEVELOPMENT/General_Use_Classes/Classes_Datum/DatumPointPolar.h"
#include <iostream>
#include <math.h>
#include <cmath>
#include <vector>

using namespace std;

namespace MMFdata 
{
	
	DatumPointPolar::DatumPointPolar() : DatumPoint(),  dist2ctr(1), angle(0) 
	{
		//default setting of X differs from ancestral class, where X = 0
		DatumPoint::set_Xcoord(1.0); 
		DatumPoint::set_Ycoord(0.0); 
	}
	
	DatumPointPolar::DatumPointPolar(int ID, int spp, int value_int, double value_dec, 
									 double dbl, double X, double Y)
									:	DatumPoint(ID, spp, value_int, value_dec, dbl, X, Y)
	{	
		//rescale values of coordinates and set angle using formulas
		calc_dist2ctr_fromcoords();
		scale_XY_toUnitCircle();
		calc_angle_fromcoords();
	}
	
	DatumPointPolar::DatumPointPolar(const DatumPointPolar& dppObject) 
									:	DatumPoint(dppObject), dist2ctr(dppObject.dist2ctr),
										angle(dppObject.angle)
	{
		//deliberately empty
	}									

	DatumPointPolar::~DatumPointPolar() {
		//deliberately empty
	}

	double DatumPointPolar::get_Xcoord() const {
		//Because the DPP constructors rescale the passed values of X and Y  to the unit circle, 
		//the function DatumPoint::get_Xcoord() used here returns value of X for the unit circle.
		//Therefore X is < 1 and is a percentage of the dist2ctr, which is equal to 1.
		//Multiplying X by empirical dist2ctr transforms it to the empirical value of X.

		return DatumPoint::get_Xcoord() * dist2ctr;
	}
	
	double DatumPointPolar::get_Ycoord() const {
		//Because the DPP constructors rescale the passed values of X and Y  to the unit circle, 
		//the function DatumPoint::get_Ycoord() used here returns value of Y for the unit circle.
		//Therefore Y is < 1 and is a percentage of the dist2ctr, which is equal to 1.
		//Multiplying Y by empirical dist2ctr transforms it to the empirical value of Y.
		
		return DatumPoint::get_Ycoord() * dist2ctr;
	}
	
	vector<double> DatumPointPolar::get_XYcoords() {
		//The DPP constructors rescale the values of X and Y that are passed to 
		//DatumPoint::set_Xcoordinate (set_Ycoordinate) to the unit circle.
		//Therefore, in this context, the functions DatumPoint::get_Xcoord() and 
		//DatumPoint::get_Ycoord() return the DPP stored values of X and Y, which 
		//are for the unit circle. Therefore when called here, X and Y are each < 1 
		//and are each a percentage of the dist2ctr, which is 1 for the unit circle. 
		//Multiplying X and Y by dist2ctr transforms them to their empirical values
		
		vector<double> coords;
		double temp1 = DatumPoint::get_Xcoord() * dist2ctr;
		double temp2 = DatumPoint::get_Ycoord() * dist2ctr;
		coords.push_back(temp1);
		coords.push_back(temp2);
		return coords;
	}

	double DatumPointPolar::get_dist2ctr() const {
		//returns empirical length of dist2ctr
		return dist2ctr;
	}
	double DatumPointPolar::get_angle() const {
		//returns value between 0 and 360 degrees
		return angle;
	}

	void DatumPointPolar::set_XYcoords(double XYdat[2]) {
		DatumPoint::set_Xcoord(XYdat[0]);
		DatumPoint::set_Ycoord(XYdat[1]);
		calc_dist2ctr_fromcoords();
		scale_XY_toUnitCircle();
		calc_angle_fromcoords();
	}
	
	void DatumPointPolar::set_dist2ctr(const double distance) {
		dist2ctr = distance;
	}
	 
	void DatumPointPolar::set_angle(double anglen) {
		double newX, newY;
		angle = anglen;
		
		//reset coordinates to new location given by angle
		newX = cos(angle * PI/180);		//set based upon unit circle
		newY = sin(angle * PI/180);		//set based upon unit circle
		DatumPoint::set_Xcoord(newX);	//sets coordinate without division by dist2ctr
		DatumPoint::set_Ycoord(newY);	//sets coordinate without division by dist2ctr
		
		//NOTE: no change made to dist2ctr of point
	}
	
	void DatumPointPolar::shift_dppX(const double added_value) {
		double temp = DatumPoint::get_Xcoord();
		if(temp + added_value/dist2ctr > 1.0) {
			cout << "\nWARNING: increasing DPP X coordinate to a value > 1.0 yields undefined behavior!";
		}
		DatumPoint::set_Xcoord(temp + added_value/dist2ctr);
	}
	
	void DatumPointPolar::shift_dppY(const double added_value) {
		double temp = DatumPoint::get_Ycoord();
		if(temp + added_value/dist2ctr > 1.0) {
			cout << "\nWARNING: increasing DPP Y coordinate to a value > 1.0 yields undefined behavior!";
		}
		DatumPoint::set_Ycoord(temp + added_value/dist2ctr);
	}
 
	DatumPoint DatumPointPolar::dpp2dp() {
		int ident, spn, intn;
		double decn, dbln, Xn, Yn;
 
		ident = Datum::get_identity();
		spn = Datum::get_species();
		intn = Datum::get_value_int();
		decn = Datum::get_value_dec();
		dbln = Datum::get_dbl_n();
		
		//DPP rescales XY to unit circle prior to setting object 
		//member values using DatumPoint::set_Xcoord() (and set_Ycoord() ).
		//To convert to DP object, we multiply by dist2ctr to rescale them back 
		//to their original empirical values
		Xn = DatumPoint::get_Xcoord() * dist2ctr;
		Yn = DatumPoint::get_Ycoord() * dist2ctr;
 
		//initialize DatumPoint object
		DatumPoint dp(ident, spn, intn, decn, dbln, Xn, Yn);
 
		return dp;
	}
 
	void DatumPointPolar::calc_angle_fromcoords() {
		double x = DatumPoint::get_Xcoord();
		double y = DatumPoint::get_Ycoord();
		if(x == 0 && y == 0) angle = 0;
		else angle = atan2(y, x) * 180/PI;  //use 180/PI to convert from radians to degrees
	}
	
	void DatumPointPolar::calc_dist2ctr_fromcoords() {
		double x = DatumPoint::get_Xcoord();
		double y = DatumPoint::get_Ycoord();
		if(x == 0 && y == 0) dist2ctr = 0;
		else dist2ctr = sqrt( (x * x) + (y * y) );
	}
	
	void DatumPointPolar::scale_XY_toUnitCircle() {
		double tempX = DatumPoint::get_Xcoord();
		double tempY = DatumPoint::get_Ycoord();
		
		//dist2ctr = radius of the (inner) circle on which point lies.
		//dist2ctr is usually less than the radius of the empirical plot.
		//dividing by dist2ctr scales the point to a radius of 1.0
		
		if(dist2ctr == 0) {
			DatumPoint::set_Xcoord(0);
			DatumPoint::set_Ycoord(0);
		}
		else {
			DatumPoint::set_Xcoord(tempX/dist2ctr);
			DatumPoint::set_Ycoord(tempY/dist2ctr);
		}
	}
	
	DatumPointPolar& DatumPointPolar::operator =(const DatumPointPolar& rhs) {
		//check for self assignment
		if(this == &rhs) return *this;
		else {
			double tempx, tempy;
			//set identity, species, int_value, dec_value
			//deliberate use of Datum class 
			Datum::operator =(rhs);     
			
			//set x value for unit circle
			//deliberate use of DatumPoint class functions
			
			tempx = rhs.DatumPoint::get_Xcoord();
			DatumPoint::set_Xcoord(tempx);  	
			
			//set y value for unit circle
			//deliberate use of DatumPoint class functions
			tempy = rhs.DatumPoint::get_Ycoord();
			DatumPoint::set_Ycoord(tempy); 
			
			//set dist2ctr and angle
			dist2ctr = rhs.dist2ctr;
			angle = rhs.angle;
			return *this;
		}
	}
	
	bool DatumPointPolar::operator ==(const DatumPointPolar& rhs) const {
		return	(Datum::operator ==(rhs)
				 && (get_Xcoord() == rhs.get_Xcoord())
				 && (get_Ycoord() == rhs.get_Ycoord())
				 && (dist2ctr == rhs.dist2ctr)
				 && (angle == rhs.angle)
				 );
	}
	
} //MMFdata

	
	