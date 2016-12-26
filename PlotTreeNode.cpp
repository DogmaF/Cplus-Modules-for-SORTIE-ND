//class used to construct linked lists of PlotTree pointers


#ifndef PLOTTREENODE_H
#define PLOTTREENODE_H

#include "/Users/KarmaYeshe/Documents/ACADEMIC_PROJECTS/UTOR_Manuscripts_Projects/SORTIE_RELATED_PROJECTS/FULLER_CPP_PROGRAMS/DEVELOPMENT/General_Use_Classes/Classes_Datum/PlotTree.cpp"

namespace MMFdata
{
	class PlotTreeNode
		{
		public:
			PlotTreeNode(PlotTree* treeptr, PlotTreeNode* nodelink)
				: PTptr(treeptr), link(nodelink) {}

			PlotTree* getPTptr() const { return PTptr; }
			PlotTreeNode* getLink() const { return link; }
			void setPTptr(PlotTree* treeptr) { PTptr = treeptr; }
			void setLink(PlotTreeNode* ptr) { link = ptr; }
			
			PlotTreeNode& operator =(const PlotTreeNode& rhs) {
				//check for self assignment
				if(this == &rhs) return *this;
				else {
					PTptr = rhs.PTptr;
					link = rhs.link;
						
					return *this;
				}
			}
			
			
		private:
			PlotTree* PTptr;
			PlotTreeNode* link;
		};

}

#endif  //PLOTTREENODE_H

