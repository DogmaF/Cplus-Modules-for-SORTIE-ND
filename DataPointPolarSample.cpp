/*
 *  DataPointPolarSample.cpp
 *  
 *  Created by Mike Fuller.
 *  Copyright 2008. All rights reserved.
 *
 *	Implementation of class DataPointPolarSample.
 */

#include <iostream>
#include <math.h>
#include <string>
#include <vector>
#include "/Users/KarmaYeshe/Documents/ACADEMIC_PROJECTS/UTOR_Manuscripts_Projects/SORTIE_RELATED_PROJECTS/FULLER_CPP_PROGRAMS/DEVELOPMENT/General_Use_Classes/Classes_Data/DataPointPolarSample.v4.h"

using namespace std;

namespace MMFdata
{
	
	DataPointPolarSample::DataPointPolarSample() : sampleidentity(-1), plotradius(-1), rotationangle(0)
	{
		//deliberately empty
		
	}
	
	DataPointPolarSample::DataPointPolarSample(vector<DatumPointPolar> pointdat, int sampleID, double rad)
	:	sampleidentity(sampleID), plotradius(rad), rotationangle(0)
	{
		vector<DatumPointPolar>::iterator p;
		//Insert data into local vector
		//Uses DatumPoint copy constructor
		for(p = pointdat.begin(); p != pointdat.end(); ++p) {
			datappoints.push_back(*p);
		}
	}
	
	DataPointPolarSample::DataPointPolarSample(const DataPointPolarSample& dsppObject)
	{
		int i;
		vector<DatumPointPolar>::size_type dpp;
				
		sampleidentity =	dsppObject.sampleidentity;
		plotradius =		dsppObject.plotradius;
		rotationangle =		dsppObject.rotationangle;
		
		//deep copy
		//assumes lhs is a completely new object (has no previous data)
		
		//Insert data into member vector
		//Uses DatumPointPolar copy constructor
		if(dsppObject.datappoints.size() > 0) {
			datappoints.assign(dsppObject.datappoints.begin(), dsppObject.datappoints.end());

			if(dsppObject.quadpoints.size() > 0) {
				quadpoints.assign(dsppObject.quadpoints.begin(), dsppObject.quadpoints.end());
			}
		}
	}
	
	DataPointPolarSample::~DataPointPolarSample()
	{
		//deliberately empty
	}
	
	int DataPointPolarSample::get_sampleidentity() const { return sampleidentity; }
	
	int DataPointPolarSample::get_numberofpoints() const { return datappoints.size(); }
	
	double DataPointPolarSample::get_plotradius() const { return plotradius; }
	
	double DataPointPolarSample::get_rotationangle() const { return rotationangle; }
	
	int DataPointPolarSample::get_datapt_ID(const int index) const 
	{
		return quadpoints[index].get_identity();
	}

	int DataPointPolarSample::get_datapt_species(const int index) const
	{
		return datappoints[index].get_species();
	}

	int DataPointPolarSample::get_datapt_valint(const int index) const
	{
		return datappoints[index].get_value_int();
	}

	double DataPointPolarSample::get_datapt_X(const int index) const
	{
		return datappoints[index].get_Xcoord();
	}

	double DataPointPolarSample::get_datapt_Y(const int index) const
	{
		return datappoints[index].get_Ycoord();
	}

	double DataPointPolarSample::get_datapt_valdec(const int index) const
	{
		return datappoints[index].get_value_dec();
	}

	double DataPointPolarSample::get_quadlength() const 
	{
		return quad_length; 
	}
	
	int DataPointPolarSample::get_quadsize() const 
	{
		return quadpoints.size(); 
	}

	int DataPointPolarSample::get_quadpt_ID(const int index) const 
	{
		return quadpoints[index].get_identity();
	}
	
	int DataPointPolarSample::get_quadpt_species(const int index) const
	{
		return quadpoints[index].get_species();
	}
	
	int DataPointPolarSample::get_quadpt_valint(const int index) const
	{
		return quadpoints[index].get_value_int();
	}
	
	double DataPointPolarSample::get_quadpt_X(const int index) const
	{
		return quadpoints[index].get_Xcoord();
	}
	
	double DataPointPolarSample::get_quadpt_Y(const int index) const
	{
		return quadpoints[index].get_Ycoord();
	}
	
	double DataPointPolarSample::get_quadpt_valdec(const int index) const
	{
		return quadpoints[index].get_value_dec();
	}
	
	
	DatumPointPolar DataPointPolarSample::get_point(int index) const { 
		if(datappoints.size() >= index) { return datappoints[index];	}
		else {
			cout << "\n|||||||||||||ERROR: buffer overshoot in call for get_point()\n";
			exit(1);
		}
	}

	void DataPointPolarSample::get_coordinates(vector<DatumPoint>& coords) {
		if(datappoints.size() > 0) {
			vector<DatumPointPolar>::iterator p;
			DatumPoint dp;
			for(p = datappoints.begin(); p != datappoints.end(); ++p) {
				dp.set_Xcoord( p->get_Xcoord() );
				dp.set_Ycoord( p->get_Ycoord() );
				coords.push_back(dp);
			}
		}
		else {
			cout << "\n|||||||||||||ERROR (get_coordinates): No points exist\n";
		}
	}
	
	double DataPointPolarSample::get_quadpt_xcoord(const int m) const {
		if(quadpoints.size() >= m) { return quadpoints[m].get_Xcoord(); }
		else {
			cout << "\n|||||||||||||ERROR: buffer overshoot in call for get_pppQUADxcoord()\n";
			exit(1);
		}
	}
	
	double DataPointPolarSample::get_quadpt_ycoord(const int n) const {
		if(quadpoints.size() >= n) { return quadpoints[n].get_Ycoord(); }
		else {
			cout << "\n|||||||||||||ERROR: buffer overshoot in call for get_pppQUADycoord()\n";
			exit(1);
		}
	}
	
	void DataPointPolarSample::get_allpointspolar(vector<DatumPointPolar>& pointscopy) {
		if(datappoints.size() > 0) {
			vector<DatumPointPolar>::iterator p;
			for(p = datappoints.begin(); p != datappoints.end(); ++p) {
				pointscopy.push_back(*p);
			}
		}
		else {
			cout << "\n|||||||||||||ERROR (get get_allpointspolar): No points exist\n";
		}
	}
		
	void DataPointPolarSample::get_angles(vector<double>& angle_vector) {
		if(datappoints.size() > 0) {
			vector<DatumPointPolar>::iterator p;
			for(p = datappoints.begin(); p != datappoints.end(); ++p) {
				angle_vector.push_back( p->get_angle() );
			}
		}
		else {
			cout << "\n|||||||||||||ERROR (get angles): No points exist\n";
		}
	}
	
	void DataPointPolarSample::get_alld2c(vector <double>& rad_vector) {
		if(datappoints.size() > 0) {
			vector<DatumPointPolar>::iterator p;
			for(p = datappoints.begin(); p != datappoints.end(); ++p) {
				rad_vector.push_back( p->get_dist2ctr() );
			}
		}
		else {
			cout << "\n|||||||||||||ERROR (get alld2c): No points exist\n";
		}
	}
	
	void DataPointPolarSample::get_quadpoints(vector<DatumPoint>& qpoints_copy) {
		if(quadpoints.size() > 0) {
			qpoints_copy.assign(quadpoints.begin(), quadpoints.end()); 
		}
		else {
			cout << "\n|||||||||||||ERROR (get_quadpoints): No points exist\n";
		}
	}

	void DataPointPolarSample::set_sampleidentity(const int sampleID) {
		sampleidentity = sampleID;
	}
	
	void DataPointPolarSample::set_plotradius(const double rad_n)  {
		plotradius = rad_n;
	}
	
	void DataPointPolarSample::set_rotationangle(const double rot_angle) {
		rotationangle = rot_angle;
	}
	
	void DataPointPolarSample::set_quadlength() {
		//value 1.4 = 2(sin(45)), the length of the square in the unit circle
		quad_length = 1.4 * plotradius;
	}
		
	void DataPointPolarSample::set_angles(const vector<double> angleinput) {
		if(datappoints.size() > 0) {
			int i;
			double next;
			if(angleinput.size() == datappoints.size() ) {  
				for(i = 0; i < datappoints.size(); i++) {
					next = angleinput[i];
					//Note: calling set_angle() changes the value of point angle and 
					//changes values of X,Y coordinates to account for change in
					//angle on unit circle
					datappoints[i].set_angle(next);
				}
			}
			else {
				cout << "\nERROR (set point angles): size of ext/int arrays do not match"
				<< "\nPoint angles not set\n";
			}
		}
	}
	
	void DataPointPolarSample::set_coordinates(vector<DatumPoint> coordsreset) {
		if(datappoints.size() > 0) {
			int i;
			double theta;
			double temp[2];
			
			//check for size compatibility
			if(coordsreset.size() == datappoints.size() ) 
			{
				vector<DatumPoint>::iterator p;
				for(p = coordsreset.begin(); p != coordsreset.end(); ++p) {
				
					temp[0] = p->get_Xcoord();
					temp[1] = p->get_Ycoord();
					datappoints[i].set_XYcoords(temp);
					
					//update point angles to reflect new coordinates
					theta = atan2( temp[1], temp[0] ) * 180/PI;
					datappoints[i].set_angle(theta);
				}
			}
			else 
			{
				cout << "\nERROR (set_coordinates): array sizes don't match\n";
			}
		}
	}

	void DataPointPolarSample::set_ppoints(vector<DatumPointPolar> dpp) 
	{
		datappoints.assign(dpp.begin(), dpp.end() );
	}
		
	void DataPointPolarSample::rotatecircplot() {
		if(datappoints.size() > 0) {
			int i,n;
			double next;
			cout << "\nEnter rotation angle: ";
			cin >> rotationangle;
			cout << "Rotation Angle entered: " << rotationangle << endl;
			
			//rotate angles by adding amount rotated
			for(i = 0; i < datappoints.size(); i++) {
				datappoints[i].set_angle(datappoints[i].get_angle() + rotationangle);
			}
			
			//check for angles > 360, reset angle if necessary
			for(i = 0; i < datappoints.size(); i++) {
				next = datappoints[i].get_angle();
				if(next > 360.0) {
					n = next/360;  //deliberate int/double decimal cut-off
					next-= (n * 360.0);
					datappoints[i].set_angle(next);
				}
			}
			
			//Update x,y coordinates to reflect change in angle.
			//NOTE: do not use set_coordinates() here as it resets the angles.
			set_rotatedcoordinates();
			
		}
		else if(datappoints.size() == 0) {
		cout << "\n||||||||||||||ERROR (rotate angles): No points to rotate\n"; }
	}
	
	void DataPointPolarSample::rotatecircplot(const double r_angle) {
		if(datappoints.size() > 0) {
			int i,n;
			double next;
			rotationangle = r_angle;
			
			//rotate angles by adding amount rotated
			for(i = 0; i < datappoints.size(); i++) {
				datappoints[i].set_angle(datappoints[i].get_angle() + rotationangle);
			}
			
			//check for angles > 360, reset angle if necessary
			for(i = 0; i < datappoints.size(); i++) {
				next = datappoints[i].get_angle();
				if(next > 360.0) {
					n = next/360;  //deliberate int/double decimal cut-off
					next-= (n * 360.0);
					datappoints[i].set_angle(next);
				}
			}
			
			//Update x,y coordinates to reflect change in angle.
			//NOTE: do not use set_coordinates() here as it resets the angles.
			set_rotatedcoordinates();
			
		}
		else if(datappoints.size() == 0) {
		cout << "\n||||||||||||||ERROR (rotate angles): No points to rotate\n"; }
	}
	
	void DataPointPolarSample::set_rotatedcoordinates() {
		
		int i;
		double temp[2];
		for(i = 0; i < datappoints.size(); i++) {
			temp[0] = cos( datappoints[i].get_angle() * PI/180);
			temp[1] = sin( datappoints[i].get_angle() * PI/180);
			datappoints[i].set_XYcoords(temp);
		}
		
	}
	
	void DataPointPolarSample::cutsquarecookie() 
	{
		/***
		 Extracts a square plot from the center of a circular plot.
		 The variables mincorner and maxcorner represent the XY coordinates of a 
		 square whose corners are placed at angles 45, 135, 225, and 315 degrees 
		 on the unit circle. The values of mincorner and maxcorner correspond to sin(315)+1, 
		 and cos(315)+1 respectively. Adding 1 to quadpoint coordinates shifts axes of   
		 square to the realm of positive real numbers (as expected for a typical square plot).
		 ***/
		
		//Abort if attempted on plot with no data
		if(datappoints.size() == 0) {
			cout << "\n||||||||||||||ERROR: tried to cut center cookie from empty plot. Plot No.: " 
				<< sampleidentity;
		}
		if( !quadpoints.empty() ) {
			quadpoints.clear();
		}
		
		if(datappoints.size() > 0 && quadpoints.empty() ) 
		{
			int i;
			DatumPoint tempdp;
			vector<DatumPointPolar>::iterator dpp_p;
			vector<DatumPoint>::iterator dp_p;
			
			//cos(45) = 0.707; Here we are rounding the radius to 0.70.
			//This step provides for even-numbered plot dimensions (28x28m for a 20m-radius circle plot)
			const double mincorner = -0.70 * plotradius;  
			const double maxcorner = 0.70 * plotradius;   
			
			//Find all points that lie within square.
			//XY coords of DPP objects are stored as scaled on the unit circle (i.e. <= 1.0)
			//Calling get_Xcoord scales the coords back to empirical values (i.e. can be > 1).
			//The test compares empirical coordinates of each point with the empirical plot corners.
			
			for(dpp_p = datappoints.begin(); dpp_p != datappoints.end(); ++dpp_p) {
				
				//test
				if(	  dpp_p->get_Xcoord() >= mincorner
				   && dpp_p->get_Xcoord() <= maxcorner
				   && dpp_p->get_Ycoord() >= mincorner
				   && dpp_p->get_Ycoord() <= maxcorner )
				{ 
					//call to dpp2dp converts XY values to empirical values
					//calls DP copy constructor.
					tempdp = dpp_p->dpp2dp();
					quadpoints.push_back(tempdp); 
				} 
			}
			
			if(quadpoints.size() > 0) //if any of the points fell within the square
			{
				//shift lower left corner of plot to point 0,0
				//second term adjusts for part of circular plot not included in cookie
				for(dp_p = quadpoints.begin(); dp_p != quadpoints.end(); ++dp_p) {
					dp_p->shiftX( plotradius - ( plotradius * 0.30 ) );
					dp_p->shiftY( plotradius - ( plotradius * 0.30 ) );
				}

				//set length in meters of the square plot representing the cookie
				set_quadlength();
			}
		}
	}
	
	void DataPointPolarSample::shift_quadX(const int index, const double amount)
	{
		//Uses DatumPoint::shiftX(const double added_value)
		if(quadpoints.size() >= index) {
			quadpoints[index].shiftX(amount);
		}
	}
	
	void DataPointPolarSample::shift_quadY(const int index, const double amount)
	{
		//Uses DatumPoint::shiftY(const double added_value)
		if(quadpoints.size() >= index) {
			quadpoints[index].shiftY(amount);
		}
	}
	
	DataPointPolarSample& DataPointPolarSample::operator =(const DataPointPolarSample& rhs) 
	{
		int i,j;
		
		//check for self assignment
		if(this == &rhs) return *this;
		
		else if(datappoints.size() == 0 && rhs.datappoints.size() == 0) {
			sampleidentity = rhs.sampleidentity;
			plotradius = rhs.plotradius;
			rotationangle = rhs.rotationangle;
		}
		
		else if(datappoints.size() == 0 && rhs.datappoints.size() > 0) 
		{
			sampleidentity	=	rhs.sampleidentity;
			plotradius		=	rhs.plotradius;
			rotationangle	=	rhs.rotationangle;
			
			//Copy rhs DPP data over to lhs
			//Using vector assignment operator and DPPS copy constructor
			datappoints = rhs.datappoints;
			
			//alternative assignment approach
			//datappoints.assign(rhs.datappoints.begin();, rhs.datappoints.end());
		
			//copy quadpoints if they exist
			if(rhs.quadpoints.size() > 0) 
			{
				quadpoints = rhs.quadpoints;
			}
		}
		
		else if(datappoints.size() > 0 && rhs.datappoints.size() > 0) 
		{
			sampleidentity	=	rhs.sampleidentity;
			plotradius		=	rhs.plotradius;
			rotationangle	=	rhs.rotationangle;
			
			//Destroy existing lhs data 
			//Copy rhs DPP data over to lhs
			datappoints.clear();
			datappoints = rhs.datappoints;

			//copy over quadcoordinate array if needed
			
			//if both sides have quad data, delete lhs data and copy rhs data
			if(quadpoints.size() > 0 && rhs.quadpoints.size() > 0)
			{
				quadpoints.clear();
				quadpoints = rhs.quadpoints;
			}
		}	
	}
	
	bool DataPointPolarSample::operator ==(const DataPointPolarSample& rhs) const 
	{
		int test, count;
		vector<double> temp1, temp2;
		
		//Deliberately does not compare sampleidentity values.
		
		//First compare scalars
		//use of separate comparisons to reduce comp intensity
		if(datappoints.size() != rhs.datappoints.size()) {
			test = 0;
		}
		else if(plotradius != rhs.plotradius) {
			test = 0;
		}
		else if(rotationangle != rhs.rotationangle) {
			test = 0;
		}
		//if scalars are equal, compare each polar point if data exist
		//uses DPP comparison operator.
		
		//Note 1: if points are equal, their quadpoints will be equal
		//if points are not equal, their quadpoints will also be unequal
		
		//Note 2: if scalars are equal then we have already tested for the condition
		//of unequal data size values as well as the case of no data on either side
		else if(datappoints.size() > 0 && rhs.datappoints.size() > 0)
		{	
			count = 0;
			while(count < datappoints.size()
				  && datappoints[count] == rhs.datappoints[count] )
			{ count++; }
			if(count != datappoints.size()) {
				test = 0;
			}
			else test = 1;
		}
		
		//cout << "\nTEST VALUE DSPP: " << test << endl;
		if(	test == 1 ) return true;
		else return false;
	}		
	
} //namespace MMFdata



