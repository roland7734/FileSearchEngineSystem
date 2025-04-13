#include "utils/MagicWrapper.hpp"
#include "config/config.hpp"
#include <stdexcept>
#include <mutex>

std::once_flag MagicWrapper::initFlag;
MagicWrapper::MagicHandleRAII* MagicWrapper::sharedHandle = nullptr;

MagicWrapper::MagicHandleRAII::MagicHandleRAII() {
    handle = magic_open(MAGIC_MIME_TYPE);
    if (!handle) {
        throw std::runtime_error("Failed to open magic handle.");
    }

    if (magic_load(handle, Config::MAGIC_DB_PATH) != 0) {
        std::string err = magic_error(handle);
        magic_close(handle);
        handle = nullptr;
        throw std::runtime_error("Failed to load magic DB: " + err);
    }
}

MagicWrapper::MagicHandleRAII::~MagicHandleRAII() {
    if (handle) {
        magic_close(handle);
        handle = nullptr;
    }
}

MagicWrapper::MagicWrapper() {
    std::call_once(initFlag, []() {
        sharedHandle = new MagicHandleRAII();
    });
}

MagicWrapper::~MagicWrapper() {
}

MagicWrapper& MagicWrapper::instance() {
    static MagicWrapper instance;
    return instance;
}

std::string MagicWrapper::detectMimeType(const std::string& filePath) const {
    magic_t handle = sharedHandle->handle;
    const char* mime = magic_file(handle, filePath.c_str());
    if (!mime || std::string(mime).find("cannot open") != std::string::npos) {
        return "application/octet-stream";
    }

    return std::string(mime);
}
