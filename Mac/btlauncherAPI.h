#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string>
#include <sstream>
#include <boost/weak_ptr.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/random_generator.hpp>

#include "JSAPIAuto.h"
#include "BrowserHost.h"
#include "../btlauncher.h"
#include "SystemHelpers.h"
#include "SimpleStreamHelper.h"

#ifndef H_btlauncherAPI
#define H_btlauncherAPI

typedef struct kinfo_proc kinfo_proc;

class btlauncherAPI : public FB::JSAPIAuto
{
public:
	btlauncherAPI(const btlauncherPtr& plugin, const FB::BrowserHostPtr& host);
	virtual ~btlauncherAPI();

	btlauncherPtr getPlugin();

	// Read-only property ${PROPERTY.ident}
	std::string get_version();

	// Method echo
	void ajax(const std::string& url, const FB::JSObjectPtr& callback);
	void checkForUpdate(const FB::JSObjectPtr& callback);
	void downloadProgram(const std::string& val, const FB::JSObjectPtr& callback);
	std::string getInstallPath(const std::string& val);
	std::string getInstallVersion(const std::string& val);
	FB::variant runProgram(const std::string& program, const FB::JSObjectPtr& callback);
	void gotCheckForUpdate(const FB::JSObjectPtr& callback, bool success
		, const FB::HeaderMap& header, const boost::shared_array<uint8_t>& data
		, const size_t size);
	FB::VariantList isRunning(const std::string& val);
	FB::VariantList stopRunning(const std::string& val);

private:
	int GetBSDProcessList(kinfo_proc **procList, size_t *procCount);
	std::string installPath;
	btlauncherWeakPtr m_plugin;
	FB::BrowserHostPtr m_host;
	int m_outstanding_ajax_requests;

	// the next request ID to use
	boost::uint32_t m_ajax_request_id;

	// maps a request ID to a callback object. It is necessary
	// to store these this way so that we can abort them when
	// shutting down
	std::map<boost::uint32_t, FB::JSObjectPtr> m_outstanding_ajax;

	void gotDownloadProgram(const FB::JSObjectPtr& callback, 
		std::string& program,
		bool success,
		const FB::HeaderMap& headers,
		const boost::shared_array<uint8_t>& data,
		const size_t size);

	void gotajax(boost::uint32_t,
		bool success,
		const FB::HeaderMap& headers,
		const boost::shared_array<uint8_t>& data,
		const size_t size);
};

#endif // H_btlauncherAPI

