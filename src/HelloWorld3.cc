/* taken from openvdb vdb_view */

#include "openvdb/Types.h"
#include <openvdb/openvdb.h>
#include <openvdb/viewer/Viewer.h>
#include <openvdb/tools/LevelSetSphere.h>

#include <boost/algorithm/string/classification.hpp> // for boost::is_any_of()
#include <boost/algorithm/string/predicate.hpp> // for boost::starts_with()
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>



int main(int argc,  char *argv[]) {

  const char* progName = argv[0];
  if (const char* ptr = ::strrchr(progName, '/')) progName = ptr + 1;

  int status = EXIT_SUCCESS;


  try {
    openvdb::initialize(); // usually this is needed when there is a file io
    openvdb::logging::initialize(argc, argv);

    openvdb_viewer::Viewer viewer = openvdb_viewer::init(progName, /*bg=*/false);
    openvdb::GridCPtrVec allGrids;

    openvdb::Int32Grid::Ptr grid = openvdb::Int32Grid::create();

    // Get an accessor for coordinate-based access to voxels.
    openvdb::Int32Grid::Accessor accessor = grid->getAccessor();

    // Define a coordinate with large signed indices.
    openvdb::Coord xyz(0, 0, 0);

    int size = 8;
    
    for (int i  = 0; i < size; i++)
      {
	for (int j  = 0; j < size; j++)
	  {
	    for (int k  = 0; k < size; k++)
	      {
		xyz.setX(i);
		xyz.setY(j);
		xyz.setZ(k);
		accessor.setValue(xyz, 1);
	      }
	  }
      }


    // Identify the grid as a level set.
    grid->setGridClass(openvdb::GRID_LEVEL_SET);

    
    allGrids.push_back(grid);
	
    viewer.open();
    viewer.view(allGrids);

    openvdb_viewer::exit();
	

  } catch (const char* s) {
    OPENVDB_LOG_FATAL(s);
    status = EXIT_FAILURE;
  } catch (std::exception& e) {
    OPENVDB_LOG_FATAL(e.what());
    status = EXIT_FAILURE;
  } catch (...) {
    OPENVDB_LOG_FATAL("Exception caught (unexpected type)");
    std::terminate();
  }

  return status;
    
}
    

