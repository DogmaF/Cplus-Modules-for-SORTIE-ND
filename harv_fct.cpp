



//rand_harvest_byindiv(BAToflargestclass, BAtotalharvested);

	rand_harvest_byindiv(double BAToflargestclass, double BAtotalharvested) {
		
		int i, index, randindex, sizeclass, count, trees2protect;
		double BAToflargestclass(0), BAtotalharvested(0);
		bool test;
		PlotTree* ptr = NULL; PlotTree* nbor = NULL;
		
		while(  basalarea_tracker[sizeclasses.size() -1] < BAToflargestclass 
			  && BAtotalharvested < maxtotalharvested)
		{
			//PT_hashtable contains pointers to trees in treemap and 
			//pointers to each tree's immediate neighbor.
			
			//get random number between 0 and number of trees in hashtable
			randindex = ( rand() % PT_hashtable.size() );
			
			//Get pointer to a random tree 
			ptr = PT_hashtable[randindex]->getPTptr();
			sizeclass = ptr->get_stemclass();
			
			//test whether to cut tree pointed to by ptr 
			//if cutting won't overshoot harvest target, run test
			if( (tree_basalarea(ptr)/10000 + basalarea_tracker[sizeclass -1]) 
			   < basalareatargets[sizeclass -1]
			   && (tree_basalarea(ptr)/10000 + BAtotalharvested) < maxtotalharvested)
				test = cuttarget(ptr);
			
			//If tree is cut, add its basal area to appropriate target
			//retrieve neighbor of tree pointed to by ptr and
			//point the PT pointer, nbor, to neighbor of ptr
			if(test == true) {
				
				//add basal area of ptr in meters square
				//tree basal area is per square meter, but group basal area is per hectares
				//divide by tree BA by 10000 to convert to BA per hectare
				basalarea_tracker[sizeclass -1] += tree_basalarea(ptr)/10000;
				BAtotalharvested += tree_basalarea(ptr)/10000;
				
				//continue cutting neighbors if harvest target not met
				//find neighbor of current ptr
				nbor = PT_hashtable[randindex]->getLink()->getPTptr();
				sizeclass = nbor->get_stemclass();
				
				//test whether to cut nbor
				if( (tree_basalarea(nbor)/10000 + basalarea_tracker[sizeclass -1]) 
				   < basalareatargets[sizeclass -1]
				   && (tree_basalarea(ptr)/10000 + BAtotalharvested) < maxtotalharvested)
					test = cuttarget(nbor);
				
				if(test == true) {
					
					//if nbor is cut, find and test neighbor of nbor, etc
					//index is used to prevent infinite looping
					//two trees have been examined so set index to 2 
					
					index = 2;
					while(test == true 
						  && index < PT_hashtable.size() 
						  && BAtotalharvested < maxtotalharvested) {
						
						//add basal area of current nbor to sum for class
						basalarea_tracker[sizeclass -1] += tree_basalarea(nbor)/10000;
						BAtotalharvested += tree_basalarea(ptr)/10000;
						
						//test whether to cut next nbor
						//test_nextnbor() redirects the pointer, nbor, to its immediate neighbor
						test = test_nextnbor(nbor);
						
						//get sizeclass of current nbor
						sizeclass = nbor->get_stemclass();
						
						index++;
					}
				}
			}
		} // end while
	}