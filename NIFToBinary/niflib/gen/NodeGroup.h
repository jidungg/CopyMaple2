/* Copyright (c) 2006, NIF File Format Library and Tools
All rights reserved.  Please see niflib.h for licence. */

#ifndef _NODEGROUP_H_
#define _NODEGROUP_H_

#include "../NIF_IO.h"

namespace Niflib {

// Forward define of referenced blocks
class NiNode;

/*!
 * A group of NiNodes references.
 */
struct NIFLIB_API NodeGroup {
	/*! Default Constructor */
	NodeGroup();
	/*! Default Destructor */
	~NodeGroup();
	/*!
	 * Number of node references that follow.
	 */
	mutable uint numNodes;
	/*!
	 * The list of NiNode references.
	 */
	vector<NiNode * > nodes;
};

}
#endif
