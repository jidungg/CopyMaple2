/* Copyright (c) 2006, NIF File Format Library and Tools
All rights reserved.  Please see niflib.h for licence. */

#ifndef _HEADER_H_
#define _HEADER_H_

#include "../NIF_IO.h"
#include "../obj/NiObject.h"

namespace Niflib {


/*!
 * The NIF file header.
 */
struct NIFLIB_API Header {
	/*! Default Constructor */
	Header();
	/*! Default Destructor */
	~Header();
	/*!
	 * 'NetImmerse File Format x.x.x.x' (versions <= 10.0.1.2) or 'Gamebryo
	 * File Format x.x.x.x' (versions >= 10.1.0.0), with x.x.x.x the version
	 * written out. Ends with a newline character (0x0A).
	 */
	HeaderString headerString;
	/*!
	 * Unknown.
	 */
	array<3,LineString> copyright;
	/*!
	 * The NIF version, in hexadecimal notation: 0x04000002, 0x0401000C,
	 * 0x04020002, 0x04020100, 0x04020200, 0x0A000100, 0x0A010000,
	 * 0x0A020000, 0x14000004, ...
	 */
	uint version;
	/*!
	 * Determines the endian-ness of the data.              1 = little endian
	 * (default)             0 = big endian
	 */
	byte endianType;
	/*!
	 * An extra version number, for companies that decide to modify the file
	 * format.
	 */
	uint userVersion;
	/*!
	 * Number of file objects.
	 */
	mutable uint numBlocks;
	/*!
	 * This also appears to be the extra user version number and must be set
	 * in some circumstances.
	 */
	uint userVersion2;
	/*!
	 * Could be the name of the creator of the NIF file?
	 */
	ShortString creator;
	/*!
	 * Unknown. Can be something like 'TriStrip Process Script'.
	 */
	ShortString exportInfo1;
	/*!
	 * Unknown. Possibly the selected option of the export script. Can be
	 * something like 'Default Export Script'.
	 */
	ShortString exportInfo2;
	/*!
	 * Number of object types in this NIF file.
	 */
	mutable ushort numBlockTypes;
	/*!
	 * List of all object types used in this NIF file.
	 */
	vector<string > blockTypes;
	/*!
	 * Maps file objects on their corresponding type: first file object is of
	 * type object_types[object_type_index[0]], the second of
	 * object_types[object_type_index[1]], etc.
	 */
	vector<ushort > blockTypeIndex;
	/*!
	 * Unknown.
	 */
	uint unknownInt2;
	void Read( istream& in );
	void Write( ostream& out ) const;
	string asString( bool verbose = false ) const;
};

}
#endif
