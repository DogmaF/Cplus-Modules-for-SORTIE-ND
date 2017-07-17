/*
 *  CutBlock.cpp
 *  
 *  Created by Mike Fuller on June 25, 2008.
 *  Copyright 2008. All rights reserved.
 *
 *	Implementation of class CutBlock. 
 *
 */

#ifndef CUTBLOCK_CPP
#define CUTBLOCK_CPP

#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <iterator>
#include "CutBlock.hpp"
#include "PlotTree.cpp"

namespace MMFdata
{
	//default constructor
	CutBlock::CutBlock() : cutblock_n(-1), fractionlargestretained(0), harvest_error(0.02), cuttype("NULL"),
							maxtotalharvestpercent(0.33), BAtotalharvested(0), plotarea(0), targetscut(0),
							nborscut(0), ave_nborscut(0), harvest_thresholdBA(16.0), timetable_flag(false)
	{	
		errorlog.clear();
	}

	//errorlog constructor
	CutBlock::CutBlock(string errlog) : cutblock_n(-1), fractionlargestretained(0), harvest_error(0.02), cuttype("NULL"),
							maxtotalharvestpercent(0.33), BAtotalharvested(0), plotarea(0), targetscut(0),
							nborscut(0), ave_nborscut(0), harvest_thresholdBA(16.0), timetable_flag(false)
	{
		errorlog = errlog;
		ofstream fout;	
		fout.open(errorlog.c_str(), ios::app);
		fout << "\nCutBlock object initiated";
		fout.close();

	}
	
	//parameterized constructor
	CutBlock::CutBlock(vector<PlotTree> data, vector<double> limits, int ID, string& type, 
					   vector<double> pcut, vector<double> pnborcut)
	: cutblock_n(ID), cuttype(type), fractionlargestretained(0), harvest_error(0.02),
	  maxtotalharvestpercent(0.33), BAtotalharvested(0), plotarea(0), targetscut(0),
	  nborscut(0), ave_nborscut(0), harvest_thresholdBA(16.0), timetable_flag(false)
	{
		errorlog.clear();
		swap(treemap, data);
		swap(sizeclasses, limits);
		swap(probcuttarget, pcut);
		swap(probcutnbor, pnborcut);
		
		singularitytest(probcuttarget, "target");
		singularitytest(probcutnbor, "neighbor");
		
		//test for correct cuttype
		if( type != "singletree"
		   && type != "clearcut"
		   && type != "singletree_propagation"
		   //&& type != "perhectare"
		   //&& type != "distribution"
			) {
			cout << "\nERROR (constructor): harvest type " << type << " not recognized\n";
			exit(1);
		}
	}
	
	//copy constructor
	CutBlock::CutBlock(const CutBlock& datObject)
	{
		cutblock_n = datObject.cutblock_n;
		cuttype = datObject.cuttype;
		probcuttarget = datObject.probcuttarget;
		probcutnbor = datObject.probcutnbor;
		specieslist = datObject.specieslist;
		treemap = datObject.treemap;
		sizeclasses = datObject.sizeclasses;
		classcounts = datObject.classcounts;
		targetcounts = datObject.targetcounts;
		basalareatargets = datObject.basalareatargets;
		perhectaretargets = datObject.perhectaretargets;
		distributiontargets = datObject.distributiontargets;
		exception_species = datObject.exception_species;
		fractionlargestretained = datObject.fractionlargestretained;
		harvest_error = datObject.harvest_error;
		plotarea = datObject.plotarea;
		harvest_thresholdBA = datObject.harvest_thresholdBA;
		timetable_flag = datObject.timetable_flag;
		timetable = datObject.timetable;
	}
	
	//destructor
	CutBlock::~CutBlock()
	{
		//deliberately empty
	}
	
	//accessors
	const int CutBlock::get_cutblock_n() const {
		if(errorlog.length() > 0) {
			ofstream fout;	
			fout.open(errorlog.c_str(), ios::app);
			fout << "\nget_cutblock_n called";
			fout.close();
		}

		return cutblock_n;
	}
	const string CutBlock::get_cuttype() const {
		return cuttype;
	}	
	const vector<double> CutBlock::get_probcuttarget() const {
		return probcuttarget;
	}	
	const vector<double> CutBlock::get_probcutnbor() const {
		return probcutnbor;
	}	
	const vector<int> CutBlock::get_specieslist() const {
		return specieslist;
	}	
	const vector<PlotTree> CutBlock::get_treemap() const {
		return treemap;
	}
	const vector<PlotTreeNode*> CutBlock::get_hashtable() const {
		return PT_hashtable;
	}
	const vector<double> CutBlock::get_sizeclasses() const {
		return sizeclasses;
	}	
	const int CutBlock::get_numsizeclasses() const {
		return sizeclasses.size();
	}	
	const vector<int> CutBlock::get_classcounts() const {
		//assumes tree counts have been calculated
		return classcounts;
	}	
	const vector<int> CutBlock::get_targetcounts() const {
		return targetcounts;
	}	
	const vector<double> CutBlock::get_basalareatargets() const {
		return basalareatargets;
	}	
	const vector<int> CutBlock::get_perhectaretargets() const {
		return perhectaretargets;
	}		
	const vector<double> CutBlock::get_distributiontargets() const {
		return distributiontargets;
	}
	const double CutBlock::get_maxtotalharvestpercent() const {
		return maxtotalharvestpercent;
	}
	const double CutBlock::get_plotarea() const {
		return plotarea;
	}
	const int CutBlock::get_targetscut() const {
		return targetscut;
	}
	const int CutBlock::get_nborscut() const {
		return nborscut;
	}
	const double CutBlock::get_ave_nborscut() const {
		return ave_nborscut;
	}
	
	const double CutBlock::get_harvest_thresholdBA() const {
		if(errorlog.length() > 0) {
			ofstream fout;	
			fout.open(errorlog.c_str(), ios::app);
			fout << "\nget_harvest_threshold called";
			fout.close();
		}

		return harvest_thresholdBA;
	}
	const bool CutBlock::get_timetable_flag() const {
		return timetable_flag;
	}
	vector<int> CutBlock::get_timetable() {
		if(timetable.size() > 0)
			return timetable;
		else {
			vector<int> tmp;
			tmp.push_back(0);
			return tmp;
		}
	}
	
	vector<double> CutBlock::get_classbasalareas()  {
		int i;
		double ba;
		vector<double> temp;
		
		for(i = 1; i < sizeclasses.size() +1; i++) {
			ba = group_basalarea_perha(i);
			temp.push_back(ba);
		}
		return temp;
	}	
	
	PlotTree* CutBlock::get_ptr2tree(int index) {
		PlotTree* ptr;
		ptr = &treemap[index];
		return ptr;
	}
	
	//mutators
	void CutBlock::set_cutblock_n(int ID) {
		cutblock_n = ID;
	}
	void CutBlock::set_cuttype(string& type) {
		
		//test for correct cuttype
		if( type != "clearcut" 
		   && type != "singletree"
		   && type != "singletree_propagation"
		   //&& type != "perhectare"
		   //&& type != "distribution"
		   ) {
			cout << "\nERROR: harvest type " << type << " not recognized\n";
			exit(1);
		}
		else
			cuttype = type;
	}
	void CutBlock::set_probcuttarget(vector<double> pt) {
		swap(probcuttarget, pt);
		singularitytest(probcuttarget, "target");
	}
	void CutBlock::set_probcutnbor(vector<double> pn) {
		swap(probcutnbor, pn);
		singularitytest(probcutnbor, "neighbor");
	}
	void CutBlock::set_specieslist(vector<int> splist) {
		swap(specieslist, splist);
	} 
	void CutBlock::set_treemap(vector<PlotTree> map) {
		swap(treemap, map);
	}
	void CutBlock::set_sizeclasses(vector<double> limits) {
		swap(sizeclasses, limits);
	}
	void CutBlock::set_classcounts(vector<int> clcounts) {
		swap(classcounts, clcounts);
	}
	void CutBlock::set_targetcounts(vector<int> tcounts) {
		swap(targetcounts, tcounts);
	}
	void CutBlock::set_basalareatargets(vector<double> tba) {
		swap(basalareatargets, tba);
	}
	void CutBlock::set_perhectaretargets(vector<int> tperha) {
		swap(perhectaretargets, tperha);
	}
	void CutBlock::set_distributiontargets(vector<double> dist) {
		swap(distributiontargets, dist);
	}
	void CutBlock::set_maxtotalharvestpercent(double max) {
		maxtotalharvestpercent = max;
	}
	void CutBlock::set_harvest_thresholdBA(double thresholdBA) {
		harvest_thresholdBA = thresholdBA;
	}
	void CutBlock::set_timetable(vector<int> ttable) {
		timetable_flag = true;
		timetable = ttable;
	}
	
	void CutBlock::set_criteria(vector<double> sizeclasses, vector<double> classprobcut, 
								vector<double> classnborprobcut,
								vector<int> harvestgoals, string& harvesttype) {
		sizeclasses = sizeclasses;
		probcuttarget = classprobcut;
		probcutnbor = classnborprobcut;
		cuttype = harvesttype;
		
		singularitytest(probcuttarget, "target");
		singularitytest(probcutnbor, "neighbor");
		
		if( harvesttype != "clearcut"
		   && harvesttype != "perhectare"
		   ) 
		{
			cout << "\nERROR: harvest type " << harvesttype << " not recognized\n"
			<< "Check that choice and input value types match\n";
			exit(1);
		}
	}
	
	void CutBlock::set_criteria(vector<double> sizeclasses, vector<double> classprobcut, 
								vector<double> classnborprobcut,
								vector<double> harvestgoals, string& harvesttype) {
		sizeclasses = sizeclasses;
		probcuttarget = classprobcut;
		probcutnbor = classnborprobcut;
		cuttype = harvesttype;
		
		singularitytest(probcuttarget, "target");
		singularitytest(probcutnbor, "neighbor");
		
		if( harvesttype != "clearcut" 
		   && harvesttype != "singletree"
		   && harvesttype != "singletree_propagation"
		   //&& harvesttype != "distribution"
		   ) 
		{
			cout << "\nERROR: harvest type " << harvesttype << " not recognized\n"
			<< "Check that choice and input value types match\n";
			exit(1);
		}
	}
	
	void CutBlock::get_pramfile(const string directorypath) {
		if(errorlog.length() > 0) {
			ofstream fout;
			fout.open(errorlog.c_str(), ios::app);
			fout << "\nget_pramfile called";
			fout.close();
		}

		//input streams
		ifstream fin1, finext;
		int i, tmpint, filesize(0), no_sp_exceptions(0);
		int oneline(100);  //used by getline fct
		double tempdbl;
		char txtline[100]; //used by getline fct
		string targetsfile;
		string plotcornersfile;
				
		//file names
		//filecode is default or the name of a different parameter file
		string filecode;
		string treedatafile, sizeclassfile, probcutfile, probnborfile;
		
		//Print header to std out
		cout << "\nEXTERNAL FILES USED THIS RUN\n";
		
		//connect to parameter file and get parameter settings and file names
		//if the file code "default" is used, the default names for files is expected
		openfile(fin1, directorypath);
		fin1 >> filecode;
		if(filecode == "default") {
			fin1 >> cutblock_n >> cuttype >> treedatafile 
			>> sizeclassfile >> probcutfile >> probnborfile >> plotcornersfile
			>> targetsfile >> no_sp_exceptions >> fractionlargestretained;
			
			if(fractionlargestretained > 0.9999) {
				cout << "\nERROR (parameters): percent of trees to preserve must be less than 100%\n"
				<< "ABORTING\n";
				exit(1);
			}
			
			for(i = 0; i < no_sp_exceptions; i++) {
				fin1 >> tmpint;
				exception_species.push_back(tmpint);
			}
			
			fin1.close();
			cout << "\n\tPARAMETER FILE: " << directorypath;
		}
		//if an alternative pram file is preferred, skip default file and open alternative
		else {
			fin1.close();
			openfile(finext, filecode);
			finext >> cutblock_n >> cuttype >> treedatafile 
			>> sizeclassfile >> probcutfile >> probnborfile >> plotcornersfile
			>> targetsfile >> no_sp_exceptions >> fractionlargestretained;
			
			if(fractionlargestretained > 0.9999) {
				cout << "\nERROR (parameters): percent of trees to preserve must be less than 100%\n"
					<< "ABORTING\n";
				exit(1);
			}
			
			for(i = 0; i < no_sp_exceptions; i++) {
				finext >> tmpint;
				exception_species.push_back(tmpint);
			}
			
			finext.close();
			cout << "\n\tPARAMETER FILE: " << filecode;
		}
		
		//connect to size classes file and get values
		//IMPORTANT: size classes must be extracted before treedata
		//because treedata input relies on size classes to set PlotTree variables
		extract_sizeclassfile(sizeclassfile);
		
		//get harvest targets and save to member vectors
		if(cuttype == "singletree") getr_harvesttargets(targetsfile, basalareatargets);
		if(cuttype == "singletree_propagation") getr_harvesttargets(targetsfile, basalareatargets);
		//if(cuttype == "perhectare") getr_harvesttargets(targetsfile, perhectaretargets);
		//if(cuttype == "distribution") getr_harvesttargets(targetsfile, distributiontargets);
		
		//connect to target probabilities file and get values
		extract_probcutfile(probcutfile);
		
		//connect to neighbor probabilities file and get values
		extract_nborcutfile(probnborfile);
		
		//connect to tree data file and get values
		extract_treedatafile(treedatafile);
		
		//prepare tree data for harvesting and set counts of each class and nonzero classes
		prep_treedata();
		
		//connect to plotcorners file and get values
		extract_plotcornersfile(plotcornersfile);
		
		//calculate plot total area in hectares
		plotarea = calc_plotarea();
		
		//if plotarea not > 0, abort
		if(plotarea <= 0) 
		{
			cout << "\nERROR (get_pramfile): plot area is zero or less\n";
			cout << "ABORTING\n";
			exit(1);
		}
	}
	
	void CutBlock::show_harvestcriteria() {
		int i;
		cout << "\n\nHARVEST CRITERIA\n"
		<< "\nHarvest Type: " << cuttype
		<< "\nPlot Corners: " << plotcorners[0] << ", " << plotcorners[1] << ", "
		<< plotcorners[2] << ", " << plotcorners[3] << endl
		<< "\nS-Class\tLim\tTar-P\tNbr-P\tTarget\n";
		
		for(i = 0; i < sizeclasses.size(); i++) {
			cout << "  " << i << "\t"
			<< "  " << setprecision(2) << sizeclasses[i] << "\t  "
			<< setprecision(3) << probcuttarget[i] << "\t "
			<< probcutnbor[i] << "\t";
			
			if(cuttype == "singletree" || cuttype == "singletree_propagation") 
				cout << setprecision(2) << basalareatargets[i] << "% BA\n"<< flush;
			else if(cuttype == "perhectare") 
				cout << setprecision(2) << perhectaretargets[i] << "/ha\n"<< flush;
			else if(cuttype == "distribution") 
				cout << setprecision(2) << distributiontargets[i] << "% relative abundance\n"<< flush;
			else cout << endl;
		}
	}
	
	void CutBlock::show_treemap() {
		vector<PlotTree>::iterator p;
		
		//print header lines of output file
		cout << "X\tY\tSpecies\tType\tDiam\tHeight\tMortClass\n";
		
		if(treemap.size() > 0) {
			for(p = treemap.begin(); p != treemap.end(); ++p) {
				
				//print only dead trees
				cout << p->get_Xcoord() << "\t" << p->get_Ycoord() << "\t"
				<< p->get_species() << "\t" << p->get_SortieType() << "\t"
				<< p->get_stemdiameter() << "\t" << p->get_height() << "\t"
				<< p->get_mortclass() << endl;
			}
		}
		else
		  	cout << "\nNo data in treemap";
	}
	
	void CutBlock::show_mortclasses() {
		cout << "\nDefault Mortality Classes\n"
		<< " 1 = live\n"
		<< " 2 = stump\n"
		<< " 3 = snag\n"
		<< "-1 = class not set\n";
	}
	 
	void CutBlock::print_treemap() {
		vector<PlotTree>::iterator p;
		cout << "\nCB\tTree\tSpecies\tDBH\tSizeCl\tMort\n";
		
		//show first line, with cutblock number in first column
		cout << cutblock_n << "\t"
		<< treemap[0].get_identity() << "\t"
		<< treemap[0].get_species() << "\t"
		<< treemap[0].get_stemdiameter() << "\t"
		<< treemap[0].get_stemclass() << "\t"
		<< treemap[0].get_mortclass() << endl;
		
		//show remaining lines, tabbed to right by one column
		for(p = treemap.begin() +1; p != treemap.end(); ++p) {
			cout << "\t" << p->get_identity() << "\t"
			<< p->get_species() << "\t"
			<< p->get_stemdiameter() << "\t"
			<< p->get_stemclass() << "\t"
			<< p->get_mortclass() << endl;
		}
	}
	
	void CutBlock::print_SORTIEdata(const string directorypathandfilename) {
		ofstream fout;
		int i;

		if(errorlog.length() > 0) {
			ofstream fout2;	
			fout2.open(errorlog.c_str(), ios::app);
			fout2 << "\nPrint_SORTIEdata called";
			fout2.close();
		}
		
		//connect to new file
		openoutput(fout, directorypathandfilename);
		
		//print time step variables and header lines of output file
		fout << currentstep << "\t" << totalstep << endl
		<< "X\tY\tSpecies\tType\tDiam\tHeight\n";
			
		for(i = 0; i < PT_hashtable.size(); i++) {
				
			//print only dead trees
			if(PT_hashtable[i]->getPTptr()->get_mortclass() > 1) {
				fout 
				<< PT_hashtable[i]->getPTptr()->get_Xcoord() << "\t" 
				<< PT_hashtable[i]->getPTptr()->get_Ycoord() << "\t"
				<< PT_hashtable[i]->getPTptr()->get_species() << "\t" 
				<< PT_hashtable[i]->getPTptr()->get_SortieType() << "\t"
				<< PT_hashtable[i]->getPTptr()->get_stemdiameter() << "\t" 
				<< PT_hashtable[i]->getPTptr()->get_height() << endl;
			}
		}

		fout.close();
	}
	
	void CutBlock::print_harvestmap(const string directorypathandfilename) {
		ofstream fout;
		int i;
		vector<PlotTree>::iterator p;
		
		//connect to new file
		openoutput(fout, directorypathandfilename);
		
		//print header lines of output file
		fout << "X\tY\tDBH\tMortclass\n";
		
		//print mortality status for each tree		
		for(i = 0; i < PT_hashtable.size(); i++) {
			fout 
			<< PT_hashtable[i]->getPTptr()->get_Xcoord() << "\t" 
			<< PT_hashtable[i]->getPTptr()->get_Ycoord() << "\t"
			<< PT_hashtable[i]->getPTptr()->get_stemdiameter() << "\t"
			<< PT_hashtable[i]->getPTptr()->get_mortclass() << "\t"
			<< endl;
		}
		fout.close();
	}
	
	void CutBlock::revsort_treemap_bydiameter() 
	{
		int index, index_of_next_largest;
		
		for(index = 0; index < treemap.size() -1; index++)
		{
			index_of_next_largest = find_maxdiam(index);
			swap_PlotTree(treemap.at(index), treemap.at(index_of_next_largest) );
		}
		
	}
	
	void CutBlock::sort_treemap_byID() {
		int index, index_of_next_smallest;
		
		for(index = 0; index < treemap.size() -1; index++)
		{
			index_of_next_smallest = find_minID(index);
			swap_PlotTree(treemap.at(index), treemap.at(index_of_next_smallest) );
		}
	}

	bool CutBlock::cuttarget(PlotTree* target) {
		int diamclass;
		double probability;
		bool result(false);
		
		//get_stemclass(); returns integer > 0 
		diamclass = target->get_stemclass();

		if(diamclass > 0) {  //limit targets to trees in merchantable classes
		
			//get probability; subtract 1 from stem class to get appropriate index
			probability = probcuttarget[diamclass];
		
			//run test
			result = cuttree(target, probability);
		}
		
		return result;
	}
	
	bool CutBlock::cutnbor(PlotTree* neighbor) {
		bool result(false), is_rpair(false);
		int i, index, vsize, diamclass, ID;
		double probability, basalarea;
		
		//get_stemclass() returns integer > 0 if stem class has been set
		diamclass = neighbor->get_stemclass();

		//if(diamclass > 0) {

			//get additional details
			ID = neighbor->get_identity();
			//get size of recip nbor vector
			vsize = recipnbor_IDs.size();
		
			//Set cutting probability value
			probability = probcutnbor[diamclass];
		
			//Search for neighbor on recipnbor_IDs vector
			index = 0;
			while(ID != recipnbor_IDs[index] && index < vsize) { index++; }
			if(index < vsize) { is_rpair = true; }
			else is_rpair = false;
		
			//If neighbor is a member of a reciprocalpair, use probability correction.
			if(is_rpair == true) probability = sqrt( probability/(probability +1) );
		
			//attempt to cut neighbor; return true if neighbor is cut
			//if true, mortclass is set to 2
			result = cuttree(neighbor, probability);
		//}
		
		return result;
	}
	
	bool CutBlock::test_nextnbor(PlotTree* nextnbor) {
		int index, vsize, sizeclass, mortclass, ID;
		double probability;
		bool test(false), result(false), is_rpair(false);
		
		//find current nbor in PT_hashtable
		index = 0;
		while(nextnbor != PT_hashtable[index]->getPTptr() 
			  && index < PT_hashtable.size() ) {
			index++;
		}
		
		if(index != PT_hashtable.size()) {
			// now that index of current nbor has been found, use index to 
			// redirect the pointer to the neighbor of nbor,
			// continuing the walk from neighbor to neighbor
			
			nextnbor = PT_hashtable[index]->getLink()->getPTptr();
			sizeclass = nextnbor->get_stemclass();

			//if(sizeclass > 0) {
				mortclass = nextnbor->get_mortclass();
				ID = nextnbor->get_identity();
				vsize = recipnbor_IDs.size();
			
				//Set cutting probability value
				//subtract 1 to get appropriate index for probability value
				probability = probcutnbor[sizeclass];
			
				//Search for neighbor on recipnbor_IDs vector
				index = 0;
				while(ID != recipnbor_IDs[index] && index < vsize) { index++; }
				if(index < vsize) is_rpair = true; 
				else is_rpair = false;
			
				//If neighbor is a member of a reciprocalpair, use probability correction.
				if(is_rpair == true) probability = sqrt( probability/(probability +1) );
			
				//test whether to cut this neighbor
				test = cuttest(probability);
			
				//if test result is true, and neighbor was previously alive, 
				//cut it, update basal area totals, 
				//and return true
				if( test == true && mortclass == 1) {
					nextnbor->set_mortclass(2);
					addto_basalarearemoved(nextnbor);
					result = true;
				}
			//}
			
			//if test result is true, and neighbor was previously dead, 
			//restore it (set mort class to 1 and subtract basal area from totals)
			//and return false
			else if( test == true && mortclass > 1) {
				restoretree(nextnbor);
				result = false;
			}
			
			//otherwise, if test is false, leave tree alone,
			//and return false
			else if(test == false) result = false;
		}
		
		else {
			cout << "\nERROR (test_nextnbor): "
			<< "can not locate index of nbor on PT_hashtable\n"
			<< "ABORTING\n" << flush;
			exit(1);
		}
		
		return result;
	}
	
	void CutBlock::harvest() {

		if(errorlog.length() > 0) {
			ofstream fout;	
			fout.open(errorlog.c_str(), ios::app);
			fout << "\nharvest function called";
			fout.close();
		}

		if( cuttype == "clearcut" ) cutblock_clearcut();
		else if( cuttype == "singletree" ) cutblock_singletree();
		else if( cuttype == "singletree_propagation" ) cutblock_singletree_propagation();
		//else if( cuttype == "perhectare" ) cutblock_countperha();
		//else if( cuttype == "distribution" ) cutblock_distribution();
		else {
			cout << "\nHarvest type " << cuttype << " not recognized\n";
			exit(1);
		}
	}
	
	void CutBlock::cutblock_clearcut() {
		vector<PlotTreeNode*>::iterator p;
		
		cout << "\nCommencing Clearcut\n";
		
		//Kill them. Kill them ALL!!!!
		for(p = PT_hashtable.begin(); p != PT_hashtable.end(); ++p) {
			(*p)->getPTptr()->set_mortclass(2);
		}
	}
	
	void CutBlock::cutblock_singletree() {
		
		if(errorlog.length() > 0) {
			ofstream fout;	
			fout.open(errorlog.c_str(), ios::app);
			fout << "\ncutblock_singletree() function called";
			fout.close();
		}
		
		int i, top, bottom, index, randindex, trees2protect;
		double BAofprotected(0), precutBA(0), maxnndist;
		bool test;
		PlotTree* ptr = NULL;
		PlotTree* nbor = NULL;
		PlotTreeNode* next = NULL;
		
		if(treemap.size() > 0) {
			
			//get pre-cut basal area of forest stand, using treemap
			for(i = 0; i < sizeclasses.size(); i++) {
				precutBA += group_basalarea_perha(i);
			}
			cout << "\nPrecut BA " << precutBA;
			
			if(precutBA >= harvest_thresholdBA) {
				
				//Set up cstdlib random number generator
				set_rand();
				
				//continue only if there are trees that meet the sizeclass criteria
				if(PT_hashtable.size() > 0) {
										
					//convert target percentages to basal area in meters-square per hectare 
					//to cut from each size class
					for(i = 0; i < basalareatargets.size(); i++) { basalareatargets[i] *= group_basalarea_perha(i); }
					
					//initialize vector used to sum basal area removed per class
					basalarea_tracker.clear();
					for(i = 0; i < sizeclasses.size(); i++) { basalarea_tracker.push_back(0); }
					
					//Harvest the trees 
					//continue harvest until error-corrected basal area target BA oflargest class is met
					cout << "\nCommencing Single-Tree Selection by: Random Harvest" << flush;
					BAharvest_byrandindiv();
				}
				else cout << "\nNo trees in data set meet size class criteria\n";
				cout << "\nHarvest Completed\n" << flush;
			}
			else cout << "\nThreshold basal area for harvest not met. No trees harvested."
				<< "\nThreshold BA: " << harvest_thresholdBA << "\nStand BA: " << precutBA << endl;
		}
		else cout << "\nNo trees to harvest\n";
	}
	
	void CutBlock::cutblock_singletree_propagation() {
		
		if(errorlog.length() > 0) {
			ofstream fout;	
			fout.open(errorlog.c_str(), ios::app);
			fout << "\ncutblock_singletree_propagation() function called";
			fout.close();
		}
		
		ifstream fin;
		int i, trees2protect(0);
		double BAofprotected(0), precutBA(0), maxnndist;
		char ans('n');
		
		if(treemap.size() > 1) {
			
			//get pre-cut basal area of forest stand, using treemap
			for(i = 0; i < sizeclasses.size(); i++) {
				precutBA += group_basalarea_perha(i);
			}
			cout << "\nPrecut BA " << precutBA;
			
			if(precutBA >= harvest_thresholdBA) {
				//Set up cstdlib random number generator
				set_rand();
								
				//continue only if there are trees that meet the sizeclass criteria
				if(PT_hashtable.size() > 0) {
					
					//Find nearest neighbors of trees and link them up in PT_hashtable.
					link_neighbors();
					
					//get largest distance between nearest neighbors in core
					//for use as torus buffer distance
					maxnndist = max_nndist();
					
					//search for reciprocal pairs and fill up recipnbor_IDs vector
					find_recippairs();					
					
					//convert target percentages to basal area in meters-square per hectare 
					//to cut from each size class
					for(i = 0; i < basalareatargets.size(); i++) { basalareatargets[i] *= group_basalarea_perha(i); }
					
					//initialize vector used to sum basal area removed per class during harvest
					basalarea_tracker.clear();
					for(i = 0; i < sizeclasses.size(); i++) { basalarea_tracker.push_back(0); }
					
					//Harvest the trees 
					//continue harvest until error-corrected basal area target BA oflargest class is met
					cout << "\nCommencing Single-Tree Selection by: Basal Area\n" << flush;
					propagationcut_BA();
				}
				else cout << "\nNo trees in data set meet size class criteria\n";
				cout << "\nHarvest Completed\n" << flush;
			}
			else cout << "\nThreshold basal area for harvest not met. No trees harvested."
					<< "\nThreshold BA: " << harvest_thresholdBA << "\nStand BA: " << precutBA << endl;
		}
		else cout << "\nNo trees exist to harvest\n";
	}
	
	/***
	void CutBlock::cutblock_grouptree() {
		if(errorlog.length() > 0) {
			ofstream fout;	
			fout.open(errorlog.c_str(), ios::app);
			fout << "\ncutblock_grouptree() function called";
			fout.close();
		}
		
		ifstream fin;
		int i, trees2protect(0);
		double BAofprotected(0), precutBA(0), maxnndist;
		char ans('n');
		
		if(treemap.size() > 1) {
			
			//get pre-cut basal area of forest stand, using treemap
			for(i = 0; i < sizeclasses.size(); i++) {
				precutBA += group_basalarea_perha(i);
			}
			cout << "\nPrecut BA " << precutBA;
			
			if(precutBA >= harvest_thresholdBA) {
				//Set up cstdlib random number generator
				set_rand();
				
				//continue only if there are trees that meet the sizeclass criteria
				if(PT_hashtable.size() > 0) {				
					
					//convert target percentages to basal area in meters-square per hectare 
					//to cut from each size class
					for(i = 0; i < basalareatargets.size(); i++) { basalareatargets[i] *= group_basalarea_perha(i); }
					
					//initialize vector used to sum basal area removed per class during harvest
					basalarea_tracker.clear();
					for(i = 0; i < sizeclasses.size(); i++) { basalarea_tracker.push_back(0); }
					
					//Harvest the trees 
					//continue harvest until error-corrected basal area target BA oflargest class is met
					cout << "\nCommencing Group-Tree Selection by: Basal Area\n" << flush;
					groupcut_BA();
				}
				else cout << "\nNo trees in data set meet size class criteria\n";
				cout << "\nHarvest Completed\n" << flush;
			}
			else cout << "\nThreshold basal area for harvest not met. No trees harvested."
				<< "\nThreshold BA: " << harvest_thresholdBA << "\nStand BA: " << precutBA << endl;
		}
		else cout << "\nNo trees exist to harvest\n";
	}
	***/
	
	//operators
	CutBlock& CutBlock::operator =(const CutBlock& rhs) 
	{
		//check for self assignment
		if(this == &rhs) return *this;
		else {
			cutblock_n = rhs.cutblock_n;
			cuttype = rhs.cuttype;
			probcuttarget = rhs.probcuttarget;
			probcutnbor = rhs.probcutnbor;
			specieslist = rhs.specieslist;
			
			treemap = rhs.treemap;
			sizeclasses = rhs.sizeclasses;
			classcounts = rhs.classcounts;
			targetcounts = rhs.targetcounts;
			basalareatargets = rhs.basalareatargets;
			
			perhectaretargets = rhs.perhectaretargets;
			distributiontargets = rhs.distributiontargets;
			exception_species = rhs.exception_species;
			fractionlargestretained = rhs.fractionlargestretained;
			harvest_error = rhs.harvest_error;
			
			plotarea = rhs.plotarea;
			harvest_thresholdBA = rhs.harvest_thresholdBA;
			timetable_flag = rhs.timetable_flag;
			timetable = rhs.timetable;
			return *this;
		}
	}
	
	bool CutBlock::operator !=(const CutBlock& rhs) const
	{
		bool status;
		
		//check for self assignment
		if(this == &rhs) status = false;
		
		//check each variable
		else if
			(	cutblock_n != rhs.cutblock_n
			 || cuttype != rhs.cuttype  //returns true if chars are different
			 || probcuttarget != rhs.probcuttarget
			 || probcutnbor != rhs.probcutnbor
			 || specieslist != rhs.specieslist
			 || treemap != rhs.treemap
			 
			 || sizeclasses != rhs.sizeclasses
			 || classcounts != rhs.classcounts
			 || targetcounts != rhs.targetcounts
			 || basalareatargets != rhs.basalareatargets
			 || perhectaretargets != rhs.perhectaretargets
			 
			 || distributiontargets != rhs.distributiontargets
			 || exception_species != rhs.exception_species
			 || fractionlargestretained != rhs.fractionlargestretained
			 || harvest_error != rhs.harvest_error
			 || plotarea != rhs.plotarea
			 
			 || harvest_thresholdBA != rhs.harvest_thresholdBA
			 || timetable_flag != rhs.timetable_flag
			 || timetable != rhs.timetable
			 
			 ) {
				status = true;
		}
		
		else status = false;
		
		return status;
	}
	
	bool CutBlock::operator ==(const CutBlock& rhs) const
	{
		return	( (cutblock_n == rhs.cutblock_n)
				 && cuttype == rhs.cuttype  //returns true if chars are different
				 && probcuttarget == rhs.probcuttarget
				 && probcutnbor == rhs.probcutnbor
				 && specieslist == rhs.specieslist
				 && treemap == rhs.treemap
				 
				 && sizeclasses == rhs.sizeclasses
				 && classcounts == rhs.classcounts
				 && targetcounts == rhs.targetcounts
				 && basalareatargets == rhs.basalareatargets
				 && perhectaretargets == rhs.perhectaretargets
				 
				 && distributiontargets == rhs.distributiontargets
				 && exception_species == rhs.exception_species
				 && fractionlargestretained == rhs.fractionlargestretained
				 && harvest_error == rhs.harvest_error
				 && plotarea == rhs.plotarea
				 
				 && harvest_thresholdBA == rhs.harvest_thresholdBA
				 && timetable_flag == rhs.timetable_flag
				 && timetable == rhs.timetable
				 
				 
				 );
	}
			
			
	int CutBlock::find_maxdiam(int top)
	{
		// set largest tree to current top of treemap
		// set index of largest tree to top of treemap
		
		vector<PlotTree>::iterator p;
		double max = treemap.at(top).get_stemdiameter(); 
		int index(top+1), index_of_max(top); 
		
		for(p = treemap.begin() + top +1; p != treemap.end(); ++p)
		{ 
			if(p->get_stemdiameter() > treemap.at(index_of_max).get_stemdiameter() )
			{
				index_of_max = index;
			}
			index++;
			
		} // end for
		
		return (index_of_max);
		
	} 
	
	int CutBlock::find_minID(int top)
	{
		// set smallest identity to current top of treemap
		// set index of smallest identity to top of treemap
		
		vector<PlotTree>::iterator p;
		double min = treemap.at(top).get_identity(); 
		int index(top+1), index_of_min(top); 
		
		for(p = treemap.begin() + top +1; p != treemap.end(); ++p)
		{ 
			if(p->get_identity() < treemap.at(index_of_min).get_identity() )
			{
				index_of_min = index;
			}
			index++;
			
		} // end for
		
		return (index_of_min);
		
	} 
	
	void CutBlock::swap_PlotTree(PlotTree& t1, PlotTree& t2)
	{
		PlotTree temp = t1;
		t1 = t2;
		t2 = temp;
	}
	
	//THIS FUNCTION DOES NOT APPEAR TO BE USED
	//THIS FUNCTION MAY NEED TO BE UPDATED TO REFLECT CHANGES IN SIZECLASSES
	int CutBlock::index_startofclass(int sizeclass) {
		int index(0);
		double classtop;  //upper limit of tree diameter for this size class
						  //assumes trees are reverse sorted by size
		
		if(sizeclass < sizeclasses.size() && sizeclass > 0) {
			//set value of classtop to diameter of largest tree in class
			classtop = sizeclasses[sizeclass -1]; 
			//step through treemap until smallest tree in size class is found
			while(treemap[index].get_stemdiameter() >= classtop) {
				//cout << "stemdiam " << treemap[index].get_stemdiameter() << endl << flush;
				index++;
			}
		}

		else {
			cout << "\nERROR (index_startofclass): chosen size class is out of range\n"
				<< "ABORTING\n";
			exit(1);
		}
		return index;
	}
			
	void CutBlock::set_defaultclasssizes() {
		sizeclasses.reserve(6);
		sizeclasses.push_back(0.0);
		sizeclasses.push_back(10.0);
		sizeclasses.push_back(26.0);
		sizeclasses.push_back(38.0);
		sizeclasses.push_back(50.0);
		sizeclasses.push_back(62.0);
	}
	
	double CutBlock::dist(PlotTree a, PlotTree b)
	{
		//Implements plot as a torus. 
		//Determines shortest distance between two trees.
		//Considers distance between trees when edges of plot wrap around.
		
		double Xdist, Ydist, dist1, dist2;
		double aX = a.get_Xcoord();
		double aY = a.get_Ycoord();
		double bX = b.get_Xcoord();
		double bY = b.get_Ycoord();
		double Xlen = plotcorners[1] - plotcorners[0];
		double Ylen = plotcorners[2] - plotcorners[3];
		
		//Accounts for diameter of stem when calculating distance between trees.
		//Stem diameter is stored in units of cm. Convert cm to meters (divide by 100).
		//Subtract from the inter-tree distance the radius of each tree (divide diameter by 2).
		//Dbh is store in the value_dec member variable.
		double radiuscorrection = a.get_stemdiameter()/200 + b.get_stemdiameter()/200;
		
		//For each axis, find the shortest distance: with or without torus
		//wrapping.  To torus-wrap, find the smallest coordinate and add the
		//plot length to it.
		
		if(bX > aX) {
			dist1 = bX - aX;
			dist2 = (aX + Xlen) - bX;
			if(dist1 < dist2) { Xdist = dist1; }
			else { Xdist = dist2; }
		}
		else {
			dist1 = aX - bX;
			dist2 = (bX + Xlen) - aX;
			if(dist1 < dist2) { Xdist = dist1; }
			else { Xdist = dist2; }
		}
		
		if(bY > aY) {
			dist1 = bY - aY;
			dist2 = (aY + Ylen) - bY;
			if(dist1 < dist2) { Ydist = dist1; }
			else { Ydist = dist2; }
		}
		else {
			dist1 = aY - bY;
			dist2 = (bY + Ylen) - aY;
			if(dist1 < dist2) { Ydist = dist1; }
			else { Ydist = dist2; }
		}
		
		return sqrt((Xdist * Xdist) + (Ydist * Ydist)) - radiuscorrection;
	}
	
	bool CutBlock::cuttree(PlotTree* target, const double pvalue) {
		//a stump is assigned to mortality class 2
		
		bool testresult(false);
		
		//perform test to determine whether to make tree a stump
		testresult = cuttest(pvalue);

		if(testresult == true) target->set_mortclass(2);
		
		return testresult;		
	}
	
	bool CutBlock::cuttest(const double pval) {
		
		double testvalue;
		bool result(false);
		
		//extract random integer
		int temp = rand();
		
		//set testvalue to decimal part of temp/100 to get value between 0.0 and 0.99
		testvalue = double(temp)/100 - temp/100;
		
		//perform test to determine whether to make tree a stump
		if(testvalue < pval) result = true;
		
		return result;		
	}
	
	void CutBlock::resurrecttree(PlotTree* deadtree) {
		deadtree->set_mortclass(1);
	}		
	
	void CutBlock::restoretree(PlotTree* deadtree) {
		int diamclass = deadtree->get_stemclass();
		
		//BA in meters square per hectare
		double basalarea = tree_basalarea(deadtree)/plotarea; 
		
		//resurrect dead tree
		deadtree->set_mortclass(1);

		//reclassify size class as -1 to prevent future cutting
		deadtree->set_stemclass(-1);
		
		//subtract dead tree's basal area from the current basal area sum
		//for this size class
		if(diamclass > 0) { //only subtract BA if tree is in merchantable class
			if(basalarea_tracker[diamclass] >= basalarea) {
				basalarea_tracker[diamclass] -= basalarea;
				BAtotalharvested -= basalarea;
			}
		}
	}
	
	double CutBlock::tree_basalarea(PlotTree& target) {
		//stem diameter is given in cm
		//divide square cm by 10000 to get basal area per square meter
		double radius = target.get_stemdiameter() /2;
		//stem area in square cm
		double stemarea = radius * radius * PI;
		//convert to square meters
		return stemarea/10000;
	}
	
	double CutBlock::tree_basalarea(PlotTree* target) {
		//stem diameter of a single tree is measured in cm
		//there are 10,000 square cm in 1.0 square meter
		//divide basal area per square cm by 10,000 to get basal area per square meter
		
		//calculate radius length from diameter (in cm)
		double radius = target->get_stemdiameter() /2;
		
		//calculate stem basal area in square cm
		double stemarea = radius * radius * PI;
		
		//convert to square meters and return
		return stemarea/10000;
	}
	
	double CutBlock::group_basalarea_perha(int category) {
		//stem diameter of a single tree is measured in cm
		//there are 10,000 square cm in 1.0 square meter
		//divide basal area per square cm by 10,000 to get basal area per square meter
		//divide basal area per square meter by plot area in hectares to get
		//basal area in square meters per ha
		
		if(errorlog.length() > 0) {
			ofstream fout;	
			fout.open(errorlog.c_str(), ios::app);
			fout << "\ngroup_basalarea_perha() function called";
			fout.close();
		}
		
		vector<PlotTreeNode*>::iterator p;
		double radius;
		double stemarea(0);
		
		//sum the basal area of *live* trees across size classes
		//mortality classes < 2 are considered to be living
		//note: mortclass values of trees in treemap are updated 
		//during a harvest via pointers in the PT_hashtable
		if(sizeclassindices[category] > -1) {
			for(p = PT_hashtable.begin(); p != PT_hashtable.end(); ++p) {
				
				if( (*p)->getPTptr()->get_stemclass() == category && (*p)->getPTptr()->get_mortclass() < 2) {
					//calculate total basal area of live trees in square meters
					stemarea += tree_basalarea( *(*p)->getPTptr() );
				}
			}
			//convert ba to square meters per ha
			if(plotarea > 0)
				return stemarea/plotarea;
			else {
				cout << "\nERROR (group_basalarea_perha): plotarea not greater than 0\n";
				exit(1);
			}
		}
		else 
			return 0;
	}
	
	void CutBlock::fill_hastable()
	{
		if(errorlog.length() > 0) {
			ofstream fout;	
			fout.open(errorlog.c_str(), ios::app);
			fout << "\nfill_hashtable() function called";
			fout.close();
		}
		
		int i;
		PlotTree* ptr2target = NULL;		
		PlotTreeNode* ptr2PTN = NULL;
		
		//make sure PT_hashtable has no existing data
		PT_hashtable.clear();
		
		//make pointers in hash table point to objects in treemap,
		for(i = 0; i < treemap.size(); ++i) {
			ptr2target = &treemap[i];
			ptr2PTN = new PlotTreeNode(ptr2target, NULL);
			PT_hashtable.push_back(ptr2PTN);
		}
	}
	
	void CutBlock::find_recippairs() {
		int i, j, size( PT_hashtable.size() );
		PlotTree *first, *nboroffirst;
		
		for(i = 0; i < size; i++) 
		{
			first = PT_hashtable[i]->getPTptr();
			nboroffirst = PT_hashtable[i]->getLink()->getPTptr();
			for(j = 0; j < size; j++) 
			{
				if( PT_hashtable[j]->getPTptr() == nboroffirst ) 
					recipnbor_IDs.push_back(PT_hashtable[i]->getPTptr()->get_identity() ); 
			}
		}								
	}
	
	double CutBlock::max_nndist() {
		double NNdistance, max_NNdistance(0);
		vector<PlotTreeNode*>::iterator PTNptr;
		
		//Determine maximum nearest neighbor distance of trees within plot core.
		for(PTNptr = PT_hashtable.begin(); PTNptr != PT_hashtable.end(); ++PTNptr) {
			
			NNdistance = (*PTNptr)->getPTptr()->get_nndist();
			
			if(NNdistance > max_NNdistance) {
				max_NNdistance = NNdistance;
			}
		}
		return max_NNdistance;
	}
		
	void CutBlock::link_neighbors() {
		//assumes that PT_hashtable has been filled
		//calling this function also sets values of 
		//nearest neighbor distances for all elements
		
		double nndistance;
		PlotTree* nbor = NULL;
		PlotTreeNode* ptr2nbor = NULL;
		vector<PlotTreeNode*>::iterator PTNptr;
		
		//link first elements of PT-hashtable to their nearest neighbors 
		for(PTNptr = PT_hashtable.begin(); PTNptr != PT_hashtable.end(); ++PTNptr) {
			
			//locate nearest tree in plot 
			nbor = findneighbor((*PTNptr)->getPTptr() );
			
			//set neighbor link to nearest tree in plot
			ptr2nbor = new PlotTreeNode(nbor, NULL);
			(*PTNptr)->setLink(ptr2nbor);
			
			//update nearest neighbor distance
			nndistance = dist(*(*PTNptr)->getPTptr(), *nbor);
			(*PTNptr)->getPTptr()->set_nndist(nndistance);
		}	
	}

	PlotTree* CutBlock::findneighbor(PlotTree* targettree) {
		// Assumes that first nodes of PT_hashtable have been assigned.
		int i, pos;
		double mindistance(9999);
		double distance;
		PlotTree* nexttree = NULL;
		
		//Search for neighbors within plot core
		//Compare distance to all trees in core to find nearest neighbor
		for(i = 0; i < PT_hashtable.size(); i++) {
			
			//point corenbor to next element in hashtable
			nexttree = PT_hashtable[i]->getPTptr();
			
			//prevent self-comparison
			if(targettree != nexttree) {
				
				//compute distance between target and element \
				to find index of nearest neighbor
				distance = dist(*targettree, *nexttree);
				if(distance > 0 && distance < mindistance) {
					mindistance = distance;
					pos = i;
				}
			}
		}
		
		//assign nexttree to nearest neighbor 
		nexttree = PT_hashtable[pos]->getPTptr();
		
		return nexttree;
	}
		
	void CutBlock::singularitytest(vector<double> values, const string type) {
		vector<double>::iterator p;
		int index(1);
		
		for(p = values.begin(); p != values.end(); ++p) {
			if(*p == 1.0) {
				cout << "\nNOTICE: based on input data, ALL trees in "  
				<< type << " class " << index << " will be cut";
			}
			if(*p == 0.0) {
				cout << "\nNOTICE: based on input data, NO trees in "  
				<< type << " class " << index << " will be cut";
			}
			index++;
		}
	}
	
	void CutBlock::openfile(ifstream& inputstream, string filename) {
		
		inputstream.open(filename.c_str() );
		if(inputstream.fail() ) {
			cout << "\nERROR: can not open file " << filename << endl;
			exit(1);
		}
	}
	
	void CutBlock::openoutput(ofstream& outputstream, string filename) {
		
		outputstream.open(filename.c_str() );
		if(outputstream.fail() ) {
			cout << "\nERROR: can not open file " << filename << endl;
			exit(1);
		}
	}
	
	int CutBlock::calc_stemclass(double diam) {
		int i, stemclass;
		
		if(diam <= 0) {
			cout << "\nERROR (calc_stemclass): diameter <= 0\n" << flush;
			exit(1);
		}
		
		//zero size class includes all trees smaller than size class #1
		if(diam < sizeclasses[1]) {
			stemclass = 0;
		}
		else if(diam >= sizeclasses[1] && diam < sizeclasses[ sizeclasses.size() -1 ]) {
			for(i = 1; i < sizeclasses.size() -1; i++) {
				if(diam >= sizeclasses[i] && diam < sizeclasses[i+1]) {
					stemclass = i;
				}
			}
		}
		else {
			stemclass = sizeclasses.size() -1;
		}
		
		//REMOVE WHEN BUG FIXED
		if(stemclass == 0 || stemclass > 5 || stemclass == -1) {
			cout << "\nSTEMCLASS CALC ERROR: diam = " << diam << endl 
				<< "stemclass = " << stemclass << endl << flush;
			exit(1);
		}
		
		return stemclass;
	}
	
	void CutBlock::extract_sizeclassfile(string file) {
		ifstream fin;
		int i, filesize;
		double tempdbl;
		string filecode;
		
		//remove any existing data
		sizeclasses.clear();
		
		//add sizeclass data
		openfile(fin, file);
		fin >> filecode;
		if(filecode == "default") {
			set_defaultclasssizes();
			cout << "\n\tSIZE CLASS FILE: default";
		}
		else {
			fin >> filesize;
			for(i = 0; i < filesize; i++) {
				fin >> tempdbl;
				sizeclasses.push_back(tempdbl);
			}
			cout << "\n\tSIZE CLASS FILE: " << filecode;
		}
		fin.close();
	}
				
	void CutBlock::extract_treedatafile(string file) {
		int count, stemcls, dummy;
		string header;
		ifstream fin1, fin2;
		
		int oneline(100);  //used by getline fct
		char txtline[100]; //used by getline fct
		
		//PlotTree variables
		PlotTree* nexttree = NULL;
		int datasize(0), speciesn, mortcls, sortietype;
		double dbh, Xval, Yval, ht;
		
		//connect to tree data file and extract PlotTree data
		openfile(fin1, file);
		
		//Count number of data lines in input file
		fin1 >> currentstep >> totalstep;
		while (!fin1.eof()) {
			fin1.getline(txtline, oneline);
			if (strlen(txtline) > 1)   //check for empty line
				datasize++;
		}
		fin1.close();
		
		//subtract header line from datasize
		datasize -= 1;

		//remove any existing treemap data and set vector capacity 
		treemap.clear();
		treemap.reserve(datasize);
		
		//reconnect to tree data file 
		openfile(fin2, file);
		
		//extract tree data
		count = 0;
		fin2 >> dummy >> dummy;  //strip off time step values (we already have them)
		fin2 >> header >> header >> header >> header >> header >> header; //strip column headings

		while(count < datasize) {
			//get one data line
			fin2 >> Xval >> Yval >> speciesn >> sortietype >> dbh >> ht;

			if(is_spexception(speciesn) == false) {
				
				//determine stem size class
				stemcls = calc_stemclass(dbh);
				if(stemcls == 0 || stemcls == -1) {
					cout << "\nERROR (extract_treedatafile): bad stemclass value (" << stemcls << ") "
					<< "\nDBH = " << dbh << flush;
					exit(1);
				}
				
				//convert mortality class if tree is alive
				//SORTIE uses different "tree type" values for different dbh classes
				//SORTIE tree types 1-3 are alive, 4 = stump, 5 = snag
				//here, we use 1 for alive, regardless of dbh
				if(sortietype < 4) mortcls = 1;
				if(sortietype == 4) mortcls = 2;
				if(sortietype == 5) mortcls = 3;
				else mortcls = 1;
				
				//initialize PlotTree object
				nexttree = new PlotTree(speciesn, dbh, Xval, Yval, ht, stemcls, 
						        mortcls, sortietype);
				
				//assign a value to identity number
				nexttree->set_identity(count +1);
				
				//add tree to treemap
				treemap.push_back(*nexttree);
			}
			count++;
		}
		fin2.close();
		
		cout << "\n\tTREE DATA FILE: " << file
		<< endl << "\t\t" << datasize << " trees read from file\n" << flush;
		
		delete nexttree;
	}
	
	void CutBlock::extract_probcutfile(string file) {
		ifstream fin;
		int i, filesize;
		double tempdbl;
		
		//remove any existing data
		probcuttarget.clear();
		
		//add target cut probabilities
		openfile(fin, file);
		fin >> filesize;
		
		if(filesize != sizeclasses.size() ) {
			cout << "\nWARNING: size of target probabilities file " << file 
			<< " does not match size of sizeclasses\n";
			exit(1);
		}
		else {
			for(i = 0; i < filesize; i++) {
				fin >> tempdbl;
				probcuttarget.push_back(tempdbl);
			}
		}
		fin.close();
		cout << "\n\tTARGET PROB FILE: " << file << flush;
	}
	
	void CutBlock::extract_nborcutfile(string file) {
		ifstream fin;
		int i, filesize;
		double tempdbl;
		
		//remove any existing data
		probcutnbor.clear();
		
		//add neighbor cut probabilities
		openfile(fin, file);
		fin >> filesize;
		
		if(filesize != sizeclasses.size() ) {
			cout << "\nWARNING: size of neighbor probabilities file " << file 
			<< " does not match size of sizeclasses\n";
			exit(1);
		}
		else {
			for(i = 0; i < filesize; i++) {
				fin >> tempdbl;
				probcutnbor.push_back(tempdbl);
			}
		}
		fin.close();
		cout << "\n\tNEIGHBOR PROB FILE: " << file << flush;
	}
	
	void CutBlock::extract_plotcornersfile(string file) {
		ifstream fin;
		int i;
		double tempdbl;
		
		//remove any existing data
		plotcorners.clear();
		
		//get plot corners
		openfile(fin, file);

		for(i = 0; i < 4; i++) {
				fin >> tempdbl;
				plotcorners.push_back(tempdbl);
		}
		
		fin.close();
		cout << "\n\tPLOT CORNERS FILE: " << file << flush;
	}
	
	void CutBlock::extract_timesteptable(string file) {
		ifstream fin;
		int i, tmp, filesize;
		
		//connect to file
		openfile(fin, file);
		
		fin >> filesize;
		for(i = 0; i < filesize; ++i) {
			fin >> tmp;
			timetable.push_back(tmp);
		}
		fin.close();
		cout << "\n\tTIME STEP FILE: " << file << flush;
	}
	
	
	void CutBlock::set_rand() {
		double temp1, temp2, temp3;
		int rseed;
		//Set up cstdlib random number generator
		//first, get current time
		currenttime = (time(NULL));
		temp1 = double(currenttime)/10000;
		temp2 = (temp1 - currenttime/10000) *10000;
		
		rseed = int(temp2);
		//next, reseed generator using current time
		srand(rseed);
		
		cout << "\nRand Seed: " << rseed;
	}
	
	void CutBlock::getr_harvesttargets(string filename, vector<double>& values) {
		ifstream fin;
		int i, filesize;
		double tmp;
		
		openfile(fin, filename);
		fin >> filesize;
		if(filesize != sizeclasses.size() ) {
			cout << "\nERROR: harvest targets file " << filename 
			<< " not same size as sizeclasses (" << sizeclasses.size() << ")\n";
			exit(1);
		}
		else {
			for(i = 0; i < sizeclasses.size(); i++) {
				fin >> tmp;
				values.push_back(tmp);
			}
		}
	}
	
	void CutBlock::getr_harvesttargets(string filename, vector<int>& values) {
		ifstream fin;
		int i, filesize;
		int tmp;
		
		openfile(fin, filename);
		fin >> filesize;
		if(filesize != sizeclasses.size() ) {
			cout << "\nERROR: harvest targets file " << filename 
			<< " not same size as sizeclasses (" << sizeclasses.size() << ")\n";
			exit(1);
		}
		else {
			for(i = 0; i < sizeclasses.size(); i++) {
				fin >> tmp;
				values.push_back(tmp);
			}
		}
	}
	
	void CutBlock::set_classbounds() {
		//sets bounds for all but the zero class, whose bound is is size of hashtable -1
		//assumes treemap is reverse sorted by stem diameter
		//note: smallest tree in PT_hashtable set when table was initialized
		
		if(errorlog.length() > 0) {
			ofstream fout;	
			fout.open(errorlog.c_str(), ios::app);
			fout << "\nset_classbounds() function called";
			fout.close();
		}

		int i, startindex, nextindex;
		
		//set up vector to accept values
		sizeclassindices.clear();  
		
		//set initial values to -1, indicating no trees in class
		for(i = 0; i < sizeclasses.size(); i++) {
			sizeclassindices.push_back(-1);
		}
		
		//set bound of zero class 
		//if no trees exist in zero class, set index to -1
		if(PT_hashtable[PT_hashtable.size() -1]->getPTptr()->get_stemdiameter() >= sizeclasses[1]) {
			sizeclassindices[0] = -1;
		}
		//othewise, set value to highest index in PT_hashtable (smallest tree in map)
		else if(PT_hashtable[PT_hashtable.size() -1]->getPTptr()->get_stemdiameter() < sizeclasses[1]) {
		sizeclassindices[0] = PT_hashtable.size() -1;
		}
		
		//start with largest size class, end with next-to-smallest class
		startindex = 0;
		nextindex = 0;
		for(i = sizeclasses.size() -1; i > 0; i--) {
			
			//index = 0;
			//value at sizeclasses[i] is lower bound for class
			//NOTE at end of loop, index is outside range of PT_hashtable
			while(nextindex < PT_hashtable.size()  
				  && PT_hashtable[nextindex]->getPTptr()->get_stemdiameter() >= sizeclasses[i]) {
				
				nextindex++;
			}
			
			//If no trees in PT_hashtable are larger than one of the class
			//boundaries, then the index will not increment.
			//If all of the trees in PT_hashtable are larger than one of the
			//class boundaries, then index will equal PT_hashtable.size, and
			//all remaining size classes will have the same bounds value of 
			//PT_hashtable.size
			
			//if nextindex did not move, then there are no trees in this class
			if(nextindex == startindex) { sizeclassindices[i] = -1; }
			
			//if index moved and is somewhere in the middle of tree map, we found the lower bound
			else if(nextindex > startindex) {
				if(nextindex <= PT_hashtable.size() ) {
					sizeclassindices[i] = nextindex -1;
				}
			}
			else if(nextindex < startindex) {
				cout << "\nERROR (set_classbounds): start < next";
				exit(1);
			}
			
			//update start location
			startindex = nextindex;
		}
	}
	
	void CutBlock::calc_classcounts() {
		
		if(errorlog.length() > 0) {
			ofstream fout;	
			fout.open(errorlog.c_str(), ios::app);
			fout << "\ncalc_classcounts() function called";
			fout.close();
		}
		
		//assumes stems are sorted in reverse order by diameter
		int i, index(0), top(0), bottom(0);
		
		//set up vector to accept values
		classcounts.clear();  classcounts.resize(sizeclasses.size());
		
		for(i = sizeclasses.size() -1; i > -1; i--) 
		{
			//value at sizeclasses[i] is lower bound for class
			while(index < PT_hashtable.size() 
				  && PT_hashtable[index]->getPTptr()->get_stemdiameter() >= sizeclasses[i] ) {
				index++;
			}
			
			if(index > 0) {
				bottom = index -1;
				classcounts[i] = bottom - top;
			}
			else classcounts[i] = 0;
			top = bottom;
		}
	}	
	
	bool CutBlock::is_spexception(int n) {
		int count(0);
		while(count < exception_species.size() && n != exception_species[count] ) {
			count++;
		}
		if(count == exception_species.size() ) return false;
		else return true;
	}
	
	int CutBlock::count_protected() {
		int count(0);
		double n_trees;
		//if tree protection requested, set count to number of largest trees to be protected
		count = 0;
		n_trees = fractionlargestretained * double(classcounts[classcounts.size() -1]);
		while(count < n_trees) { count++; }
		count--;
		
		return count;
	}
	
	double CutBlock::set_trees2protect(int num2protect) {
		int i;
		double BAofpreserved;
		vector<PlotTree*> trees2protect(num2protect);
		vector<PlotTreeNode*> neighbors;
		
		//calculate basal area represented by preserved trees (m2/ha)
		//and get pointers to preserved trees
		if(plotarea > 0) {
			for(i = 0; i < num2protect; i++) {
				BAofpreserved += tree_basalarea(treemap[i])/plotarea;
				trees2protect[i] = PT_hashtable[i]->getPTptr();
			}
		}
		else {
			cout << "\nERROR (set_trees2protect): plotarea not greater than 0\n";
			exit(1);
		}
		
		//find hashtable elements whose links point to one of the preserved trees
		//and destroy their links (uses delete operator)
		find_neighborsofprotected(trees2protect, neighbors); 
		
		//remove preserved trees from hashtable
		//assumes hashtable elements have been reverse sorted by tree size
		PT_hashtable.erase( PT_hashtable.begin(), PT_hashtable.begin() + num2protect );
		
		//create new links for hashtable elements whose deleted links used to point to 
		//one of the preserved trees. Link them so that they now point to their next-nearest neighbor.
		//(Their first nearest neighbors are the protected trees)
		set_group_links(neighbors);

		return BAofpreserved;
	}
		
	void CutBlock::find_neighborsofprotected(vector<PlotTree*> protectedtrees, 
											 vector<PlotTreeNode*>& neighborsofprotected) { 
		int i, j, discardcount(0);
		PlotTreeNode* discard = NULL;
		PlotTreeNode* newneighbor = NULL;
		
		for(i = 0; i < PT_hashtable.size(); i++) {
			for(j = 0; j < protectedtrees.size(); j++) {
				//if neighbor of this tree is one of the preserved trees
				if( PT_hashtable[i]->getLink()->getPTptr() == protectedtrees[j]) {
					
					//destroy the link to the preserved tree
					discard = PT_hashtable[i]->getLink();
					delete discard;
					discardcount++;
					
					//add this tree to the list of trees whose neighbors is a preserved tree
					newneighbor = new PlotTreeNode(PT_hashtable[i]->getPTptr(), NULL);
					neighborsofprotected.push_back(newneighbor);
				}
			}
		}
	}
	
	void CutBlock::set_group_links(vector<PlotTreeNode*>& group) {
		//Used to update neighbor links when certain trees have been removed \
		from PT_hashtable
		
		if(errorlog.length() > 0) {
			ofstream fout;	
			fout.open(errorlog.c_str(), ios::app);
			fout << "\nset_group_links() function called";
			fout.close();
		}
		
		int i;
		double distance;
		PlotTree* neighbor = NULL;
		PlotTreeNode* PTNlink = NULL;
		
		for(i = 0; i < group.size(); i++) {
			
			//point secondneighbor to second nearest neighbor of this tree
			//find current nearest neighbor in core of plot
			neighbor = findneighbor(group[i]->getPTptr() );
			
			//calculate distances to nearest neighbors
			distance = dist(*group[i]->getPTptr(), *neighbor);
			
			//set link of core tree to point to second nearest neighbor
			PTNlink = new PlotTreeNode(neighbor, NULL);
			group[i]->setLink(PTNlink);
		}
	}
	
	void CutBlock::BAharvest_byrandindiv() {
		//Ignores neighbors
		//Classboundaries contains the indices that mark the lower bound (tree diameter)
		//of each size class in the PT_hashtable
		
		if(errorlog.length() > 0) {
			ofstream fout;	
			fout.open(errorlog.c_str(), ios::app);
			fout << "\nBAharvest_byrandindiv() function called";
			fout.close();
		}
		
		int i, randindex, sizeclass, classindex, top, bottom, targetsmetsum(0);
		int numsizeclasses = sizeclasses.size();
		double maxBA2harvest(0);
		bool test(false);
		PlotTree* ptr = NULL; PlotTree* nbor = NULL;
		
		int targetsmet[numsizeclasses];
		double classBA[numsizeclasses];
		
		//initialization. Set value to 1 if no trees in class
		//start with first merchantable class (1)
		for(i = 1; i < numsizeclasses; i++) { 
			if(sizeclassindices[i] > -1) {
				targetsmet[i] = 0; 
			}
			else {
				targetsmet[i] = 1;
			}
		}
		//set value for zero class to zero so it won't affect sum of total, which is a flag
		targetsmet[0] = 0;
		
		//initialization. Set value to zero if no trees in class
		for(i = 0; i < numsizeclasses; i++) { 
			if(sizeclassindices[i] > -1) {
					classBA[i] = group_basalarea_perha(i);
			}
			else {
				classBA[i] = 0;
			}
		}
		
		//calculate max total basal area allowed to cut
		//equals (total basal area of harvestable size classes) * (maximum allowed percent of total to cut)
		//first, sum the group basal areas for each size class
		for(i = 0; i < numsizeclasses; i++) { maxBA2harvest += classBA[i]; }
		
		//next, multiply the sum by the percent of total allowed to harvest
		maxBA2harvest *= maxtotalharvestpercent;
		
		//continue harvest until maxBA2harvest is achieved or
		// harvest goals for every class have been achieved
		BAtotalharvested = 0;
		while(BAtotalharvested < maxBA2harvest && targetsmetsum < nonzero_count)
		{
			//PT_hashtable contains pointers to trees in treemap and 
			//pointers to each tree's immediate neighbor.
			
			//choose a size class at random and set values of top and bottom indices in hashtable
			classindex = choose_randsizeclass(top, bottom, targetsmet);

			//drop out of while loop if all class targets have been achieved
			//flag = -1
			if(classindex == -1) break;
			
			//get random number limited to the range of indices represented by trees in this class
			if(bottom == top) {
				cout << "\nzero data error: top " << top << "\tbot " << bottom 
					<< "\tclass " << classindex << endl << flush;
				exit(1);
			}
			//bottom is high value, top is low value
			else randindex = (rand() % ( (bottom +1) - top) ) + top;
			
			//Get pointer to a random tree within this class, and calculate its basal area in m2 per ha
			ptr = PT_hashtable[randindex]->getPTptr();
			sizeclass = ptr->get_stemclass();
			
			//if both the target tree pointed to by ptr and it's nearest neighbor are alive, and
			//if harvest goals have not yet been achieved for target, then
			//test whether to cut target; otherwise, move on to next target
			if(ptr->get_mortclass() == 1 && targetsmet[sizeclass -1] == 0)
				test = cuttarget(ptr);
			else {
				test = false;
			}
			
			//If target is cut, add its basal area to appropriate tracker class 
			if(test == true) {
				//add target basal area to basalarea_tracker[this class] and to BAtotalharvested
				addto_basalarearemoved(ptr);
				
				//update status of harvest target amount for class of target tree
				if(basalarea_tracker[sizeclass] > basalareatargets[sizeclass]) {
					targetsmet[sizeclass] = 1;
				}
			}
			
			//check status of harvest targets
			targetsmetsum = 0;
			for(i = 0; i < numsizeclasses; i++) targetsmetsum += targetsmet[i];
			
		} // end while
		
		cout << "\nmaxBA2harvest " << maxBA2harvest << "\tBAtotalharvested " << BAtotalharvested 
		<< "\nCl\tBAgp\tBAharv\tPercent of group\n" << flush;
		for(i = 0; i < numsizeclasses; i++) {
			cout << i << "\t" << setprecision(2) << classBA[i] << "\t"
			<< basalarea_tracker[i] << "\t";
			if(classBA[i] > 0)
				cout << basalarea_tracker[i]/classBA[i] *100 << endl << flush;
			else cout << 0 << endl << flush;
		}
	}
	
	void CutBlock::propagationcut_BA() {
		//Cuts neighbors with random probability
		//Classboundaries contains the indices that mark the lower bound (tree diameter)
		//of each size class in the PT_hashtable
		
		if(errorlog.length() > 0) {
			ofstream fout;	
			fout.open(errorlog.c_str(), ios::app);
			fout << "\npropagationcut_BA() function called";
			fout.close();
		}
		
		int i, randindex, sizeclass, classindex, top, bottom, targetsmetsum(0);
		int numsizeclasses = sizeclasses.size();
		double maxBA2harvest(0);
		bool test(false);
		PlotTree* ptr = NULL; PlotTree* nbor = NULL;
		
		//FLAGS
		int twenty(0), forty(0), sixty(0), eighty(0);
		
		int targetsmet[numsizeclasses];
		double classBA[numsizeclasses];
		
		//initialization
		cout << "\ninitializing vectors" << flush;
		//set value for zero class to zero so it won't affect sum of total, which is a flag
		targetsmet[0] = 0;
		
		//set values for remaining classes
		//Set value to 1 if no trees in class
		for(i = 1; i < numsizeclasses; i++) { 
			if(sizeclassindices[i] > -1) {
				targetsmet[i] = 0; 
			}
			else {
				targetsmet[i] = 1;
			}
		}
		
		//initialization. Set value to zero if no trees in class
		for(i = 0; i < numsizeclasses; i++) { 
			if(sizeclassindices[i] > -1) {
				classBA[i] = group_basalarea_perha(i);
			}
			else {
				classBA[i] = 0;
			}
		}
		
		//calculate max total basal area allowed to cut
		//equals (total basal area of harvestable size classes) * (maximum allowed percent of total to cut)
		//first, sum the group basal areas for each merchantable size class (begin with class 1, not zero)
		for(i = 0; i < numsizeclasses; i++) { maxBA2harvest += classBA[i]; }
		
		//next, multiply the sum by the percent of total allowed to harvest
		maxBA2harvest *= maxtotalharvestpercent;
		
		cout << "\nmax harv: " << setprecision(2) << maxBA2harvest << " per ha" << endl;
		
		//continue harvest until maxBA2harvest is achieved or
		// harvest goals for every class have been achieved
		
		BAtotalharvested = 0;
		while(BAtotalharvested < maxBA2harvest && targetsmetsum < nonzero_count)
		{
			//PT_hashtable contains pointers to trees in treemap and 
			//pointers to each tree's immediate neighbor.
			
			//show percent harvested at 20 percent intervals
			if(BAtotalharvested/maxBA2harvest >= 0.20 && twenty < 1)
			{ cout << "\n20 percent harvested"; twenty = 1; }
			if(BAtotalharvested/maxBA2harvest >= 0.40 && forty < 1)
			{ cout << "\n40 percent harvested"; forty = 1; }
			if(BAtotalharvested/maxBA2harvest >= 0.60 && sixty < 1)
			{ cout << "\n60 percent harvested"; sixty = 1;}
			if(BAtotalharvested/maxBA2harvest >= 0.80 && eighty < 1)
			{ cout << "\n80 percent harvested\n"; eighty = 1; }
				
			//get new random class index
			classindex = choose_randsizeclass(top, bottom, targetsmet);
			
			//drop out of while loop if all class targets have been achieved
			if(classindex == -1) {
				cout << "\nALL TARGEST MET" << flush;
				break;
			}
			//get random number limited to the range of indices represented by trees in this class
			if(bottom == top) {
				cout << "\nzero data error: top " << top << "\tbot " << bottom << endl << flush;
				exit(1);
			}
			//bottom is high value, top is low value
			else randindex = (rand() % (bottom -top)) + top;
			
			//Get pointer to a random tree within this class, and calculate its basal area in m2 per ha
			ptr = PT_hashtable[randindex]->getPTptr();
			nbor = PT_hashtable[randindex]->getLink()->getPTptr();	
			sizeclass = ptr->get_stemclass();
						
			//if both the target tree pointed to by ptr and it's nearest neighbor are alive, and
			//if harvest goals have not yet been achieved for target, then
			//test whether to cut target; otherwise, move on to next target
			//stem class = -1 if previously resurrected
			if(
			   ptr->get_mortclass() == 1 
			   && nbor->get_mortclass() == 1 
			   && targetsmet[sizeclass] == 0
			   && ptr->get_stemclass() > 0
				)
			{ test = cuttarget(ptr); }
			else 
			{
				test = false;
			}
			if(randindex < top || randindex > bottom) 
			{
				cout << "\nERROR: (propcut) randindex out of bounds. randindex = " << randindex
					 << "\ntop = " << top << "\tbot = " << bottom << endl;
				exit(1);
			}
			
			//If target is cut, add its basal area to appropriate tracker class and
			//test whether to cut it's first neighbor
			if(test == true) {
				
				//update count of number of trees cut
				if(test == true) { targetscut++; }
				
				//add target basal area to basalarea_tracker[this class] and to BAtotalharvested
				addto_basalarearemoved(ptr);
				
				//update status of harvest target amount for class of target tree
				if(basalarea_tracker[sizeclass] > basalareatargets[sizeclass]) {
					targetsmet[sizeclass] = 1;
				}
				
				//test whether to cut first nbor
				//Note: we tested whether first nbor was alive when we tested target, above,
				//so there is no need to retest mortality class of first nbor.
				//Also, cutting of neighbors occurs regardless of status of class harvest target.
				//Note: cutnbor checks stemclass of nbor.
				test = cutnbor(nbor);

				//if first neighbor is cut, add its basal area to amount cut and
				//test whether to cut second neighbor, etc.
				//In the while loop below, we point the PlotTree pointer, nbor, to neighbor of nbor
				if(test == true) {
					if(test == true) { nborscut++; }
					propagate_nborcut(nbor, targetsmet);
				}
			}
			
			//check status of harvest targets
			targetsmetsum = 0;
			for(i = 0; i < numsizeclasses; i++) targetsmetsum += targetsmet[i];

		} // end while
		
		//calculate average number of neighbors cut
		if(targetscut > 0) { ave_nborscut = double(nborscut)/targetscut; }
		else { ave_nborscut = 0; }
		
		cout << "100 percent harvested\n"
			<< "\nmaxBA2harvest " << maxBA2harvest << "\tBAtotalharvested " << BAtotalharvested 
			<< "\ntarget trees cut " << targetscut << "\taverage nbors cut " << ave_nborscut << endl
			<< "\nCl\tBAgp\tBAharv\tPercent of group\n" << flush;
		for(i = 0; i < numsizeclasses; i++) {
			cout << i << "\t" << setprecision(2) 
				<< classBA[i] << "\t"
				<< basalarea_tracker[i] << "\t";
			if(classBA[i] > 0)
				cout << basalarea_tracker[i]/classBA[i] *100 << endl << flush;
			else cout << 0 << endl << flush;
		}
	}
	
	/*********
	void CutBlock::groupcut_BA() {
		//Cuts groups of trees
		//Classboundaries contains the indices that mark the lower bound (tree diameter)
		//of each size class in the PT_hashtable
		//IMPORTANT: assumes that the stand area is large enough to support basal area targets,
		//when trees are located within non-overlapping gaps
		
		if(errorlog.length() > 0) {
			ofstream fout;	
			fout.open(errorlog.c_str(), ios::app);
			fout << "\ngroupcut_BA() function called";
			fout.close();
		}
		
		int i, randindex, sizeclass, classindex, top, bottom;
		int numsizeclasses = sizeclasses.size();
		double gap_radius;
		double maxBA2harvest(0), classBA[numsizeclasses];
		bool test(false);
		PlotTree* ptr = NULL; 
		PlotTree gapcenter;
		vector<PlotTree> gaptrees;
		
		//FLAGS
		int twenty(0), forty(0), sixty(0), eighty(0);
		
		//initialization
		cout << "\ninitializing vectors" << flush;
		
		//initialization. Set value to zero if no trees in class
		for(i = 0; i < numsizeclasses; i++) { 
			if(sizeclassindices[i] > -1) {
				classBA[i] = group_basalarea_perha(i);
			}
			else {
				classBA[i] = 0;
			}
		}
		
		//calculate max total basal area allowed to cut
		//equals (total basal area of harvestable size classes) * (maximum allowed percent of total to cut)
		//first, sum the group basal areas for each merchantable size class (begin with class 1, not zero)
		for(i = 0; i < numsizeclasses; i++) { maxBA2harvest += classBA[i]; }
		
		//next, multiply the sum by the percent of total allowed to harvest
		maxBA2harvest *= maxtotalharvestpercent;
		cout << "\nmax harv: " << setprecision(2) << maxBA2harvest << " per ha" << endl;
		
		//continue harvest until maxBA2harvest is achieved 
		
		BAtotalharvested = 0;
		while(BAtotalharvested < maxBA2harvest)
		{
			//PT_hashtable contains pointers to trees in treemap and 
			//pointers to each tree's immediate neighbor.
			
			//show percent harvested at 20 percent intervals
			if(BAtotalharvested/maxBA2harvest >= 0.20 && twenty < 1)
			{ cout << "\n20 percent harvested"; twenty = 1; }
			if(BAtotalharvested/maxBA2harvest >= 0.40 && forty < 1)
			{ cout << "\n40 percent harvested"; forty = 1; }
			if(BAtotalharvested/maxBA2harvest >= 0.60 && sixty < 1)
			{ cout << "\n60 percent harvested"; sixty = 1;}
			if(BAtotalharvested/maxBA2harvest >= 0.80 && eighty < 1)
			{ cout << "\n80 percent harvested\n"; eighty = 1; }
		
			
			//set random gap coordinates
			gapcenter.set_Xcoord( rand_Xcoord() );
			gapcenter.set_Ycoord( rand_Ycoord() );
			
			//set gap radius based upon height of tallest tree within 10m of gap center
			gap_radius = heightoflargest_at10m(gapcenter) * gap_radius_percentageoftreeheight;
			
			//if coordinates located within or adjacent to gap, reset coordinates
			//included counter to avoid infinite loop
			i = 0;
			while(is_ingap(gapcenter, gap_radius) == true
				  && i < 1000) {
				gapcenter.set_Xcoord( rand_Xcoord() );
				gapcenter.set_Ycoord( rand_Ycoord() );
				gap_radius = heightoflargest_at10m(gapcenter) * gap_radius_percentageoftreeheight;
				i++;
			}
			if(i == 1000) {
				cout << "\nERROR (groupcut_BA): plot area too small for group cut harvest"
				<< " at current BA target values\n";
				exit(1);
			}
			
			//compile list of trees in gap (trees >= 2cm DBH)
			gaptrees = get_gaptrees(gapcenter, gap_radius);
			
			//calculate basal area of gap; add to total BA removed
			gapBA = calc_sampleBA(gaptrees);
			addto_basalarearemoved(gapBA);  //overloaded fct
			
			//cut all trees in gap that are >= 2cm
			cut_gaptrees(gapcenter, gap_radius);
			
			//add gap center to vector of gap centers 
			//INCLUDE RADIUS???
			gapcenters_v.push_back(gapcenter);
			
		} // end while
				
		cout << "100 percent harvested\n"
		<< "\nmaxBA2harvest " << maxBA2harvest << "\tBAtotalharvested " << BAtotalharvested 
		<< "\ntarget trees cut " << targetscut << "\nnumber of gaps created " << gap_n
		<< "\taverage gap size cut " << ave_gapsize << endl
		<< "\nCl\tBAgp\tBAharv\tPercent of group\n" << flush;
		for(i = 0; i < numsizeclasses; i++) {
			cout << i << "\t" << setprecision(2) 
			<< classBA[i] << "\t"
			<< basalarea_tracker[i] << "\t";
			if(classBA[i] > 0)
				cout << basalarea_tracker[i]/classBA[i] *100 << endl << flush;
			else cout << 0 << endl << flush;
		}
	}
	*********/
	
	double CutBlock::calc_plotarea() {
		double minX, maxX, minY, maxY;
		double length(0), width(0);
		
		if(plotcorners.size() == 4) {
			minX = plotcorners[0];
			maxX = plotcorners[1];
			minY = plotcorners[2];
			maxY = plotcorners[3];
			
			//calculate dimensions in meters
			length = maxY - minY;   width = maxX - minX;
		}
		else {
			"\nERROR (calc_plotarea): plot corners vector contains no data\n";
			exit(1);
		}
		
		//divide by 10000 to convert to meters-square per ha
		return (length * width)/10000;
	}
	

	void CutBlock::addto_basalarearemoved(PlotTree* tree) {
		//add basal area of ptr in meters square per ha
		//tree basal area is given per square meter, but group basal area is per hectares
		//divide by tree BA by plotarea to convert to BA per hectare
		int sizeclass = tree->get_stemclass();
		double stembasalarea = tree_basalarea(tree)/plotarea;
		
		basalarea_tracker[sizeclass] += stembasalarea;
		BAtotalharvested += stembasalarea;
	}
	
		/****
	void CutBlock::addto_basalarearemoved(double BAremoved) {
		//add basal area in meters square per ha
		basalarea_tracker[sizeclass] += BAremoved;
		BAtotalharvested += BAremoved;
	}
	 
	  ***/
	
	void CutBlock::set_topandbottom(int& top_n, int& bottom_n, const int stemsizeclass) {
		//top_n and bottom_n are indices of the PT_hashtable
		//boundaries = class_bounds (lower bound of each size class)
		
		if(errorlog.length() > 0) {
			ofstream fout;	
			fout.open(errorlog.c_str(), ios::app);
			fout << "\nset_topandbottom() function called";
			fout.close();
		}
		
		//if no trees in this class, set values to -1
		if(sizeclassindices[stemsizeclass] == -1) {
			top_n= -1;
			bottom_n = -1;
		}
		
		else {
			int index(1);
			//set value for zero class
			//put top_n just above next highest non-empty class
			//value is -1 if the class is empty
			
			if(stemsizeclass == 0) {
				//find next highest non-empty class
				while(sizeclassindices[index] == -1 && index < sizeclasses.size()) {
					index++;
				}
				if(index == sizeclasses.size()) {
					cout << "\nERROR (set_topandbottom): no empty size classes\n";
					exit(1);
				}
				else {
					bottom_n = PT_hashtable.size() -1;
					top_n = sizeclassindices[index] +1;
				}
			}
			
			//top and bottom of classes > zero class:
			else if(stemsizeclass > 0) {
				
				//top and bottom mark the index range, in the PT_hashtable, of the current size class 
				bottom_n = sizeclassindices[stemsizeclass];
				
				//the index at top is one higher than the index at the bottom of next largest class
				//unless class is empty, in which case top and bottom are identical
				if(stemsizeclass < sizeclasses.size() -1) {
					top_n = sizeclassindices[stemsizeclass +1] +1;
				}
				
				//if classindex is for largest class, set top to head of PT_hashtable
				else top_n = 0;
			}
			else {
				cout << "\nERROR (set_topandbottom): stem size class < 0\n"
				<< "sizeclass = " << stemsizeclass << endl;
				exit(1);
			}
		}
	}
	
	void CutBlock::propagate_nborcut(PlotTree* neighbor, int targets_completed[]) {
		
		if(errorlog.length() > 0) {
			ofstream fout;	
			fout.open(errorlog.c_str(), ios::app);
			fout << "\npropagate_nborcut() function called";
			fout.close();
		}
		
		int index, sizeclass;
		bool test(true);
		
		//add basal area of previous nbor to total for its class
		addto_basalarearemoved(neighbor);
		
		//get size class of nbor
		sizeclass = neighbor->get_stemclass();
		
		//update status of harvest target amount for class of neighbor tree
		if(basalarea_tracker[sizeclass] > basalareatargets[sizeclass]) {
			targets_completed[sizeclass] = 1;
		}
		
		//test whether to cut neighbor of neighbor
		//index is used to prevent infinite looping
		index = 0;
		//while(test == true && index < PT_hashtable.size() ) {
		while(test == true) {
			
			//test whether to cut next nbor
			//test_nextnbor() redirects the pointer, nbor, to its immediate neighbor
			//and if nbor is cut, adds its basal area to totals for class and harvest
			//but if nbor was already dead, it is resurrected and 
			//its basal area is subtracted from totals 
			test = test_nextnbor(neighbor);
			if(test == true) { nborscut++; }
			index++;
		}
	}
	
	int CutBlock::choose_randsizeclass(int& top_n, int& bot_n, int targets_completed[]) {
		int class_index(0), index;
		char ans;
		
		if(errorlog.length() > 0) {
			ofstream fout;	
			fout.open(errorlog.c_str(), ios::app);
			fout << "\nchoose_randsizeclass() function called";
			fout.close();
		}
		
		//choose a size class at random from the merchantable classes
		//and assign it's index to classindex.
		//the value is between 1 and nonzero_count
		class_index = (rand() % nonzero_count) +1;
		
		//if size class harvest target has been met, or if there are no trees in this class,
		//choose a different class.
		//use index to prevent infinite loop; loop 100 times to be confident that
		//all or nearly all size classes have been checked
		index = 0;
		while(sizeclassindices[class_index] == -1
			  || targets_completed[class_index] == 1 
			  && index < 100 ) {
			class_index = (rand() % nonzero_count) +1;
			index++;
		}
		if(index == 100 ) { 
			class_index = -1;
			cout << "\nAll class targets met. Continue?" << flush;
			cin >> ans;
		}
		else {
			//set top and bottom to range in PT_hashtable for this size class
			set_topandbottom(top_n, bot_n, class_index);
		}
		
		return class_index;
	}
	
	PlotTree* CutBlock::get_neighbor(PlotTreeNode* target) {
		return target->getLink()->getPTptr();
	}
	
	void CutBlock::prep_treedata() {
		//assumes treemap is filled
		int i;
		int numsizeclasses = sizeclasses.size();
		
		//sort tree map by stem diameter in reverse order
		//sorting reorders the trees by size class to assist
		//later indexing during the harvest
		revsort_treemap_bydiameter();
		
		//Set up association of trees and neighbors in hash table.
		//The order of trees in hash table is identical to sorted treemap
		fill_hastable();
		
		//continue only if there are trees that meet the sizeclass criteria
		if(PT_hashtable.size() > 0) {
			
			//get indices for bottom of size classes in sorted treemap
			set_classbounds();
			
			//show class bounds
			cout << "\nSize class bounds\nClass\tLowBound\n";
			for(i = 0; i < sizeclasses.size(); i++) {
				cout << i << "\t" << sizeclassindices[i] << endl;
			}
			
			//count number of trees in each size class 
			calc_classcounts();
			
			//determine how many size classes have at least one tree.
			//requires that classcounts have been computed
			//counts only merchantable classes (index > 0)
			//if a size class contains stems, its number is placed in the nonzeroclasses vector
			nonzero_count = 0;
			nonzeroclasses.clear();
			for(i = 1; i < sizeclasses.size(); ++i) {
				if(classcounts[i] > 0) {
					nonzeroclasses.push_back(i);
					nonzero_count++;
				}
			}
		}
	}
	
	/****
	double CutBlock::rand_Xcoord() {
		double minX = plotcorners[0];
		double maxX = plotcorners[1];
		
		//extract a random number between minX and maxX
		return ( rand() % ( (maxX +1) - minX) ) + minX; 
	}
	
	double CutBlock::rand_Ycoord() {
		double minY = plotcorners[2];
		double maxY = plotcorners[3];
		
		//extract a random number between minX and maxX
		return ( rand() % ( (maxY +1) - minY) ) + minY; 
	}

	
	bool CutBlock::is_ingap(const PlotTree location, const double gap_radius) {
		int count;
		int size = gaptrees.size();
		
		count = 0;
		while( count < size
			   && dist(location, gapcenters_v[count]) > gap_radius) {
			  count++;
		}
		
		if(count < size) { return false; }
		else { return true; }
	}
	 	
	
	void CutBlock::cut_gaptrees(const PlotTree centerofgap, const double radius) {
		vector<PlotTreeNode*>::iterator p;
		
		//kill all trees inside gap
		for(p = PT_hashtable.begin(); p != PT_hashtable.end(); ++p) {
			if( dist( (*p)->getPTptr(), centerofgap) <= radius ) {
				(*p)->getPTptr()->set_mortclass(2);
			}
		}
	}
	 
	
	
	double CutBlock::calc_sampleBA(vector<PlotTree> sample) {
		double stemarea(0);
		vector<PlotTree>::iterator p;
		
		//sum the basal area of all trees in sample
		for(p = sample.begin(); p != sample.end(); ++p) {
			//calculate total basal area of live trees in square meters
			stemarea += tree_basalarea( (*p)->getPTptr() );
		}
		//convert ba to square meters per ha
		if(plotarea > 0)
			return stemarea/plotarea;
		else {
			cout << "\nERROR (calc_sampleBA): plotarea not greater than 0\n";
			exit(1);
		}
	}
	 
	  ***/
	
} //MMFdata

#endif
	



