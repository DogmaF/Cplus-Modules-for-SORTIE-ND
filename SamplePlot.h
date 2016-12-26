/*
 *  SamplePlot.h
 *  
 *  Created by Mike Fuller on June 2, 2008.
 *  Copyright 2008. All rights reserved.
 *
 *	PURPOSE:
 *	Base class for creating classes used for sample plot data.
 *
 */

#ifndef SAMPLEPLOT_H
#define SAMPLEPLOT_H

namespace MMFdata
{
	class SamplePlot
		{
		public:
			//default constructor; plotnumber set to -1
			//all other member variables set to zero
			SamplePlot(); 
			
			//sets member variables to values of arguments
			SamplePlot(int ID, int size, double xmax, double xmin, double ymax, double ymin);
			
			//copy constructor
			SamplePlot(const SamplePlot& datObject);
			
			//destructor
			virtual ~SamplePlot();
			
			//Accessor Functions 
			int get_plotno() const;
			int get_size() const;
			double get_area() const;
			double get_length() const;
			double get_width() const;
			double get_Xmax() const;
			double get_Xmin() const;
			double get_Ymax() const;
			double get_Ymin() const;
			
			//Mutator Functions
			void set_plotno(int ID);
			void set_size(int n);
			double set_area(double a);
			double set_length(double l);
			double set_width(double w);
			double set_Xmax(double xmax);
			double set_Xmin(double xmin);
			double set_Ymax(double ymax);
			double set_Ymin(double ymin);

			
			//Overloaded Operators
			SamplePlot& operator =(const SamplePlot& rhs);
			bool operator ==(const SamplePlot& rhs) const;
			
		private:
			int plotnumber;  //identity number
			int plotsize; //number of data points in plot
			double plotarea; 
			double plotlength, plotwidth;
			double Xmax, Xmin, Ymax, Ymin;  //plot corners
		};
	
} //MMFdata

#endif  //SAMPLEPLOT_H