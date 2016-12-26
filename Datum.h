/*
 *  Datum.h
 *  
 *  Created by Mike Fuller on 04/02/08.
 *  Copyright 2008. All rights reserved.
 *
 *	PURPOSE:
 *	Base class for creating classes used for sample data.
 *	Root of namespace MMFdata.
 *
 */

#ifndef DATUM_H
#define DATUM_H

namespace MMFdata
{
	class Datum
		{
		public:
			//identitynumber set to -1
			//all other member variables set to zero
			Datum(); 
			
			//sets member variables to values of arguments
			Datum(int ID, int spec, int value_int, double value_dec, double dbl);
			
			//copy constructor
			Datum(const Datum& datObject);
			
			//destructor
			virtual ~Datum();
			
			//Accessor Functions 
			int get_identity() const;
			int get_species() const;
			int get_value_int() const;
			double get_value_dec() const;
			double get_dbl_n() const;
			
			//Mutator Functions
			void set_identity(int ident);
			void set_species(int spn);
			void set_value_int(int vint);
			void set_value_dec(double vdec);
			void set_dbl_n(double n);
			
			//Overloaded Operators
			Datum& operator =(const Datum& rhs);
			bool operator ==(const Datum& rhs) const;
			
		private:
			int identitynumber;
			int species;
			int value_integer;
			double value_decimal;
			double dbl_n;
		};
	
} //MMFdata

#endif  //DATUM_H