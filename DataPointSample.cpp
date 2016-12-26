/*
 *  DataPointSample.cpp
 *  
 *  Created by Mike Fuller.
 *  Copyright 2008. All rights reserved.
 *
 *	Implementation of class DataPointSample.
 */
#include <iostream>
#include <math.h>
#include <vector>
#include "/Users/KarmaYeshe/Documents/ACADEMIC_PROJECTS/UTOR_Manuscripts_Projects/SORTIE_RELATED_PROJECTS/FULLER_CPP_PROGRAMS/DEVELOPMENT/General_Use_Classes/Classes_Data/DataPointSample.h"

using std::vector;
using namespace std;

namespace MMFdata
{
	
	DataPointSample::DataPointSample() : sampleidentity(-1), numberofpoints(1)
	{
		//initialize array so it can be compared with overloaded comparison operator
		datapoints = new DatumPoint[1];
		datapoints[0].set_identity(-1); //negative one signifies empty object;
		//useful for comparisons
	}
	
	DataPointSample::DataPointSample(DatumPoint* pointdata, int sampleID, int size)
	:	sampleidentity(sampleID), numberofpoints(size)
	{
		int i;
		datapoints = new DatumPoint[numberofpoints];
		
		//Uses DatumPoint overloaded assignment operator
		for(i = 0; i < numberofpoints; i++) {
			datapoints[i] = pointdata[i];
		}
	}
	
	DataPointSample::DataPointSample(const DataPointSample& dspObject)
	{
		int i;
		sampleidentity =	dspObject.sampleidentity;
		numberofpoints =	dspObject.numberofpoints;
		datapoints =		new DatumPoint[numberofpoints];
		
		//deep copy
		//Uses DatumPoint overloaded assignment operator
		for(i = 0; i < numberofpoints; i++) {
			datapoints[i] = dspObject.datapoints[i];
		}
	}
	
	DataPointSample::~DataPointSample()
	{
		delete [] datapoints;
	}
	
	int DataPointSample::get_sampleidentity() const {
		return sampleidentity;
	}
	
	int DataPointSample::get_numberofpoints() const {
		return numberofpoints;
	}
	
	DatumPoint DataPointSample::get_point(int index) const {
		return datapoints[index];	
	}
	
	void DataPointSample::get_allpoints(vector<DatumPoint>& pointscopy)  
	{
		int i;
		pointscopy.clear();
		//do nothing if there are no points
		if(datapoints[1].get_identity() != -1)
		{
			//Uses DatumPoint overloaded assignment operator
			for(i = 0; i < numberofpoints; i++) 
			{
				pointscopy[i] = datapoints[i];
			}
		}
	}
	
	void DataPointSample::get_coordinates(double** coords) {
		int i;
		for(i = 0; i < numberofpoints; i++) {
			coords[i][0] = datapoints[i].get_Xcoord();
		}
		for(i = 0; i < numberofpoints; i++) {
			coords[i][1] = datapoints[i].get_Ycoord();
		}
	}
	
	void DataPointSample::set_sampleidentity(const int sampleID) {
		sampleidentity = sampleID;
	}
	
	void DataPointSample::set_coordinates(double** coordsreset, int resetsize) {
		int i,j;
		double temp[2];
		//check for size compatibility
		if(resetsize == numberofpoints) {
			for(i = 0; i < numberofpoints; i++) {
				for(j = 0; j < numberofpoints; j++) {
					temp[0] = coordsreset[i][0];
					temp[1] = coordsreset[i][1];
					datapoints[i].set_XYcoords(temp);
				}
			}
		}
		else {
			cout << "\nERROR (set_coordinates): array sizes don't match\n";
		}
	}
	
	DataPointSample& DataPointSample::operator =(const DataPointSample& rhs) 
	{
		int i,j;
		
		//check for self assignment
		if(this == &rhs) return *this;
		
		else if(numberofpoints == 0 && rhs.numberofpoints == 0) {
			sampleidentity = rhs.sampleidentity;
		}
		
		else if(rhs.numberofpoints != 0) {
			//destroy preexisting arrays
			delete [] datapoints;
			
			//create new member vectors for lhs object
			sampleidentity = rhs.sampleidentity;
			numberofpoints = rhs.numberofpoints;
			datapoints = new DatumPoint[rhs.numberofpoints];
			
			//deep copy values from rhs to lhs
			//uses DatumPointPolar overloaded operator =
			for(i = 0; i < numberofpoints; i++) {
				datapoints[i] = rhs.datapoints[i];
			}
		}
	}	
	
	bool DataPointSample::operator ==(const DataPointSample& rhs) const {
		int i, j, test, count;
		
		//check for self assignment	
		if(this == &rhs) test = 1;
		
		//false here if sizes are different
		else if(numberofpoints != rhs.numberofpoints) {
			test = 0;
			//cout << "\nnumber of points not equal"; 
		}
		//true if both objects are empty 
		else if(numberofpoints == rhs.numberofpoints && numberofpoints == 0) {
			test = 1;
		}  
		//continue test if not empty and sizes equal
		//includes comparison of point coordinates
		else if(numberofpoints == rhs.numberofpoints && numberofpoints > 0) {
			count = 0;
			while(count < numberofpoints && datapoints[count] == rhs.datapoints[count]) {
				count++;
			}
			if(count == numberofpoints) test = 1; 
			else { 
				test = 0; 
				//cout << "\npoints not equal"; 
			}
		}		
		
		if(	test == 1 ) 
			return true;
		else 
			return false;		
	}
	
} //namespace MMFdata

	
	







	
	