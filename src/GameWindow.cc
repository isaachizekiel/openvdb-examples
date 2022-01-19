/* */

#if defined(_WIN32)
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

#include "GameWindow.h"


namespace game_window {

  class GameWindowImpl {

  public:
    GameWindowImpl();

    void init(const std::string& progName);

    std::string getVersionString() const;

    bool isOpen() const;
    bool open(int width = DEFAULT_WIDTH, int height = DEFAULT_HEIGHT);
    // void view(const openvdb::GridCPtrVec&);
    void handleEvents();
    void close();

    void resize(int width, int height);

    // internal
    void interrupt();

    static void sleep(double seconds);

  private:

    bool mDidInit;
    //CameraPtr mCamera;
    //ClipBoxPtr mClipBox;
    //RenderModulePtr mViewportModule;
    //std::vector<RenderModulePtr> mRenderModules;
    openvdb::GridCPtrVec mGrids;
    size_t mGridIdx, mUpdates;
    std::string mGridName, mProgName, mGridInfo, mTransformInfo, mTreeInfo;
    int mWheelPos;
    bool mShiftIsDown, mCtrlIsDown, mShowInfo;
    bool mInterrupt;

    GLFWwindow* mWindow;

    
  };
  

  class ThreadManager {

  public:
    ThreadManager();

    void view(const openvdb::GridCPtrVec& gridList);
    void close();
    void resize(int width, int height);
    void doView();

  private:
    static void* doViewTask(void* arg);

    std::atomic<bool> mRedisplay;
    bool mClose, mHasThread;
    std::thread mThread;
    openvdb::GridCPtrVec mGrids;
    
  };


  namespace  {
    GameWindowImpl* sGameWindow = nullptr;
    ThreadManager* sThreadMgr = nullptr;
    std::mutex sLock;

    void
    keyCB(GLFWwindow*, int key, int /*scancode*/, int action, int /*modifiers*/)      
    {
      //if (sGameWindow) sGameWindow->keyCallback(key, action);
    }


    void
    mouseButtonCB(GLFWwindow*, int button, int action, int /*modifiers*/)
    {
      //if (sGameWindow) sGameWindow->mouseButtonCallback(button, action);
    }


    void
    mousePosCB(GLFWwindow*, double x, double y)
    {
      //if (sGameWindow) sGameWindow->mousePosCallback(int(x), int(y));
    }


    void
    mouseWheelCB(GLFWwindow*, double /*xoffset*/, double yoffset)
    {
      //if (sGameWindow) sGameWindow->mouseWheelCallback(int(yoffset));
    }


    void
    windowSizeCB(GLFWwindow*, int width, int height)
    {
      //if (sGameWindow) sGameWindow->windowSizeCallback(width, height);
    }


    void
    windowRefreshCB(GLFWwindow*)
    {
      //if (sGameWindow) sGameWindow->windowRefreshCallback();
    }
    
  } // namespace



  GameWindow
  init(const std::string& progName, bool background)
  {
    if (sGameWindow == nullptr) {
      std::lock_guard<std::mutex> lock(sLock);
      if (sGameWindow == nullptr) {
	OPENVDB_START_THREADSAFE_STATIC_WRITE
	  sGameWindow = new GameWindowImpl;
	OPENVDB_FINISH_THREADSAFE_STATIC_WRITE
	  }
    }
    sGameWindow->init(progName);

    if (background) {
      if (sThreadMgr == nullptr) {
	std::lock_guard<std::mutex> lock(sLock);
	if (sThreadMgr == nullptr) {
	  OPENVDB_START_THREADSAFE_STATIC_WRITE
	    sThreadMgr = new ThreadManager;
	  OPENVDB_FINISH_THREADSAFE_STATIC_WRITE
            }
      }
    } else {
      if (sThreadMgr != nullptr) {
	std::lock_guard<std::mutex> lock(sLock);
	delete sThreadMgr;
	OPENVDB_START_THREADSAFE_STATIC_WRITE
	  sThreadMgr = nullptr;
	OPENVDB_FINISH_THREADSAFE_STATIC_WRITE
	  }
    }

    return GameWindow();
  }


  void
  exit()
  {
    glfwTerminate();
  }
  


  GameWindow::GameWindow()
  {
    OPENVDB_LOG_DEBUG_RUNTIME("constructed Viewer from thread " << std::this_thread::get_id());
  }


  void
  GameWindow::open(int width, int height)
  {
    if (sGameWindow) sGameWindow->open(width, height);
  }


  void
  GameWindow::view(const openvdb::GridCPtrVec& grids)
  {
    if (sThreadMgr) {
      sThreadMgr->view(grids);
    } else if (sGameWindow) {
      //sGameWindow->view(grids);
    }
  }


  void
  GameWindow::handleEvents()
  {
    if (sGameWindow) sGameWindow->handleEvents();
  }


  void
  GameWindow::close()
  {
    if (sThreadMgr) sThreadMgr->close();
    else if (sGameWindow) sGameWindow->close();
  }


  void
  GameWindow::resize(int width, int height)
  {
    if (sGameWindow) sGameWindow->resize(width, height);
  }


  std::string
  GameWindow::getVersionString() const
  {
    std::string version;
    if (sGameWindow) version = sGameWindow->getVersionString();
    return version;
  }



  
  ThreadManager::ThreadManager()
    : mClose(false)
    , mHasThread(false)
  {
    mRedisplay = false;
  }


  void
  ThreadManager::view(const openvdb::GridCPtrVec& gridList)
  {
    if (!sGameWindow) return;

    mGrids = gridList;
    mClose = false;
    mRedisplay = true;

    if (!mHasThread) {
      mThread = std::thread(doViewTask, this);
      mHasThread = true;
    }
  }


  void
  ThreadManager::close()
  {
    if (!sGameWindow) return;

    // Tell the viewer thread to exit.
    mRedisplay = false;
    mClose = true;
    // Tell the viewer to terminate its event loop.
    sGameWindow->interrupt();

    if (mHasThread) {
      mThread.join();
      mHasThread = false;
    }

    // Tell the viewer to close its window.
    sGameWindow->close();
  }


  void
  ThreadManager::doView()
  {
    // This function runs in its own thread.
    // The mClose and mRedisplay flags are set from the main thread.
    while (!mClose) {
      // If mRedisplay was true, then set it to false
      // and then, if sViewer, call view:
      bool expected = true;
      if (mRedisplay.compare_exchange_strong(expected, false)) {
	//if (sGameWindow) sGameWindow->view(mGrids);
      }
      sGameWindow->sleep(0.5/*sec*/);
    }
  }


  //static
  void*
  ThreadManager::doViewTask(void* arg)
  {
    if (ThreadManager* self = static_cast<ThreadManager*>(arg)) {
      self->doView();
    }
    return nullptr;
  }

  

  GameWindowImpl::GameWindowImpl()
    : mDidInit(false)
      //, mCamera(new Camera)
      //, mClipBox(new ClipBox)
    , mGridIdx(0)
    , mUpdates(0)
    , mWheelPos(0)
    , mShiftIsDown(false)
    , mCtrlIsDown(false)
    , mShowInfo(true)
    , mInterrupt(false)
    , mWindow(nullptr)
  {
  }


  void
  GameWindowImpl::init(const std::string& progName)
  {
    mProgName = progName;

    if (!mDidInit) {
      struct Local {
	static void errorCB(int error, const char* descr) {
	  OPENVDB_LOG_ERROR("GLFW Error " << error << ": " << descr);
	}
      };
      glfwSetErrorCallback(Local::errorCB);
      if (glfwInit() == GL_TRUE) {
	OPENVDB_LOG_DEBUG_RUNTIME("initialized GLFW from thread "
				  << std::this_thread::get_id());
	mDidInit = true;
      } else {
	OPENVDB_LOG_ERROR("GLFW initialization failed");
      }
    }
    
    /* mViewportModule.reset(new ViewportModule); */
  }


  std::string
  GameWindowImpl::getVersionString() const
  {
    std::ostringstream ostr;

    ostr << "OpenVDB: " <<
      openvdb::OPENVDB_LIBRARY_MAJOR_VERSION << "." <<
      openvdb::OPENVDB_LIBRARY_MINOR_VERSION << "." <<
      openvdb::OPENVDB_LIBRARY_PATCH_VERSION;

    int major, minor, rev;
    glfwGetVersion(&major, &minor, &rev);
    ostr << ", " << "GLFW: " << major << "." << minor << "." << rev;

    if (mDidInit) {
      ostr << ", " << "OpenGL: ";
      std::shared_ptr<GLFWwindow> wPtr;
      GLFWwindow* w = mWindow;
      if (!w) {
	wPtr.reset(glfwCreateWindow(100, 100, "", nullptr, nullptr), &glfwDestroyWindow);
	w = wPtr.get();
      }
      if (w) {
	ostr << glfwGetWindowAttrib(w, GLFW_CONTEXT_VERSION_MAJOR) << "."
	     << glfwGetWindowAttrib(w, GLFW_CONTEXT_VERSION_MINOR) << "."
	     << glfwGetWindowAttrib(w, GLFW_CONTEXT_REVISION);
      }
    }
    return ostr.str();
  }


  bool
  GameWindowImpl::open(int width, int height)
  {
    if (mWindow == nullptr) {
      glfwWindowHint(GLFW_RED_BITS, 8);
      glfwWindowHint(GLFW_GREEN_BITS, 8);
      glfwWindowHint(GLFW_BLUE_BITS, 8);
      glfwWindowHint(GLFW_ALPHA_BITS, 8);
      glfwWindowHint(GLFW_DEPTH_BITS, 32);
      glfwWindowHint(GLFW_STENCIL_BITS, 0);

      mWindow = glfwCreateWindow(
				 width, height, mProgName.c_str(), /*monitor=*/nullptr, /*share=*/nullptr);

      OPENVDB_LOG_DEBUG_RUNTIME("created window " << std::hex << mWindow << std::dec
				<< " from thread " << std::this_thread::get_id());

      if (mWindow != nullptr) {
	// Temporarily make the new window the current context, then create a font.
	std::shared_ptr<GLFWwindow> curWindow(
					      glfwGetCurrentContext(), glfwMakeContextCurrent);
	glfwMakeContextCurrent(mWindow);
	// BitmapFont13::initialize();
      }
    }
    
    // mCamera->setWindow(mWindow);

    if (mWindow != nullptr) {
      glfwSetKeyCallback(mWindow, keyCB);
      glfwSetMouseButtonCallback(mWindow, mouseButtonCB);
      glfwSetCursorPosCallback(mWindow, mousePosCB);
      glfwSetScrollCallback(mWindow, mouseWheelCB);
      glfwSetWindowSizeCallback(mWindow, windowSizeCB);
      glfwSetWindowRefreshCallback(mWindow, windowRefreshCB);
    }
    return (mWindow != nullptr);
  }


  bool
  GameWindowImpl::isOpen() const
  {
    return (mWindow != nullptr);
  }


  // Set a flag so as to break out of the event loop on the next iteration.
  // (Useful only if the event loop is running in a separate thread.)
  void
  GameWindowImpl::interrupt()
  {
    mInterrupt = true;
    if (mWindow) glfwSetWindowShouldClose(mWindow, true);
  }


  void
  GameWindowImpl::handleEvents()
  {
    glfwPollEvents();
  }


  void
  GameWindowImpl::close()
  {
    OPENVDB_LOG_DEBUG_RUNTIME("about to close window " << std::hex << mWindow << std::dec
			      << " from thread " << std::this_thread::get_id());

    //mViewportModule.reset();
    //mRenderModules.clear();
    //mCamera->setWindow(nullptr);
    GLFWwindow* win = mWindow;
    mWindow = nullptr;
    glfwDestroyWindow(win);
    OPENVDB_LOG_DEBUG_RUNTIME("destroyed window " << std::hex << win << std::dec
			      << " from thread " << std::this_thread::get_id());
  }


  //static
  void
  GameWindowImpl::sleep(double secs)
  {
    secs = fabs(secs);
    int isecs = int(secs);
    std::this_thread::sleep_for(std::chrono::seconds(isecs));
  }



  
  void
  GameWindowImpl::resize(int width, int height)
  {
    if (mWindow) glfwSetWindowSize(mWindow, width, height);
  }
  

} // namespace game_window


