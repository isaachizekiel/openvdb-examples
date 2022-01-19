/**/

#include <openvdb/tools/LevelSetSphere.h>
#include "viewer/Viewer.h"


void make_sphere(openvdb::GridCPtrVec *allGrids) {

    openvdb::FloatGrid::Ptr grid = openvdb::tools::createLevelSetSphere<openvdb::FloatGrid>(/*radius=*/50.0, /*center=*/openvdb::Vec3f(1.5, 2, 3),/*voxel size=*/0.5, /*width=*/4.0);

    grid->insertMeta("radius", openvdb::FloatMetadata(50.0));
    // Name the grid "LevelSetSphere".
    grid->setName("LevelSetSphere");

    allGrids->push_back(grid);
}


int main(int argc,  char *argv[]) {

  const char* progName = argv[0];
  if (const char* ptr = ::strrchr(progName, '/')) progName = ptr + 1;

  int status = EXIT_SUCCESS;


  try {
    openvdb::initialize(); // usually this is needed when there is a file io
    openvdb::logging::initialize(argc, argv);

    openvdb_viewer::Viewer viewer = openvdb_viewer::init(progName, /*bg=*/false);
    openvdb::GridCPtrVec allGrids;

    // create the sphere object
    make_sphere(&allGrids);
	
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
