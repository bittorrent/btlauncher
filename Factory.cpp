/**********************************************************\ 
 
 Auto-generated Factory.cpp
 
 This file contains the auto-generated factory methods 
 for the btlauncher project
 
\**********************************************************/

#include "FactoryBase.h"
#include "btlauncher.h"
#include <boost/make_shared.hpp>
#include "SystemHelpers.h"

class PluginFactory : public FB::FactoryBase
{
public:
    ///////////////////////////////////////////////////////////////////////////////
    /// @fn FB::PluginCorePtr createPlugin(const std::string& mimetype)
    ///
    /// @brief  Creates a plugin object matching the provided mimetype
    ///         If mimetype is empty, returns the default plugin
    ///////////////////////////////////////////////////////////////////////////////
    FB::PluginCorePtr createPlugin(const std::string& mimetype)
    {
        return boost::make_shared<btlauncher>();
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    /// @see FB::FactoryBase::globalPluginInitialize
    ///////////////////////////////////////////////////////////////////////////////
    void globalPluginInitialize()
    {
        btlauncher::StaticInitialize();
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    /// @see FB::FactoryBase::globalPluginDeinitialize
    ///////////////////////////////////////////////////////////////////////////////
    void globalPluginDeinitialize()
    {
        btlauncher::StaticDeinitialize();
    }
	
	void getLoggingMethods( FB::Log::LogMethodList& outMethods )
	{
#ifdef DEBUG
		// The next line will enable logging to the console (think: printf).
		outMethods.push_back(std::make_pair(FB::Log::LogMethod_Console, std::string()));
#endif		
		// The next line will enable logging to a logfile.
		outMethods.push_back(std::make_pair(FB::Log::LogMethod_File, btlauncher::GetLogFilePathName()));
		
		// Obviously, if you use both lines, you will get output on both sinks.
	}

	/*
    // All log messages, no matter how small
    LogLevel_Trace  = 0x01,
    // Debug level log messages - messages you won't care about unless you're debugging
    LogLevel_Debug  = 0x02,
    // Informational log messages - not critical to know, but you might care
    LogLevel_Info   = 0x04,
    // Only log warning and worse messages
    LogLevel_Warn   = 0x08,
    // Only log messages that are actual errors
    LogLevel_Error  = 0x10
	*/
	FB::Log::LogLevel getLogLevel()
	{
		return FB::Log::LogLevel_Debug;
	}
};

///////////////////////////////////////////////////////////////////////////////
/// @fn getFactoryInstance()
///
/// @brief  Returns the factory instance for this plugin module
///////////////////////////////////////////////////////////////////////////////
FB::FactoryBasePtr getFactoryInstance()
{
    static boost::shared_ptr<PluginFactory> factory = boost::make_shared<PluginFactory>();
    return factory;
}

