/*
 *  PlotTree.h
 *  
 *  Created by Mike Fuller on June 7, 2008.
 *  Copyright 2008. All rights reserved.
 *
 *  Constructed from classes: Datum and DatumPoint
 *
 *	PURPOSE:
 *	Data structure to hold information typical of individual
 *  trees that make up a forest stand, plot or other sample.
 *	Part of namespace MMFdata.
 *
 */

#ifndef PLOTTREE_H
#define PLOTTREE_H

#include <string>

using namespace std;

namespace MMFdata
{
	class PlotTree
		{
		public:
			
			//empty constructor
			//all numeric values except X, Y, and nndist set to -1
			//X, Y and nndist set to zero
			//plotsection set to "NL" for NULL
			PlotTree(); 
			
			//partially parameterized constructor
			//sets member variables to values of arguments
			//stump and decay classes set to -1
			//sets plotsection to "NL" and nndist to zero
			PlotTree(int specn, double stemdbh, double X, double Y, double ht, 
					 int stemc, int mortc, int st);
			
			//further parameterized constructor
			//sets member variables to values of arguments
			//stem, stump, mortality, and decay classes set to -1
			//sets nndist to zero
			PlotTree(int plotn, char section[3], int ID, int specn, 
					 double stemdbh, double X, double Y);
			
			//fully parameterized constructor
			//sets member variables to values of arguments
			PlotTree(int plotn, char section[3], int ID, int specn, 
					 double stemdbh, double X, double Y,
					 int stemc, int stumpc, int mortc, int decayc);
			
			//Copy constructor. Copies ALL values of datObject
			PlotTree(const PlotTree& datObject);
			
			//destructor
			virtual ~PlotTree();
			
			//Accessor Functions 
			int get_plotnumber() const;
			string get_plotsection() const;
			int get_identity() const;
			int get_species() const;
			double get_stemdiameter() const;
			double get_Xcoord() const;
			double get_Ycoord() const;
			double get_height() const;
			int get_stemclass() const;
			int get_stumpclass() const;
			int get_mortclass() const;
			int get_decayclass() const;
			int get_SortieType() const;
			double get_nndist() const;
			
			//Mutator Functions
			void set_plotnumber(int plotn);
			void set_plotsection(char section[3]);
			void set_identity(int ident);
			void set_species(int spn);
			void set_stemdiameter(double stemdbh);
			void set_Xcoord(double Xn);
			void set_Ycoord(double Yn);
			void set_XYcoords(double XYdat[2]);
			void shift_Xcoord(double distance);
			void shift_Ycoord(double distance);
			void set_height(double ht);
			void set_stemclass(int stemcl);
			void set_stumpclass(int stumpcl);
			void set_mortclass(int mortcl);
			void set_decayclass(int decaycl);
			void set_SortieType(int st);
			void set_nndist(double distance);
			
			//Overloaded Operators
			//sets all member variables of lhs to those of rhs
			PlotTree& operator =(const PlotTree& rhs);
			
			//booleans do not compare nndist or height
			bool operator !=(const PlotTree& rhs);
			bool operator ==(const PlotTree& rhs) const;
			
		private:
			int plotnumber;
			char plotsection[3];
			int identity;
			int species;
			double stemdiameter;
			double X_coordinate;
			double Y_coordinate;
			double height;
			int stem_class;
			int stump_class;
			int mortality_class;
			int decay_class;
			int SortieType;
			double nndist; //nearest neighbor distance
		};
	
} //MMFdata

#endif  //PLOTTREE_H
