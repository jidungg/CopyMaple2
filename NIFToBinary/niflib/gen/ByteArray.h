/* Copyright (c) 2006, NIF File Format Library and Tools
All rights reserved.  Please see niflib.h for licence. */

#ifndef _BYTEARRAY_H_
#define _BYTEARRAY_H_

#include "../NIF_IO.h"

namespace Niflib {


/*!
 * An array of bytes.
 */
struct NIFLIB_API ByteArray {
	/*! Default Constructor */
	ByteArray();
	/*! Default Destructor */
	~ByteArray();
	/*!
	 * The number of bytes in this array
	 */
	mutable uint dataSize;
	/*!
	 * The bytes which make up the array
	 */
	vector<byte > data;
};

}
#endif
