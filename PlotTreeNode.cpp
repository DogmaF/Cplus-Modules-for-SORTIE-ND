//class used to construct linked lists of PlotTree pointers


#ifndef PLOTTREENODE_H
#define PLOTTREENODE_H

#include "PlotTree.cpp"

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

