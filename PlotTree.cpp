/*
 *  PlotTree.cpp
 *  
 *  Created by Mike Fuller on June 7, 2008.
 *  Copyright 2008. All rights reserved.
 *
 *	Implementation of class PlotTree. 
 *
 */

#ifndef PLOTTREE_CPP
#define PLOTTREE_CPP

#include "PlotTree.hpp"

#include <iostream>
#include <string>
#include <cstring> //using strcpy() and strcmp()

using namespace std;

namespace MMFdata
{
	//empty constructor
	PlotTree::PlotTree() : plotnumber(-1), identity(-1), species(-1), stemdiameter(-1), 
							X_coordinate(0), Y_coordinate(0), stem_class(-1), stump_class(-1),
							mortality_class(-1), decay_class(-1), SortieType(-1), nndist(0)
	{							
		strcpy(plotsection, "NL");
	}
	
	//partially parameterized constructor
	PlotTree::PlotTree(int specn, double stemdbh, double X, double Y, double ht, int stemc, int mortc, 
					   int st)
	: plotnumber(-1), identity(-1), species(specn), stemdiameter(stemdbh),
	X_coordinate(X), Y_coordinate(Y), height(ht), stem_class(stemc), mortality_class(mortc), 
	SortieType(st), stump_class(-1), decay_class(-1), nndist(0)
	{
		strcpy(plotsection, "NL");
	}
	
	//further parameterized constructor
	PlotTree::PlotTree(int plotn, char section[3], int ID, int specn, 
					   double stemdbh, double X, double Y)
	: plotnumber(plotn), identity(ID), species(specn), stemdiameter(stemdbh),
	  X_coordinate(X), Y_coordinate(Y), height(-1), stem_class(-1), stump_class(-1), 
	  mortality_class(-1), decay_class(-1), SortieType(-1), nndist(0)
	{
		strcpy(plotsection, section);
	}
	
	//fully parameterized constructor
	//all parameters EXCEPT nndist are set to values of arguments
	//no argument provided for nndist because this must be calculated from a data set
	//nndist value is therefore set to zero
	PlotTree::PlotTree(int plotn, char section[3], int ID, int specn, 
					   double stemdbh, double X, double Y,
					   int stemc, int stumpc, int mortc, int decayc)
	: plotnumber(plotn), identity(ID), species(specn), stemdiameter(stemdbh),
		X_coordinate(X), Y_coordinate(Y), height(-1), stem_class(stemc), stump_class(stumpc),
		mortality_class(mortc), decay_class(decayc), nndist(0)
	{
		strcpy(plotsection, section);
	}
	
	//copy constructor
	PlotTree::PlotTree(const PlotTree& datObject)
	{
		plotnumber = datObject.plotnumber;
		strcpy(plotsection, datObject.plotsection);
		identity = datObject.identity;
		species = datObject.species;
		stemdiameter = datObject.stemdiameter;
		X_coordinate = datObject.X_coordinate;
		Y_coordinate = datObject.Y_coordinate;
		height = datObject.height;
		stem_class = datObject.stem_class;
		stump_class = datObject.stump_class;
		mortality_class = datObject.mortality_class;
		decay_class = datObject.decay_class;
		SortieType = datObject.SortieType;
		nndist = datObject.nndist;
	}
	
	//destructor
	PlotTree::~PlotTree()
	{
		//deliberately empty
	}
	
	//Accessor Functions
	int PlotTree::get_plotnumber() const {
		return plotnumber;
	}
	string PlotTree::get_plotsection() const {
		return plotsection;
	}
	int PlotTree::get_identity() const {
		return identity;
	}
	int PlotTree::get_species() const {
		return species;
	}
	double PlotTree::get_stemdiameter() const {
		return stemdiameter;
	}
	double PlotTree::get_Xcoord() const {
		return X_coordinate;
	}
	double PlotTree::get_Ycoord() const {
		return Y_coordinate;
	}
	double PlotTree::get_height() const {
		return height;
	}
	int PlotTree::get_stemclass() const {
		return stem_class;
	}
	int PlotTree::get_stumpclass() const {
		return stump_class;
	}
	int PlotTree::get_mortclass() const {
		return mortality_class;
	}
	int PlotTree::get_decayclass() const {
		return decay_class;
	}
	int PlotTree::get_SortieType() const {
		return SortieType;
	}
	double PlotTree::get_nndist() const {
		return nndist;
	}

	
	//Mutator Functions
	void PlotTree::set_plotnumber(int plotn) {
		plotnumber = plotn;
	}
	void PlotTree::set_plotsection(char section[3]) {
		strcpy(plotsection, section);
	}
	void PlotTree::set_identity(int ident) {
		identity = ident;
	}
	void PlotTree::set_species(int spn) {
		species = spn;
	}
	void PlotTree::set_stemdiameter(double stemdbh) {
		stemdiameter = stemdbh;
	}
	void PlotTree::set_Xcoord(double Xn) {
		X_coordinate = Xn;
	}
	void PlotTree::set_Ycoord(double Yn) {
		Y_coordinate = Yn;
	}
	void PlotTree::set_height(double ht) {
		height = ht;
	}
	void PlotTree::set_XYcoords(double XYdat[2]) {
		X_coordinate = XYdat[0];
		Y_coordinate = XYdat[1];
	}
	void PlotTree::shift_Xcoord(double distance) {
		X_coordinate += distance;
	}
	void PlotTree::shift_Ycoord(double distance) {
		Y_coordinate += distance;
	}
	void PlotTree::set_stemclass(int stemcl) {
		stem_class = stemcl;
	}
	void PlotTree::set_stumpclass(int stumpcl) {
		stump_class = stumpcl;
	}
	void PlotTree::set_mortclass(int mortcl) {
		mortality_class = mortcl;
	}
	void PlotTree::set_decayclass(int decaycl) {
		decay_class = decaycl;
	}
	void PlotTree::set_nndist(double distance) {
		nndist = distance;
	}
	void PlotTree::set_SortieType(int st) {
		SortieType = st;
	}
	
	PlotTree& PlotTree::operator =(const PlotTree& rhs) 
	{
		//check for self assignment
		if(this == &rhs) return *this;
		else {
			plotnumber = rhs.plotnumber;
			strcpy(plotsection, rhs.plotsection);
			identity = rhs.identity;
			species = rhs.species;
			stemdiameter = rhs.stemdiameter;
			X_coordinate = rhs.X_coordinate;
			Y_coordinate = rhs.Y_coordinate;
			height = rhs.height;
			stem_class = rhs.stem_class;
			stump_class = rhs.stump_class;
			mortality_class = rhs.mortality_class;
			decay_class = rhs.decay_class;
			SortieType = rhs.SortieType;
			nndist = rhs.nndist;
			return *this;
		}
	}
	
	bool PlotTree::operator !=(const PlotTree& rhs)
	{
		bool status;
		
		//check for self assignment
		if(this == &rhs) status = false;
		
		//check each variable
		else if
		(	plotnumber != rhs.plotnumber
			|| strcmp(plotsection, rhs.plotsection)  //returns true if chars are different
			|| identity != rhs.identity
			|| species != rhs.species
			|| stemdiameter != rhs.stemdiameter
			|| X_coordinate != rhs.X_coordinate
			|| Y_coordinate != rhs.Y_coordinate
			|| height != rhs.height
			|| stem_class != rhs.stem_class
			|| stump_class != rhs.stump_class
			|| mortality_class != rhs.mortality_class
			|| decay_class != rhs.decay_class
			|| SortieType != rhs.SortieType
		 ) {
			status = true;
		}
		
		else status = false;
		
		return status;
	}
	
	bool PlotTree::operator ==(const PlotTree& rhs) const
	{
		return	( (plotnumber == rhs.plotnumber)
				 && !strcmp(plotsection, rhs.plotsection) //strcmp returns true if chars are different
				 && (identity == rhs.identity)
				 && (species == rhs.species)
				 && (stemdiameter == rhs.stemdiameter)
				 && (X_coordinate == rhs.X_coordinate)
				 && (Y_coordinate == rhs.Y_coordinate)
				 && (height == rhs.height)
				 && (stem_class == rhs.stem_class)
				 && (stump_class == rhs.stump_class)
				 && (mortality_class == rhs.mortality_class)
				 && (decay_class == rhs.decay_class)
				 && (SortieType == rhs.SortieType)
				 );
	}
	
	
	
} //MMFdata

#endif  //PLOTTREE_CPP




	
	
