/*
 *  DatumTree.cpp
 *  
 *  Created by Mike Fuller.
 *  Copyright 2008. All rights reserved.
 *
 *	Implementation of class DatumTree. 
 *
 */

#include "DatumTree.h"
#include <math.h>

namespace MMFdata 
{
	DatumTree::DatumTree() 
	: DatumPointPolar(),  stem_class(0), stump_class(0), mortality_class(0), decay_class(0)
	{
		//deliberately empty 
	}
	
	DatumTree::DatumTree(int ID, int spp, int value_int, double value_dec, double X, double Y, 
							int stemc, int stumpc, int mortc, int decayc)
	:	DatumPointPolar(ID, spp, value_int, value_dec, X, Y), 
		stem_class(stemc), stump_class(stumpc), mortality_class(mortc), decay_class(decayc)
	{
		calc_angle_fromcoords();
	}
	
	DatumTree::DatumTree(const DatumTree& dpptObject) 
									:	DatumPointPolar(dpptObject), stem_class(dpptObject.stem_class), 
										stump_class(dpptObject.stump_class), 
										mortality_class(dpptObject.mortality_class), 
										decay_class(dpptObject.decay_class)
	{
		//deliberately empty
	}
	
	DatumTree::~DatumTree() {
		//deliberately empty
	}
	
	int DatumTree::get_stemclass() const {
		return stem_class;
	}
	int DatumTree::get_stumpclass() const {
		return stump_class;
	}
	int DatumTree::get_mortclass() const {
		return mortality_class;
	}
	int DatumTree::get_decayclass() const {
		return decay_class;
	}
	
	void DatumTree::set_stemclass(int stemcl) {
		stem_class = stemcl;
	}
	void DatumTree::set_stumpclass(int stumpcl) {
		stump_class = stumpcl;
	}
	void DatumTree::set_mortclass(int mortcl) {
		mortality_class = mortcl;
	}
	void DatumTree::set_decayclass(int decaycl) {
		decay_class = decaycl;
	}
	
	DatumTree& DatumTree::operator =(const DatumTree& rhs) {
		//check for self assignment
		if(this == &rhs) return *this;
		else {
			DatumPointPolar::operator =(rhs);
			stem_class = rhs.get_stemclass();
			stump_class = rhs.get_stumpclass();
			mortality_class = rhs.get_mortclass();
			decay_class = rhs.get_decayclass();
			return *this;
		}
	}
	
	bool DatumTree::operator ==(const DatumTree& rhs) const {
		return	(DatumPointPolar::operator ==(rhs)
				 && (stem_class == rhs.stem_class)
				 && (stump_class == rhs.stump_class)
				 && (mortality_class == rhs.mortality_class)
				 && (decay_class == rhs.decay_class)
				 );
	}
	
} //MMFdata

	
	
