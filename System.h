#pragma once
#include <string_view>
class System {
public: 
    System();
	~System();
    std::string_view getVersionString() const noexcept;
    static void onError(int code, const char* description) noexcept;
private:
    void logDiagnostics() const noexcept;
};