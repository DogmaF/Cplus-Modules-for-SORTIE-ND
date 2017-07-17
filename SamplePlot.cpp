/*
 *  SamplePlot.cpp
 *  
 *  Created by Mike Fuller on June 2, 2008.
 *  Copyright 2008. All rights reserved.
 *
 *	Implementation of class SamplePlot. 
 *
 */

#include "SamplePlot.h"
namespace MMFdata
{
	//default constructor
	SamplePlot::SamplePlot() : plotnumber(-1), plotsize(0), plotarea(0), plotlength(0), plotwidth(0),
								Xmax(0), Xmin(0), Ymax(0), Ymin(0)
	{							
		//deliberately empty
		//negative one for identity signifies empty object
	}
	
	//parameterized constructor
	SamplePlot::SamplePlot(int ID, int size, double xmax, double xmin, double ymax, double ymin)
	: plotnumber(ID), plotsize(size), Xmax(xmax), Xmin(xmin), Ymax(ymax), Ymin(ymin)
	{
		if( (Xmax - Xmin) > (Ymax - Ymin) ) {
			plotlength = Xmax - Xmin;
			plotwidth = Ymax - Ymin;
		}
		else {
			plotlength = Ymax - Ymin;
			plotwidth = Xmax - Xmin;
		}
		plotarea = plotlength * plotwidth;
	}
	
	//copy constructor
	SamplePlot::SamplePlot(const SamplePlot& datObject)
	{
		plotnumber = datObject.plotnumber;
		plotsize = datObject.plotsize;
		plotarea = datObject.plotarea;
		plotlength = datObject.plotlength;
		plotwidth = datObject.plotwidth;
		Xmax = datObject.Xmax;
		Xmin = datObject.Xmin;
		Ymax = datObject.Ymax;
		Ymin = datObject.Ymin;
	}
	
	//destructor
	SamplePlot::~SamplePlot()
	{
		//deliberately empty
	}
	
	int SamplePlot::get_plotno() const {
		return plotnumber;
	}
	int SamplePlot::get_size() const {
		return plotsize;
	}
	double SamplePlot::get_area() const {
		return plotarea;
	}
	double SamplePlot::get_length() const {
		return plotlength;
	}
	double SamplePlot::get_width() const {
		return plotwidth;
	}
	double SamplePlot::get_Xmax() const {
		return Xmax;
	}
	double SamplePlot::get_Xmin() const {
		return Xmin;
	}
	double SamplePlot::get_Ymax() const {
		return Ymax;
	}
	double SamplePlot::get_Ymin() const {
		return Ymin;
	}
	
	void SamplePlot::set_plotno(int ID) {
		plotnumber = ID;
	}
	void SamplePlot::set_size(int n) {
		plotsize = n;
	}
	double SamplePlot::set_area(double a) {
		plotarea = a;
	}
	double SamplePlot::set_length(double l) {
		plotlength = l;
	}
	double SamplePlot::set_width(double w) {
		plotwidth = w;
	}
	double SamplePlot::set_Xmax(double xmax) {
		Xmax = xmax;
	}
	double SamplePlot::set_Xmin(double xmin) {
		Xmin = xmin;
	}
	double SamplePlot::set_Ymax(double ymax) {
		Ymax = ymax;
	}
	double SamplePlot::set_Ymin(double ymin) {
		Ymin = ymin;
	}
	
	SamplePlot& SamplePlot::operator =(const SamplePlot& rhs) 
	{
		//check for self assignment
		if(this == &rhs) return *this;
		else {
			plotnumber = rhs.get_plotno();
			plotsize = rhs.get_size();
			plotarea = rhs.get_area();
			plotlength = rhs.get_length();
			plotwidth = rhs.get_width();
			Xmax = rhs.get_Xmax();
			Xmin = rhs.get_Xmin();
			Ymax = rhs.get_Ymax();
			Ymin = rhs.get_Ymin();
			return *this;
		}
	}
	
	bool SamplePlot::operator ==(const SamplePlot& rhs) const
	{
		return	((plotnumber == rhs.plotnumber)
				 && (plotsize == rhs.plotsize)
				 && (plotarea == rhs.plotarea)
				 && (plotlength == rhs.plotlength)
				 && (plotwidth == rhs.plotwidth)
				 && (Xmax == rhs.Xmax)
				 && (Xmin == rhs.Xmin)
				 && (Ymax == rhs.Ymax)
				 && (Ymin == rhs.Ymin)
				 );
	}
	
} //MMFdata

	
	
