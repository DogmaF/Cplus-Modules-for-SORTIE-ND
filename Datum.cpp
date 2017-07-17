/*
 *  Datum.cpp
 *  
 *  Created by Mike Fuller on 04/02/08.
 *  Copyright 2008. All rights reserved.
 *
 *	Implementation of class Datum. 
 *
 */

#include "Datum.h"
namespace MMFdata
{
	
	Datum::Datum() : identitynumber(-1), species(0), value_integer(0), value_decimal(0)
	{							
		//deliberately empty
		//negative one for identity signifies empty object
	}
	
	Datum::Datum(int ID, int spec, int value_int, double value_dec, double dbl)
	: identitynumber(ID), species(spec), value_integer(value_int), value_decimal(value_dec),
	  dbl_n(dbl)
	{
		//deliberately empty
	}
	
	Datum::Datum(const Datum& datObject)
	{
		identitynumber = datObject.identitynumber;
		species = datObject.species;
		value_integer = datObject.value_integer;
		value_decimal = datObject.value_decimal;
		dbl_n = datObject.dbl_n;
	}
	
	Datum::~Datum()
	{
		//deliberately empty
	}
	
	int Datum::get_identity() const {
		return identitynumber;
	}
	int Datum::get_species() const {
		return species;
	}
	int Datum::get_value_int() const {
		return value_integer;
	}
	double Datum::get_value_dec() const {
		return value_decimal;
	}
	double Datum::get_dbl_n() const {
		return dbl_n;
	}
	void Datum::set_identity(int ident) {
		identitynumber = ident;
	}
	void Datum::set_species(int spn) {
		species = spn;
	}
	void Datum::set_value_int(int vint) {
		value_integer = vint;
	}
	void Datum::set_value_dec(double vdec) {
		value_decimal = vdec;
	}
	void Datum::set_dbl_n(double n) {
		dbl_n = n;
	}
	
	Datum& Datum::operator =(const Datum& rhs) 
	{
		//check for self assignment
		if(this == &rhs) return *this;
		else {
			identitynumber = rhs.get_identity();
			species = rhs.get_species();
			value_integer = rhs.get_value_int();
			value_decimal = rhs.get_value_dec();
			dbl_n = rhs.get_dbl_n();
			return *this;
		}
	}
	
	bool Datum::operator ==(const Datum& rhs) const
	{
		return	((identitynumber == rhs.identitynumber)
				 && (species == rhs.species)
				 && (value_integer == rhs.value_integer)
				 && (value_decimal == rhs.value_decimal)
				 && (dbl_n == rhs.dbl_n)
				 );
	}
	
} //MMFdata

	
	
