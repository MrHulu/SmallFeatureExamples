#include <iostream>

#include <client/crash_report_database.h>
#include <client/settings.h>
#include <client/crashpad_client.h>

bool initCrashPad()
{
	
	using namespace crashpad;
	std::map<std::string, std::string> annotations;
    annotations["format"] = "minidump";       // 设置生成minidump
    // annotations["product"] = "CrashpadExample";
    // annotations["database"] = "Fred";         // 附加到所有报告的注释

	std::vector<std::string> arguments;
	arguments.push_back("--no-rate-limit");

	std::string url("http://127.0.0.1:8877/upload");
	
	//放dump的文件夹 按需改
	base::FilePath db(L"./crashpad");
	//crashpad_handler.exe 按需改
	base::FilePath handler(L"G:/HuluMan/csdn/CrashpadExample/build/crashpad_handler.exe");

	std::unique_ptr<CrashReportDatabase> database =
		crashpad::CrashReportDatabase::Initialize(db);

	if (database == nullptr || database->GetSettings() == NULL)
	{
		std::cout << "CrashReportDatabase Init Error" << std::endl;
		return false;
	}

	database->GetSettings()->SetUploadsEnabled(true);

	CrashpadClient client;
	return client.StartHandler(handler,
		db,
		db,
		url,
		annotations,
		arguments,
		true,
		true);
}

 int main() {
    using namespace base;
    using namespace crashpad;
    std::cout << "Hello, World!" << std::endl;

    if(initCrashPad()) {
        std::cout << "CrashpadHandler::InitializeCrashpad success" << std::endl;
        int* a = nullptr;
        *a = 1;
    }else {
        std::cout << "CrashpadHandler::InitializeCrashpad failed" << std::endl;
        return 123;
    }
}
