/* main.cc */

#include <iostream>
#include <openvdb/openvdb.h>


int main() {

    /* initialize */
    openvdb::initialize();

    /* create an empty floating point grid */
    openvdb::FloatGrid::Ptr grid = openvdb::FloatGrid::create();    

    std::cout << "Testing random access:" << std::endl;
    
    /* get accessor */
    openvdb::FloatGrid::Accessor accessor = grid->getAccessor();
    
    /* define a coordinate */
    openvdb::Coord xyz(1000, -200000000, 30000000);

    /* set the voxel value */
    accessor.setValue(xyz, 1.0);    

    /* verify */
    std::cout << "Grid " << xyz << " = " << accessor.getValue(xyz) << std::endl;
    
    /* reset the coordinates */
    xyz.reset(1000, 200000000, -30000000);

    /* verify */
    std::cout << "Grid " << xyz << " = " << accessor.getValue(xyz) << std::endl;

    /* set the voxel value */
    accessor.setValue(xyz, 2.0);

    /* set the voxel value */
    accessor.setValue(openvdb::Coord::min(), 3.0f);
    accessor.setValue(openvdb::Coord::max(), 4.0f);
    
    std::cout << "Testing sequential access: " << std::endl;    
    /* print all */
    for (openvdb::FloatGrid::ValueOnCIter iter = grid->cbeginValueOn(); iter; ++iter)
	{
	    std::cout << "Grid " << iter.getCoord() << " = " << *iter << std::endl;
	}
    return 0;  
}
    
