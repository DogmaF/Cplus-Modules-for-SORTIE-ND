/**********************************************************************************************
 *  CutBlock.h
 *  
 *  Created by Mike Fuller on June 25, 2008.
 *  Copyright 2008. All rights reserved.
 *
 *	PURPOSE:
 *		For use in forest harvest simulations. To set quotas (i.e. residual basal area, etc)
 *		set cut probabilities for targets and neighbors to desired percentages. For example, 
 *		to leave 30 percent BA in size class 1, set cut probabilities (target and neighbor) 
 *		to 70 percent. This will cause approximately 70 percent of the trees in the class to
 *		be cut. Quota trackers can be used to keep track of actual number of trees cut in each
 *		size class or species. 
 *
 *	EXTERNAL FILES
 *	INPUT FILES 
 *		cbparameters.txt = parameter settings, including data file name, type of harvest, etc.
 *		treedatafile = contains data for individual trees in sample plot
 *		sizeclassfile = limits of diameter classes of trees (if default limits not used)
 *		probcutfile = target cut probabilities per stem class
 *		probnborfile = neighbor cut probabilities per stem class
 *		cbquotas_int.txt = harvest targets for each stem class by count (one integer per line)
 *		cbquotas_dbl.txt = harvest targets for each stem class by basal area or other float 
 *		(one float per line).
 * 
 *	OUTPUT FILES
 *		cbSORTIEdata.txt = output formatted for SORTIE harvest input
 *
 *	HAVEST METHODS
 *		clearcut = cut all trees (SORTIE cuts adults and saplings, not seedlings).
 *
 *		singletree = trees selected randomly according to size class; 
 *					 continue cutting until max basal area removed.
 *
 *		singletree_propagation = trees selected randomly according to size class; 
 *								 continue cutting until max basal area removed.
 *								 neighbors also cut;
 *
 *		NOT YET IMPLEMENTED:
 *			perhectare = trees per ha to be cut in each class
 *			distribution = percent of total to be cut represented by each class
 *
 * DEFAULT STEM SIZE CLASSES (from Ontario Ministry of Natural Resources)
 *		dbh(cm)			class
 *		< 10			  0
 *		10 <= dbh < 26	  1
 *		26 <= dbh < 38	  2
 *		38 <= dbh < 50	  3
 *		50 <= dbh < 62	  4
 *		62 < dbh		  5
 *
 *	SORTIE DEFAULT "TREE TYPES"
 *		Type	Definition
 *		1		seedling 
 *		2		sapling 
 *		3		adult 
 *		4		stump 
 *		5		snag 
 *	
 *	DEFAULT MORTALITY CLASSES
 *		Class	Definition
 *		1		live 
 *		2		stump 
 *		3		snag 
 *
 *	NOTES
 *	During harvesting, selection of target trees is limited to individuals that meet the 
 *	selection criteria of the merchantable size classes (class numbers > 0). Selection of 
 *	neighbors is not constrained by size class; any individual in the original tree map 
 *	can be considered a neighbor, regardless of stem diameter.
 *
 **********************************************************************************************/

#ifndef CUTBLOCK_H
#define CUTBLOCK_H

#include <iostream>
#include <fstream>
#include <vector>
#include "PlotTree.hpp"
#include "plottreenode.cpp"

namespace MMFdata
{
	const double PI = 3.14159;
	
	class CutBlock
		{
		public:
			//default constructor
			//sets plotnumber to -1, fractionlargestretained to zero, cuttype to NULL
			//sets harvest_error to 10 percent, maxtotalharvested t0 0.33
			//sets harvest_thresholdBA to 16.0, timetable_flag to false
			//member vectors remain empty
			CutBlock();

			//same as default except includes output stream for logging runtime errors
			CutBlock(string errlog);	
			
			//calls default constructor for PlotTree class
			//sets member variables to values of arguments
			//sets fractionlargestretained to zero, harvest_error to 10 percent, maxtotalharvested t0 0.33
			//sets capacity of member vectors to sizes of vector arguments
			//assigns elements of member vectors to that of arguments
			//sets harvest_error to 10 percent of the basal area of a single tree
			//sets harvest_thresholdBA to 16.0, timetable_flag to false
			CutBlock(vector<PlotTree> data, vector<double> limits, int ID, string& type, 
					 vector<double> pcut, vector<double> pnborcut);
			
			//copy constructor
			CutBlock(const CutBlock& datObject);
			
			//destructor
			virtual ~CutBlock();
			
			//Accessor Functions 
			const int get_cutblock_n() const;
			const string get_cuttype() const;
			const vector<double> get_probcuttarget() const;
			const vector<double> get_probcutnbor() const;
			const vector<int> get_specieslist() const;
			const vector<PlotTree> get_treemap() const;
			const vector<PlotTreeNode*> get_hashtable() const;
			const vector<double> get_sizeclasses() const;
			const int get_numsizeclasses() const;
			const vector<int> get_classcounts() const;
			const vector<int> get_targetcounts() const;
			const vector<double> get_basalareatargets() const;
			const vector<int> get_perhectaretargets() const;
			const vector<double>get_distributiontargets() const;
			const double get_maxtotalharvestpercent() const;
			vector<double> get_classbasalareas();
			const double get_plotarea() const;
			const int get_targetscut() const;
			const int get_nborscut() const;
			const double get_ave_nborscut() const;
			const double get_harvest_thresholdBA() const;
			const bool get_timetable_flag() const;
			vector<int> get_timetable();
			
			//returns pointer to a PlotTree object
			PlotTree* get_ptr2tree(int index);
			
			//returns basal area (in square meters) of individual tree
			double tree_basalarea(PlotTree* target);
			double tree_basalarea(PlotTree& target);
			
			//returns basal area (in meters-squared per hectare) of size class
			double group_basalarea_perha(int sizeclass);
			
			//Mutator Functions
			void set_cutblock_n(int ID);
			void set_cuttype(string& type);
			void set_probcuttarget(vector<double> pt);
			void set_probcutnbor(vector<double> pn);
			void set_specieslist(vector<int> splist);
			void set_treemap(vector<PlotTree> map);
			void set_sizeclasses(vector<double> limits);
			void set_classcounts(vector<int> clcounts);
			void set_targetcounts(vector<int> tcounts);
			void set_basalareatargets(vector<double> tba);
			void set_perhectaretargets(vector<int> tperha);
			void set_distributiontargets(vector<double> dist);
			void set_maxtotalharvestpercent(double max);
			void set_harvest_thresholdBA(double thresholdBA);

			//sets values of timetable and
			//sets value of boolean 'timetable_flag' to true
			void set_timetable(vector<int> ttable);  
			
			//Sets cutting criteria, including type of cut, size class limits, 
			//amount of trees to cut or leave,  probability of cutting a target in  
			//each class, and probability of cutting a neighbor.
			//For use when harvest goals are in the form of counts
			void set_criteria(vector<double> sizeclasses, vector<double> classprobcut, 
							  vector<double> classnborprobcut,
							  vector<int> harvestgoals, string& harvesttype);
			
			//Sets cutting criteria as above, but amount to cut/leave is a vector of doubles.
			//For use when harvest goals are in the form of basal area or percent of forest.
			void set_criteria(vector<double> sizeclasses, vector<double> classprobcut,
							  vector<double> classnborprobcut,
							  vector<double> harvestgoals, string& harvesttype);
			
			//functions for processing input data to set values for plotsection, nnbor, etc
			
			/********** HARVEST PARAMETER AND DATA FILES ************************************
			 
			The get_pramfile() function connects to external file (default name "cbparameters.txt"), 
			extracts/sets parameter settings for harvest run, and enters raw tree data. The function
			look for the following text files in the current directory:
				* cbparameters.txt
				* cbBAtargets.txt
				* cbPHtargets.txt
				* cbDISTtargets.txt
				* cbplotcorners.txt
				
			In addition, names for following input files must be listed by the file cbparameters.txt:
				treedatafile, sizeclassfile, probcutfile, probnborfile. 
			 Default names for the above files are:
				* cbtreedata.txt
				* cbsizeclasses.txt
				* cbtargetp.txt
				* cbnborp.txt
			 
			*********************************************************************************/
			
			//connects to external file (default name "cbparameters.txt"), extracts/sets
			//parameter settings for harvest run, and enters raw tree data.
			//calculates plot total area in hectares
			void get_pramfile(const string directorypath);
			
			//PRINT FUNCTIONS
			
			//PRINT TO STD OUT
			
			//prints to std out the current harvest settings
			void show_harvestcriteria();
			
			//prints tree data to std out
			void show_treemap();
			
			//prints to std out the default definitions for mortality classes
			void show_mortclasses();	
	
			//PRINT TO FILE
			
			//prints to file, for each tree, the identity, species, stem diameter, stem class and 
			//mortality class of each tree
			void print_treemap();
			
			//prints to file X-Y data, diameter, and mortality class for each tree
			void print_harvestmap(const string directorypathandfilename);
			
			//prints only trees that have been killed
			void print_SORTIEdata(const string directorypathandfilename);

			
			/******* COMMON FUNCTIONS CALLED BY ALL HARVEST METHODS *****************
			 
			 Calling one of the cutblock functions initiates a series of function calls, that
			 include the following along with other functions:
			 1) set_rand()						= set seed of random number generator to current time
			 2) revsort_treemap_bydiameter()	= sort trees in reverse order by stem diameter
			 3) fill_hastable()					= link pointers to trees in treemap
			 4) link_coreneighbors()			= find nearest neighbors of trees
			 5) ind_recippairs()				= find reciprocal neighbors
			 6) get_classcounts()				= count trees in each size class
			 7) cuttarget()						= cut trees according to target cut probabilty
			 8) cutnbor()						= cut neighboring trees according to neighbor cut probability
			 9) test_nextnbor()					= cut neighbor of neighbor according to propagation probability
			 
			 ***/ 
			
			//determines type of harvest and applies corresponding cutblock type
			void harvest();
			
			//sorts PlotTree objects in treemap vector by stem diameter, in descending order.
			//(i.e. largest to smallest)
			void revsort_treemap_bydiameter();
			
			//sorts PlotTree objects by identity
			void sort_treemap_byID();
			
			//Called only within the context of a cutblock method.
			//Calls cuttree; does not check previous mortality status of target
			//Determines cut probability based upon stemclass of tree,
			//Returns true if tree was killed, false if not.
			bool cuttarget(PlotTree* target);
			
			//Called only within the context of a cutblock method.
			//Calls cuttree; does not check previous mortality status of target
			//If neighbor is a member of a reciprocal pair, uses a reduced cutting probability.
			// WARNING: if probability of cutting neighbor is 1, ALL TREES WILL BE CUT!
			bool cutnbor(PlotTree* neighbor);
			
			//Used in harvest methods that use neighborhood cut propagation.
			//Attempts to redirect pointer, nextnbor, to it's immediate neighbor.
			//If successful, cuts neighbor with probability probcutnbor of its size class.
			//If cut test is true and neighbor was previously alive, cuts it and returns true.
			//If cut test is true and neighbor was previously dead, restores it and returns false.
			//If cut test if false, does nothing to neighbor and returns false.
			//If neighbor is a member of a reciprocal pair, uses a reduced cutting probability.
			//trees smaller than smallest size class are ignored (not considered neighbors).
			bool test_nextnbor(PlotTree* nextnbor);
			
			/******* HARVEST METHODS ************************************************/
			
			//cuts every tree (adults and saplings)
			void cutblock_clearcut();
			
			/***** CUTBLOCK_SINGLETREE ******
			 * Calls propagationcut_BA()
			 * Cuts according to schedule of basal area to remove per size class
			 * First, a size class is chosen at random. 
			 * Next, a target tree is chosen at random within the class.
			 * If the target is cut, it's neighbor is targeted for cutting, and so on.
			 *
			 * Targets are cut in random order of stem diameter class, according to a cut probability. 
			 * Neighbors of targets are cut according to a separate probability value for neighbors.
			 * Neighbors of neighbors are cut successively until the cut probability is exceeded. 
			 * If the target is a member of a reciprocal pair, a reduced cut probabiity is used.
			 * 
			 * Nearest neighbors are identified using a torus plot configuration, such that the nearest neighbor
			 * may occur on the opposite boundary of the plot (a distance <= to the length of the plot). 
			 * Neighbors cut at random according to neighbor cut probability
			 * Continues to cut neighbors of neighbors until random number > cut probability
			 **********/
			void cutblock_singletree();
			
			/***** CUTBLOCK_SINGLETREE_PROPAGATION ******
			* Calls propagationcut_BA()
			* Cuts according to schedule of basal area to remove per size class
			* First, a size class is chosen at random. 
			* Next, a target tree is chosen at random within the class.
			* If the target is cut, it's neighbor is targeted for cutting, and so on.
			*
			* Targets are cut in random order of stem diameter class, according to a cut probability. 
			* Neighbors of targets are cut according to a separate probability value for neighbors.
			* Neighbors of neighbors are cut successively until the cut probability is exceeded. 
			* If the target is a member of a reciprocal pair, a reduced cut probabiity is used.
			* 
			* Nearest neighbors are identified using a torus plot configuration, such that the nearest neighbor
			* may occur on the opposite boundary of the plot (a distance <= to the length of the plot). 
			* Neighbors cut at random according to neighbor cut probability
			* Continues to cut neighbors of neighbors until random number > cut probability
			**********/
			void cutblock_singletree_propagation();
			
			/***** CUTBLOCK_GROUPTREE ******  
			 NOT YET TESTED 
			 
			 * Calls groupcut_BA()
			 * Cut design follows guidelines of OMNR (2004 tree marking guide)
			 * Cuts groups of trees found within a specified canopy gap diameter
			 * Canopy gap diameter is a percent of tree height (0 - 200%)
			 * Canopy gap diameter never exceeds max_gapdiameter (default 50m)
			 * Cuts all trees >= 2cm DBH that are within gap_diameter/2 distance of target
			 * Total BA removed according to schedule of basal area to remove per size class
			 * 
			 * First, coordinates of gap center are chosen at random 
			 * If location not inside a previous gap, all trees >= 2cm are cut up to a distance of gap_diameter/2 from center
			 * If location is inside or adjacent to previous gap, new coordinates are assigned
			 *
			 **********/
			 void cutblock_grouptree();
			
			/*** Harvest Functions not yet implemented or in progress ****************
			 
			* void cutblock_countperha();
			* void cutblock_percentabund();
			* void cutblock_distribution();
			 
			//cuts according to schedule of basal area to remove per size class
			//trees targeted for harvest at random
			* void cutblock_basalarea_byindiv();
			**************************************************************************/

			/******** OVERLOADED OPERATORS *******************************************/
			
			//assignment
			CutBlock& operator =(const CutBlock& rhs);
			
			//comparison
			bool operator ==(const CutBlock& rhs) const;
			bool operator !=(const CutBlock& rhs) const;
			
		private:
			
			/************ SCALARS AND VECTORS *************/
			
			int cutblock_n;		//identifies this cutblock (= plotnumber)
			int currentstep;	//current time step of SORTIE run
			int totalstep;		//total number of time steps in SORTIE run
			int nonzero_count;	//number of size classes that contain trees
			int targetscut;  //total number of target trees cut
			int nborscut;    //total number of neighbor trees cut
			int gap_n;       //number of gaps created during group cut
			double plotarea; //total plot area in hectares 
			double fractionlargestretained; //fraction of largest size class that is not to be cut 
			double harvest_error; //percent of harvest goal to act as buffer, or lower bound of actual goal
			double harvest_thresholdBA; //cut threshold: total absolute basa area of trees stand
										//if set, cutting only occurs when total BA hits threshold
			double maxtotalharvestpercent; //maximum allowed basal area, count per ha, or counts to be removed
			double BAtotalharvested;  //total amount harvested during a harvest run in meters-square per ha
			double ave_nborscut;	//average number of neighbors cut per target cut
			double ave_gapsize;     //average area in ha of gaps created
			double gap_radius_percentageoftreeheight;  //percent of tree height used to set radius of gap
			
			bool timetable_flag;		//true if specific time steps to harvest are provided for timetable
			string cuttype;		//type of harvest algorithm
			time_t currenttime; //time variable for seeding random number generator
			string errorlog; //appendable file that records events as they take place
			
			//probability of cutting a tree, for each size class
			vector<double> probcuttarget;		
			
			//probability of cutting neighbor of a tree, for each size class
			vector<double> probcutnbor;	
			
			//list of species names as integers
			vector<int> specieslist;  
			
			//records all trees located within the cut block, regardless of size
			vector<PlotTree> treemap;
			
			//hash table of pointers mapped to treemap
			//only includes individuals that are at least as large as the
			//smallest size class
			vector<PlotTreeNode*> PT_hashtable;
			
			//vectors to hold trees found along torus joins of plot
			vector<PlotTree> torusjointrees_N2S;
			vector<PlotTree> torusjointrees_S2N;
			vector<PlotTree> torusjointrees_E2W;
			vector<PlotTree> torusjointrees_W2E;
			
			//table of reciprocal neighbors
			vector<int> recipnbor_IDs;
			
			//holds lower-upper limits of size classes
			vector<double> sizeclasses;
			
			//holds indices for size class limits from PT_hashtable
			vector<int> sizeclassindices;
			
			//holds count of trees in each size class
			vector<int> classcounts;
			
			//size classes that contain data
			vector<int> nonzeroclasses;
			
			//harvest target values for each class, by number of trees to cut
			vector<int>targetcounts;
			
			//for keeping track of number of trees cut in each class
			vector<int> cuttree_tracker;
			
			//for keeping track of double (float) quotas (meters square per ha)
			vector<double> basalarea_tracker;
			
			//harvest target values (amount to cut) for each class, by basal area to cut 
			//(meters square per ha)
			vector<double> basalareatargets;

			//harvest target values (amount to cut) for each class, by trees per hectare to cut
			vector<int>perhectaretargets;
			
			//harvest target distribution (amount to cut) across size classes
			vector<double>distributiontargets;
			
			//exception species: names of species that will not be cut during harvest
			vector<int> exception_species;
			
			//holds min and max values of X and Y coordinates, representing plot corners
			//in order of min X, max X, min Y, max Y
			//used to calculate plot total area in hectares
			vector<double> plotcorners;
			
			//holds timestep values for restricting harvest to certain steps
			vector<int> timetable;
			
			//keeps track of gap locations (group cut)
			vector<PlotTree> gapcenters_v;
			
			/***************** FUNCTIONS ******************/
			
			//functions used for sorting the treemap
			int find_maxdiam(int top);
			int find_minID(int top);
			void swap_PlotTree(PlotTree& t1, PlotTree& t2);
			
			//for use after treemap has been reverse sorted by stem diameter
			int index_startofclass(int sizeclass);
			
			//sets values of sizeclasses to default settings
			void set_defaultclasssizes();
			
			//computes shortest distance between two trees, accounting for stem diameter
			//calculated based on torus plot construction (plot edges wrap around) 
			double dist(PlotTree a, PlotTree b);
			
			//functions used only by cuttarget and cutnbor.
			//returns true if tree is cut
			//if tree is cut, sets tree's mortality class to 2
			bool cuttree(PlotTree* target, const double pvalue);
			
			//Runs cut probability test without affecting class basal area total
			//used when running p-test on a neighbor that is already dead
			bool cuttest(const double pval);
			
			//sets mortality class of tree to live
			void resurrecttree(PlotTree* deadtree);
			
			//sets mortality class of tree to live
			//sets size class to -1 so it will not be cut again
			//and returns basal area per hectare of tree
			void restoretree(PlotTree* deadtree);
			
			// Constructs map of pointers to each tree and its nearest neighbor.
			// WARNING: sorting treemap after mapping nbors may destroy original 
			// relationship between pointers and objects
			void fill_hastable();
			
			//finds reciprocal pairs and adds tree identities to recipnbors vector
			//PRECONDITION: tree neighbor links have been set up
			void find_recippairs();
			
			//returns largest value of nearest neighbor distance of trees 
			//located within core of plot (does not include torus joins)
			double max_nndist();
			
			// Maps pointers from each tree to its nearest neighbor in plot core.
			// WARNING: sorting treemap after mapping nbors may destroy original 
			// relationship between pointers and objects
			void link_neighbors();
			
			/*Called only within the context of filling the hash table.
			  Returns nearest neighbor of target restricted to trees in plot core.
			  Accounts for diameter of stem when calculating distance between trees.
			  PRECONDITION: hashtable filled with pointers to treemap elements.
			 */
			PlotTree* findneighbor(PlotTree* targettree);
			
			//For double checking input criteria
			//examines vector for values = 1 or 0
			void singularitytest(vector<double> values, const string type);
			
			//attempts to open filestream and tests for failure
			//exit on fail
			void openfile(ifstream& inputstream, string filename);
			void openoutput(ofstream& outputstream, string filename);
			
			//returns stem diameter class of tree
			int calc_stemclass(double diam);
			
			/************    DATA INPUT FUNCTIONS    ***********/
			//called by get_pramfile()
			void extract_sizeclassfile(string file);
			
			//fills treemap vector with trees listed in file
			//assigns tree size class based upon dbh
			void extract_treedatafile(string file);
			
			//these four functions simply connect to file and
			//place contents in appropriate data containers
			void extract_probcutfile(string file);
			void extract_nborcutfile(string file);
			void extract_plotcornersfile(string file);
			void extract_timesteptable(string file);
			
			/*****************************************************/
			
			//Seeds random number generator to current time
			void set_rand();
			
			//extracts harvest targets from file
			void getr_harvesttargets(string filename, vector<double>& values);
			void getr_harvesttargets(string filename, vector<int>& values);
			
			//fills vector with size class boundaries, equal to the index of the PT_hashtable
			//where the tree diameter = the lowest value of each class
			//if no trees are greater than the lowest value, classbound == 0
			//if all trees greater than the lowest value, classbound == PT_hashtable.size
			void set_classbounds();
			
			//fills classcounts vector with number of trees in each size class
			//uses PT_hashtable; Assumes PT_hashtable has been filled!
			void calc_classcounts();
			
			//looks for species n in exception_species; returns false if not found
			bool is_spexception(int n);
			
			//calculates number of trees to preserve using classcounts
			//assumes classcounts has non-zero values
			int count_protected();
			
			//removes a fixed number of trees from top of PT_hastable
			//returns basal area of the removed trees
			//Used to protect the largest trees from harvesting
			//Assumes that PT_hashtable contains pointer to treemap, reverse sorted by stem size
			double set_trees2protect(int num2protect);
			
			//finds elements of PT_hashtable that are the neighbors of a group of protected trees
			//and destroys their links to the protected trees (calls delete operator to return
			//memory of destroyed pointers to the heap
			void find_neighborsofprotected(vector<PlotTree*> protectedtrees, 
													 vector<PlotTreeNode*>& neighborsofprotected);
			
			//sets the links of a group of PlotTreeNode objects to point to the
			//nearest neighbors of the objects
			//useful when a subgroup of the PT_hashtable needs to be linked
			//Assumes PT_hashtable has been filled with pointers to treemap
			void set_group_links(vector<PlotTreeNode*>& group);
			
			/****************** HARVEST FUNCTIONS ********************/
			
			//Harvest fucntions cut trees whose stem sizes fall within the ranges listed in the  
			//sizeclasses vector (i.e. the pool of harvestable trees).
			
			//Basal area cut by random individual: 
			//Trees are selected at random from the pool of harvestable trees  
			//Harvest continues until maximum allowable basal area is removed (total BA across size classes).
			void BAharvest_byrandindiv();
			
			//Basal area cut by random size class: 
			//During the harvest, a size class is selected at random (uniform) from the list of classes.
			//A tree is then selected at random (uniform) from the chosen size class.
			//Harvest continues until maximum allowable basal area is removed (total BA across size classes).
			void propagationcut_BA();
			
			//calculate plots area from X and Y values of plot corners
			//plot corners are read in from the parameter file
			double calc_plotarea();
			
			//add tree basal area to basalarea_tracker[this class] and BAtotalharvested
			void addto_basalarearemoved(PlotTree* tree);
			
			//add basal area value to basalarea_tracker[this class] and BAtotalharvested
			void addto_basalarearemoved(double BAremoved);
			
			//sets top_n and bottom_n to the head and tail of the range of values in the PT_hashtable,
			//that represent the trees of the size class stemsizeclass
			void set_topandbottom(int& top_n, int& bottom_n, const int stemsizeclass);
			
			//Neighborhood cut propagation
			//cuts neighbor of neighbor, adds basal area cut to total for class and
			//adds basal area cut to total for cut block
			//continues cutting first-nearest neighbors until cut test returns false
			void propagate_nborcut(PlotTree* neighbor, int targets_completed[]);
			
			void groupcut_BA();
			
			//returns index to a size class
			//returns -1 if all class targets achieved
			//chooses index at random from the list of non-zero size classes (classes that have data)
			//limits choice to classes whose basal area targets have not yet been attained. 
			//Thus, randomness of choice declines as harvest proceeds, and randomness is limited to 
			//the number of non-zero size classes
			int choose_randsizeclass(int& top_n, int& bot_n, int targets_completed[]);
			
			//returns pointer to neighbor of target
			PlotTree* get_neighbor(PlotTreeNode* target);
			
			//reverse sort treemap, fill hashtable, count trees in each class, count non-zero classes
			void prep_treedata();
			
			double rand_Xcoord();
			double rand_Ycoord(); 
			bool is_ingap(const PlotTree location, const double gap_radius);
			void cut_gaptrees(const PlotTree centerofgap, const double radius);
			double calc_sampleBA(vector<PlotTree> sample);
			
		};
	
} //MMFdata

#endif  //CUTBLOCK_H




