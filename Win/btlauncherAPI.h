#include <string>
#include <stdio.h>
#include <string.h>
#include <atlbase.h>
#include <atlstr.h>
#include <string.h>
#include <boost/weak_ptr.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/random_generator.hpp>

#include "JSAPIAuto.h"
#include "BrowserHost.h"
#include "../btlauncher.h"
#include "SystemHelpers.h"
#include "SimpleStreamHelper.h"

#include "variant_list.h"

#ifndef H_btlauncherAPI
#define H_btlauncherAPI

class btlauncherAPI : public FB::JSAPIAuto
{
public:
	btlauncherAPI(const btlauncherPtr& plugin, const FB::BrowserHostPtr& host);
	virtual ~btlauncherAPI();

	btlauncherPtr getPlugin();

	// Read/Write property ${PROPERTY.ident}
	std::string get_testString();
	void set_testString(const std::string& val);

	// Read-only property ${PROPERTY.ident}
	std::string get_version();

	// Method echo
	void ajax(const std::string& url, const FB::JSObjectPtr& callback);
#ifndef CHROME
	void checkForUpdate(const FB::JSObjectPtr& callback);
	void gotCheckForUpdate(boost::uint32_t id, const FB::JSObjectPtr& callback, bool success,
		const FB::HeaderMap& headers,
		const boost::shared_array<uint8_t>& data,
		const size_t size);
#endif //CHROMEd
	int getPID();
	void downloadProgram(const std::wstring& val, const FB::JSObjectPtr& callback);
	std::wstring getInstallPath(const std::wstring& val);
	std::wstring getInstallVersion(const std::wstring& val);
	FB::variant runProgram(const std::wstring& val, const FB::JSObjectPtr& callback);
	FB::variant enablePairing(const std::wstring& val, const std::wstring& key);
	FB::variant pair(const std::wstring& val);
	FB::variant isRunning(const std::wstring& val);
	FB::variant stopRunning(const std::wstring& val);

	// Method test-event
	void btlauncherAPI::do_callback(const FB::JSObjectPtr& callback, const std::vector<FB::variant>& args);

private:
	bool isSupported(std::wstring program);

	void gotajax(boost::uint32_t id,
		FB::JSObjectPtr callback,
		bool success,
		const FB::HeaderMap& headers,
		const boost::shared_array<uint8_t>& data,
		const size_t size);

	void gotDownloadProgram(boost::uint32_t id,
		const FB::JSObjectPtr& callback,
		std::wstring& program,
		bool success,
		const FB::HeaderMap& headers,
		const boost::shared_array<uint8_t>& data,
		const size_t size);
	btlauncherWeakPtr m_plugin;
	FB::BrowserHostPtr m_host;

	std::string m_testString;

	boost::uint32_t m_next_request_id;

	// outstanding requests. If we are asked to shut down, cancel them
	std::map<boost::uint32_t, FB::SimpleStreamHelperPtr> m_outstanding_requests;
};

#endif // H_btlauncherAPI

