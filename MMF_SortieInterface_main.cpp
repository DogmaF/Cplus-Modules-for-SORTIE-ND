/*****************************************************************************************************
 * MMF_SortieInterface_main.cpp
 * Mike Fuller July 11, 2008
 *
 *	PURPOSE
 *		This is the 'main' file for implementing class CutBlock for simulating 
 *		tree harvest methods in SORTIE. See CutBlock.h for details on methods.
 *
 *	FILE DEPENDENCIES
 *		This main file depends on several external files, which must be available 
 *		to the compiler:
 *			* CutBlock.h	(function interfaces for CutBlock class)
 *			* CutBlock.cpp	(function definitions for CutBlock class)
 *			* PlotTree.h	(function interfaces for PlotTree class)
 *			* PlotTree.cpp	(function definitions for PlotTree class)
 *
 *	EXTERNAL PARAMETER AND INPUT FILES
 *		During a simulation run, the program will look for the following files in the
 *		current directory:
 *			* cbparameters.txt	 (lists type of harvest and names of data and paramter files)
 *			* cbBAtargets.txt	 (lists targets for basal area to retain)
 *			* cbPHtargets.txt	 (lists targets for count per hectare to retain)
 *			* cbDISTtargets.txt	 (lists targets for stem size frequency distribution to retain)
 *
 *		In addition, names for following input files must be listed by the file 
 *		cbparameters.txt:
 *			* treedatafile		 (contains data for individual trees (long list of trees)
 *						 (default name: 
 *			* sizeclassfile		 (lists lower stem size limits for each size class)
 *			* probcutfile		 (lists cut probabilities for each stem size class)
 *			* probnborfile		 (lists cut probabilities for neighbors for each stem size class)
 *
 *		Examples for all of the above data and parameter files are provided in the file:
 *			* "MMF_SortieInterface Examples.txt"
 *
 *	EXTERNAL OUTPUT FILES
 *		Generates two ouputfiles:
 *			* cbSORTIEdata.txt		(file used by SORTIE as input data; trees to kill)
 *			* MMF_harvest_cutrecord.txt	(record of the amounts harvested per class)
 *
 *	IMPORTANT: the output file name must include the full path to the directory where SORTIE expects to
 *		   find the file.
 *
 *	STATUS AS OF JULY 11, 2008:
 *	Work in progress. The following harvest methods are operational:
 *		* clearcut		(cuts everything)
 *		* randomcut		(cuts trees at random across selected size classes until total target met)
 *		* basalarea		(for use with basal area retention targets per size class)
 *	
 *	The following methods will be implemented at a later date:
 *		* counts		(cut targets = number of trees per stem size class to remove)
 *		* perhectare	(retention targets = number of trees per hectare per stem size class)
 *		* distribution	(retention targets = frequency distribution (counts per class))
 *
 **************************************************************************************************/
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include "/Users/mmfuller/Documents/ACADEMIC_PROJECTS/UTOR_Manuscripts_Projects/SORTIE_RELATED_PROJECTS/FULLER_CPP_PROGRAMS/DEVELOPMENT/Harvest_Classes/src/CutBlock.cpp"

using namespace std;
using namespace MMFdata;

const string WORKINGDIR = "/Users/mmfuller/Documents/ACADEMIC_PROJECTS/UTOR_Manuscripts_Projects/SORTIE_RELATED_PROJECTS/FULLER_CPP_PROGRAMS/DEVELOPMENT/Harvest_Classes/";

int main()
{
	ofstream shortfout, longfout, errorout;
	int i, numsizeclasses, plotnumber;
	vector<double> BAin, BAout;
	double totalBA_before(0), totalBA_after(0), BA_threshold;
	string pramfile = WORKINGDIR + "cbparameters.txt";
	string shortoutput = WORKINGDIR + "MMF_harvest_cutrecord_";
	string longoutput = WORKINGDIR + "MMFharvest_longrecord_";
	string errorlog = WORKINGDIR + "MMF_harvest.errorlog.txt";
	char ans;
	
	//directory path and name for output file (file to be read by SORTIE)
	string outpath = WORKINGDIR + "cbSORTIEdata.txt";
	
	//file for holding harvest map (all trees, live and dead)
	string harvestedplot = WORKINGDIR + "MMFharvest_harvtreemap.txt";
	
	//ERROR LOGGING
	//errorout.open(errorlog.c_str(), ios::app);
	//errorout << "\nERROR LOG for MMF_harvest.exe\n";
	//errorout.close();

	//Initialize a CutBlock object
	CutBlock treestand;
	//CutBlock treestand(errorlog);
	
	//load parameter settings and 
	//extract harvest settings and tree data from files
	treestand.get_pramfile(pramfile);
	plotnumber = treestand.get_cutblock_n();
	BA_threshold = treestand.get_harvest_thresholdBA();
	
	cout << "\nplotnumber " << plotnumber
	<< "\nBA threshold " << BA_threshold;

	//show input data
	//treestand.show_harvestcriteria();
	
	//set vector sizes
	numsizeclasses = treestand.get_numsizeclasses();
	BAin.resize(numsizeclasses);
	BAout.resize(numsizeclasses);
	
	//get Basal Area before cutting
	for(i = 0; i < numsizeclasses; i++) {
		BAin[i] = treestand.group_basalarea_perha(i+1);
		totalBA_before += treestand.group_basalarea_perha(i+1);
	}
	
	//cut trees
	treestand.harvest();
	
	//print data needed by SORTIE
	treestand.print_SORTIEdata(outpath);
	
	//pring harvested plot
	treestand.print_harvestmap(harvestedplot);
	
	//get Basal Area after cutting
	for(i = 0; i < numsizeclasses; i++) {
		BAout[i] = treestand.group_basalarea_perha(i+1);
		totalBA_after += treestand.group_basalarea_perha(i+1);
	}
	
	shortfout.open(shortoutput.c_str());
	longfout.open(longoutput.c_str(), ios::app);

	longfout << "\nMMF_harvest.exe results long summary\n"
		<< "\nPlot Number: " << plotnumber 
		<< "\nHarvest Threshold (BA): " << BA_threshold
		<< "\nTotal precut BA: " << totalBA_before << " m2/ha"
		<< "\nTotal postcut BA: " << totalBA_after << " m2/ha"
		<< "\nPercent BA removed: " << setprecision(2) 
		<< (1 - totalBA_after/totalBA_before)*100 << "%" 
		<< "\n----------------------------------------------\n";
				  
	shortfout  << "\nMMF_harvest.exe results short summary\n"
		<< "\nPlot Number: " << plotnumber 
		<< "\nHarvest Threshold (BA): " << BA_threshold
		<< "\nTotal precut BA: " << totalBA_before << " m2/ha"
		<< "\nTotal postcut BA: " << totalBA_after << " m2/ha"
		<< "\nPercent BA removed: " << setprecision(2) 
		<< (1 - totalBA_after/totalBA_before)*100 << "%" <<endl;
	
	shortfout << "\nPercent BA removed per class\nClass\tBAin\tBArem\tPercent Removed\n";
	for(i = 0; i < numsizeclasses; i++) {
		if(BAin[i] > 0) {
			shortfout << i+1 << "\t" 
			<< BAin[i] << "\t" << BAin[i] - BAout[i] << "\t"
			<< setprecision(2) 
			<< (1 - BAout[i]/BAin[i]) *100 << "%" << endl;
		}
		else
			shortfout << i+1 << "\t" << 0 << "%\n";
		
		shortfout << endl;
	}
	shortfout.close();
	longfout.close();

	//cout << "\nEnter any value and hit return to close window ";
	//cin >> ans;
	//if(ans) exit(1);
	
	return 0;
}

