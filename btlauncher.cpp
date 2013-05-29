/**********************************************************\

  Auto-generated btlauncher.cpp

  This file contains the auto-generated main plugin object
  implementation for the BitTorrent Launcher project

\**********************************************************/

#include <fstream>

#ifdef FB_WIN
#include "Win/btlauncherAPI.h"
#else
#include "btlauncherAPI.h"
#endif

#include "btlauncher.h"

// static members to use for bench communications
std::string btlauncher::pluginName;
#ifdef WIN32
HMODULE btlauncher::inetdll;
#endif
std::string btlauncher::version;
std::string btlauncher::svnRevision;
std::string btlauncher::svnDate;

char* itoascii(long num)
{
    static char text[15];
    sprintf(text,"%ld",num);
    return (text);
}

#ifdef WIN32
/**
	lpParam should point to a std::string type with the text to be posted to bench.
	Pass in a pointer to a dynamically allocated string (in lpParm) and be delete 
	the string object before leaving the thread.
*/
DWORD WINAPI SendFunction(LPVOID lpParam)
{
	HINTERNET inet, isession, hReq;
	std::string* serializedMsg = (std::string *)lpParam;
	if(inet = InternetOpenA("BitTorrent Plugin", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0)){
		if(isession = InternetConnectA(inet, CRASH_PING_HOST, INTERNET_DEFAULT_HTTP_PORT ,"" ,"", INTERNET_SERVICE_HTTP, 0, 0)){
			if(hReq = HttpOpenRequestA(isession, "POST", "/e?i=40", NULL, NULL, NULL, INTERNET_FLAG_NO_CACHE_WRITE, 0)){
				HttpSendRequestA(hReq, NULL, 0, (LPVOID)(serializedMsg->c_str()), serializedMsg->size());
			}
		}
	}
	delete serializedMsg;
	return 0;
}

typedef LONG (WINAPI *MYEXCEPTIONHANDLER)(_EXCEPTION_POINTERS *ExceptionInfo);

LONG WINAPI MyExceptionHandler(_EXCEPTION_POINTERS *ExceptionInfo)
{
	FB::Log::stopLogging();
	// open the log file
	std::ifstream ifs(btlauncher::GetLogFilePathName().c_str());

	FB::VariantMap data;
	if(ifs){
		// read logfile into a string
		std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
		data["log"] = content;
	}
	else{
		data["log"] = "Logfile not available";
	}
	ifs.close();

	// associate the exception code with text description
	std::string exceptionText;
	switch(ExceptionInfo->ExceptionRecord->ExceptionCode){
		case EXCEPTION_ACCESS_VIOLATION: exceptionText = "EXCEPTION_ACCESS_VIOLATION"; break;
		case EXCEPTION_ARRAY_BOUNDS_EXCEEDED: exceptionText = "EXCEPTION_ARRAY_BOUNDS_EXCEEDED"; break;
		case EXCEPTION_BREAKPOINT: exceptionText = "EXCEPTION_BREAKPOINT"; break;
		case EXCEPTION_DATATYPE_MISALIGNMENT: exceptionText = "EXCEPTION_DATATYPE_MISALIGNMENT"; break;
		case EXCEPTION_FLT_DENORMAL_OPERAND: exceptionText = "EXCEPTION_FLT_DENORMAL_OPERAND"; break;
		case EXCEPTION_FLT_DIVIDE_BY_ZERO: exceptionText = "EXCEPTION_FLT_DIVIDE_BY_ZERO"; break;
		case EXCEPTION_FLT_INEXACT_RESULT: exceptionText = "EXCEPTION_FLT_INEXACT_RESULT"; break;
		case EXCEPTION_FLT_INVALID_OPERATION: exceptionText = "EXCEPTION_FLT_INVALID_OPERATION"; break;
		case EXCEPTION_FLT_OVERFLOW: exceptionText = "EXCEPTION_FLT_OVERFLOW"; break;
		case EXCEPTION_FLT_STACK_CHECK: exceptionText = "EXCEPTION_FLT_STACK_CHECK"; break;
		case EXCEPTION_FLT_UNDERFLOW: exceptionText = "EXCEPTION_FLT_UNDERFLOW"; break;
		case EXCEPTION_ILLEGAL_INSTRUCTION: exceptionText = "EXCEPTION_ILLEGAL_INSTRUCTION"; break;
		case EXCEPTION_IN_PAGE_ERROR: exceptionText = "EXCEPTION_IN_PAGE_ERROR"; break;
		case EXCEPTION_INT_DIVIDE_BY_ZERO: exceptionText = "EXCEPTION_INT_DIVIDE_BY_ZERO"; break;
		case EXCEPTION_INT_OVERFLOW: exceptionText = "EXCEPTION_INT_OVERFLOW"; break;
		case EXCEPTION_INVALID_DISPOSITION: exceptionText = "EXCEPTION_INVALID_DISPOSITION"; break;
		case EXCEPTION_NONCONTINUABLE_EXCEPTION: exceptionText = "EXCEPTION_NONCONTINUABLE_EXCEPTION"; break;
		case EXCEPTION_PRIV_INSTRUCTION: exceptionText = "EXCEPTION_PRIV_INSTRUCTION"; break;
		case EXCEPTION_SINGLE_STEP: exceptionText = "EXCEPTION_SINGLE_STEP"; break;
		case EXCEPTION_STACK_OVERFLOW: exceptionText = "EXCEPTION_STACK_OVERFLOW"; break;
		default: exceptionText = "Exception code not defined";
	}
	data["exception_text"] = exceptionText;

	// see if more info is available
	switch(ExceptionInfo->ExceptionRecord->ExceptionCode){
		case EXCEPTION_ACCESS_VIOLATION:
		case EXCEPTION_IN_PAGE_ERROR:
			std::string readWriteFlagText;
			if(ExceptionInfo->ExceptionRecord->ExceptionInformation[0] == 0) readWriteFlagText = "read inaccessible address";
			else if(ExceptionInfo->ExceptionRecord->ExceptionInformation[0] == 1) readWriteFlagText = "write inaccessible address";
			else if(ExceptionInfo->ExceptionRecord->ExceptionInformation[0] == 8) readWriteFlagText = "user-mode data execution prevention";
			else readWriteFlagText = "unknown flag value";
			data["read_write_flag"] = readWriteFlagText;
			data["rw_address"] = itoascii(ExceptionInfo->ExceptionRecord->ExceptionInformation[1]);
	}

	// send the crash info
	btlauncher::ThreadPostJsonToBench("exception", data);

	btlauncher::DeleteLogFile();
	// We should not trigger the Windows exception handler
	return EXCEPTION_EXECUTE_HANDLER;
}

void uncaught_exception()
{
	dataMap["action"] = "uncaught c++ exception";
	dataMap["eventName"] = btlauncher::pluginName;
	dataMap["version"] = btlauncher::version;
	dataMap["svn_revision"] = btlauncher::svnRevision;
	dataMap["svn_date"] = btlauncher::svnDate;
    
	// Create a Json::Value object which will store the contents of the VariantMap
	Json::Value json_value = FB::variantToJsonValue(dataMap);
    
	// Create a StyledWriter which will convert our Json::Value to a styled JSON string
	Json::StyledWriter writer;
    
	// Use the writer to write out the string
	std::string* serializedJsonStr = new std::string(writer.write(json_value));

    SendFunction(serializedJsonStr);
}

void InstallExceptionHandler(MYEXCEPTIONHANDLER ExceptionHandler)
{
	SetUnhandledExceptionFilter(ExceptionHandler);

    std::set_terminate(uncaught_exception);

#if defined _M_X64
	// Prevent the MS CRT from installing a handler that overrides our handler
	// This swallows some (all) exceptions, and sends them to WER
	// Only use this epic hack on the x64 build.
	// We use the ancient MS CRT in the 32-bit build, which doesn't suffer from this problem.
	BOOL prevented = PreventSetUnhandledExceptionFilter(&MyDummySetUnhandledExceptionFilter);
#endif
}
#else

void uncaught_exception()
{
    fprintf(stderr, "uncaught exception\n");
}

#endif // WIN32

std::string btlauncher::GetLogFilePathName()
{
	std::string path = FB::System::getTempPath();
	path += "\\bt_plugin.log";
	return path;
}

#ifndef DEBUG
inline
#endif
void btlauncher::DeleteLogFile()
{
#ifndef DEBUG
	FB::Log::stopLogging();
	std::string path = GetLogFilePathName();
	remove(path.c_str());
#endif
}

void btlauncher::ThreadPostJsonToBench(const std::string &action, FB::VariantMap &dataMap)
{
	dataMap["action"] = action;
	dataMap["eventName"] = btlauncher::pluginName;
	dataMap["version"] = btlauncher::version;
	dataMap["svn_revision"] = btlauncher::svnRevision;
	dataMap["svn_date"] = btlauncher::svnDate;

	// Create a Json::Value object which will store the contents of the VariantMap
	Json::Value json_value = FB::variantToJsonValue(dataMap);

	// Create a StyledWriter which will convert our Json::Value to a styled JSON string
	Json::StyledWriter writer;

	// Use the writer to write out the string
	std::string* serializedJsonStr = new std::string(writer.write(json_value));

#ifdef WIN32
	// the thread will need to delete the string object when it is finished
	CreateThread(NULL, 0, SendFunction, serializedJsonStr, 0, NULL);
#else
    // TODO: implement pthread variant
#endif
}


///////////////////////////////////////////////////////////////////////////////
/// @fn btlauncher::StaticInitialize()
///
/// @brief  Called from PluginFactory::globalPluginInitialize()
///
/// @see FB::FactoryBase::globalPluginInitialize
///////////////////////////////////////////////////////////////////////////////
void btlauncher::StaticInitialize()
{
	// Place one-time initialization stuff here; As of FireBreath 1.4 this should only
    // be called once per process
#ifdef WIN32
	inetdll = LoadLibrary(_T("wininet.dll"));
	InstallExceptionHandler(MyExceptionHandler);
#else
    std::set_terminate(uncaught_exception);
#endif

	btlauncher::version = itoascii(MAJOR);
	btlauncher::version += ".";
	btlauncher::version += itoascii(MINOR);
	btlauncher::version += ".";
	btlauncher::version += itoascii(TINY);

	btlauncher::svnDate = SVN_REVISION_DATE;
	btlauncher::svnRevision = itoascii(SVN_REVISION);

#ifdef SHARE
	pluginName = "SoSharePlugin";
#elif TORQUE
	pluginName = "TorquePlugin";
#else
	pluginName = "no_plugin_name_defined";
#endif

	FB::VariantMap data;
	ThreadPostJsonToBench("StaticInitialize", data);
}

///////////////////////////////////////////////////////////////////////////////
/// @fn btlauncher::StaticInitialize()
///
/// @brief  Called from PluginFactory::globalPluginDeinitialize()
///
/// @see FB::FactoryBase::globalPluginDeinitialize
///////////////////////////////////////////////////////////////////////////////
void btlauncher::StaticDeinitialize()
{
    // Place one-time deinitialization stuff here. As of FireBreath 1.4 this should
    // always be called just before the plugin library is unloaded
#ifdef WIN32
	FreeLibrary(inetdll);
#endif
	FB::VariantMap data;
	ThreadPostJsonToBench("StaticDeinitialize", data);
}

///////////////////////////////////////////////////////////////////////////////
/// @brief  btlauncher constructor.  Note that your API is not available
///         at this point, nor the window.  For best results wait to use
///         the JSAPI object until the onPluginReady method is called
///////////////////////////////////////////////////////////////////////////////
btlauncher::btlauncher()
{
}

///////////////////////////////////////////////////////////////////////////////
/// @brief  btlauncher destructor.
///////////////////////////////////////////////////////////////////////////////
btlauncher::~btlauncher()
{
    // This is optional, but if you reset m_api (the shared_ptr to your JSAPI
    // root object) and tell the host to free the retained JSAPI objects then
    // unless you are holding another shared_ptr reference to your JSAPI object
    // they will be released here.
	FBLOG_INFO("btlauncher::~btlauncher()", "The plugin is being destructed");
    releaseRootJSAPI();
    m_host->freeRetainedObjects();
}

void btlauncher::onPluginReady()
{
    // When this is called, the BrowserHost is attached, the JSAPI object is
    // created, and we are ready to interact with the page and such.  The
    // PluginWindow may or may not have already fire the AttachedEvent at
    // this point.
	FBLOG_INFO("btlauncher::onPluginReady()", "The plugin is ready to use");

	FB::VariantMap data;
	ThreadPostJsonToBench("onPluginReady", data);
}

void btlauncher::shutdown()
{
    // This will be called when it is time for the plugin to shut down;
    // any threads or anything else that may hold a shared_ptr to this
    // object should be released here so that this object can be safely
    // destroyed. This is the last point that shared_from_this and weak_ptr
    // references to this object will be valid
	FBLOG_INFO("btlauncher::shutdown()", "The plugin is being shut down");
	FB::VariantMap data;
	ThreadPostJsonToBench("shutdown", data);
	DeleteLogFile();
}

///////////////////////////////////////////////////////////////////////////////
/// @brief  Creates an instance of the JSAPI object that provides your main
///         Javascript interface.
///
/// Note that m_host is your BrowserHost and shared_ptr returns a
/// FB::PluginCorePtr, which can be used to provide a
/// boost::weak_ptr<btlauncher> for your JSAPI class.
///
/// Be very careful where you hold a shared_ptr to your plugin class from,
/// as it could prevent your plugin class from getting destroyed properly.
///////////////////////////////////////////////////////////////////////////////
FB::JSAPIPtr btlauncher::createJSAPI()
{
    // m_host is the BrowserHost
    return boost::make_shared<btlauncherAPI>(FB::ptr_cast<btlauncher>(shared_from_this()), m_host);
}

bool btlauncher::onMouseDown(FB::MouseDownEvent *evt, FB::PluginWindow *)
{
    //printf("Mouse down at: %d, %d\n", evt->m_x, evt->m_y);
    return false;
}

bool btlauncher::onMouseUp(FB::MouseUpEvent *evt, FB::PluginWindow *)
{
    //printf("Mouse up at: %d, %d\n", evt->m_x, evt->m_y);
    return false;
}

bool btlauncher::onMouseMove(FB::MouseMoveEvent *evt, FB::PluginWindow *)
{
    //printf("Mouse move at: %d, %d\n", evt->m_x, evt->m_y);
    return false;
}
bool btlauncher::onWindowAttached(FB::AttachedEvent *evt, FB::PluginWindow *)
{
    // The window is attached; act appropriately
    return false;
}

bool btlauncher::onWindowDetached(FB::DetachedEvent *evt, FB::PluginWindow *)
{
    // The window is about to be detached; act appropriately
    return false;
}

