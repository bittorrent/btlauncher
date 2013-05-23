/**********************************************************\

  Auto-generated btlauncher.h

  This file contains the auto-generated main plugin object
  implementation for the BitTorrent Launcher project

\**********************************************************/
#ifndef H_btlauncherPLUGIN
#define H_btlauncherPLUGIN

#ifdef WIN32
#include <windows.h>
#include <wininet.h>
#endif

#include "PluginWindow.h"
#include "PluginEvents/MouseEvents.h"
#include "PluginEvents/AttachedEvent.h"

#include "PluginCore.h"
#include "json/json.h"
#include "fbjson.h"

#include "svn_info.h"
#include "PluginVersion.h"

#ifdef _DEBUG
#define CRASH_PING_HOST "bench.staging.utorrent.com"
#else
#define CRASH_PING_HOST "bench.utorrent.com"
#endif


FB_FORWARD_PTR(btlauncher)
class btlauncher : public FB::PluginCore
{
public:
	static std::string pluginName;
#ifdef WIN32
	static HMODULE inetdll;
#endif
	static std::string version;
	static std::string svnRevision;
	static std::string svnDate;

	static void StaticInitialize();
    static void StaticDeinitialize();
	static void ThreadPostJsonToBench(const std::string &action, FB::VariantMap &data);
	static std::string GetLogFilePathName();
	static void DeleteLogFile();

public:
    btlauncher();
    virtual ~btlauncher();

public:
    void onPluginReady();
    void shutdown();
    virtual FB::JSAPIPtr createJSAPI();
    // If you want your plugin to always be windowless, set this to true
    // If you want your plugin to be optionally windowless based on the
    // value of the "windowless" param tag, remove this method or return
    // FB::PluginCore::isWindowless()
    virtual bool isWindowless() { return false; }

    BEGIN_PLUGIN_EVENT_MAP()
        EVENTTYPE_CASE(FB::MouseDownEvent, onMouseDown, FB::PluginWindow)
        EVENTTYPE_CASE(FB::MouseUpEvent, onMouseUp, FB::PluginWindow)
        EVENTTYPE_CASE(FB::MouseMoveEvent, onMouseMove, FB::PluginWindow)
        EVENTTYPE_CASE(FB::MouseMoveEvent, onMouseMove, FB::PluginWindow)
        EVENTTYPE_CASE(FB::AttachedEvent, onWindowAttached, FB::PluginWindow)
        EVENTTYPE_CASE(FB::DetachedEvent, onWindowDetached, FB::PluginWindow)
    END_PLUGIN_EVENT_MAP()

    /** BEGIN EVENTDEF -- DON'T CHANGE THIS LINE **/
    virtual bool onMouseDown(FB::MouseDownEvent *evt, FB::PluginWindow *);
    virtual bool onMouseUp(FB::MouseUpEvent *evt, FB::PluginWindow *);
    virtual bool onMouseMove(FB::MouseMoveEvent *evt, FB::PluginWindow *);
    virtual bool onWindowAttached(FB::AttachedEvent *evt, FB::PluginWindow *);
    virtual bool onWindowDetached(FB::DetachedEvent *evt, FB::PluginWindow *);
    /** END EVENTDEF -- DON'T CHANGE THIS LINE **/
};


#endif

