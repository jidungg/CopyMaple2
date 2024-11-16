/* Copyright (c) 2006, NIF File Format Library and Tools
All rights reserved.  Please see niflib.h for licence. */

#ifndef _SKINDATA_H_
#define _SKINDATA_H_

#include "../NIF_IO.h"

// Include structures
#include "SkinWeight.h"
namespace Niflib {


/*!
 * Skinning data component.
 */
struct NIFLIB_API SkinData {
	/*! Default Constructor */
	SkinData();
	/*! Default Destructor */
	~SkinData();
	/*!
	 * Rotation offset of the skin from this bone in bind position.
	 */
	Matrix33 rotation;
	/*!
	 * Translation offset of the skin from this bone in bind position.
	 */
	Vector3 translation;
	/*!
	 * Scale offset of the skin from this bone in bind position. (Assumption
	 * - this is always 1.0 so far)
	 */
	float scale;
	/*!
	 * Translation offset of a bounding sphere holding all vertices. (Note
	 * that its a Sphere Containing Axis Aligned Box not a minimum volume
	 * Sphere)
	 */
	Vector3 boundingSphereOffset;
	/*!
	 * Radius for bounding sphere holding all vertices.
	 */
	float boundingSphereRadius;
	/*!
	 * Number of weighted vertices.
	 */
	mutable ushort numVertices;
	/*!
	 * The vertex weights.
	 */
	vector<SkinWeight > vertexWeights;
};

}
#endif
