#ifndef MAGICWRAPPER_HPP
#define MAGICWRAPPER_HPP

#include <string>
#include <magic.h>
#include <mutex>

class MagicWrapper {
public:
    static MagicWrapper& instance();

    std::string detectMimeType(const std::string& filePath) const;

private:
    MagicWrapper();
    ~MagicWrapper();

    MagicWrapper(const MagicWrapper&) = delete;
    MagicWrapper& operator=(const MagicWrapper&) = delete;

    struct MagicHandleRAII {
        magic_t handle;
        MagicHandleRAII();
        ~MagicHandleRAII();
    };

    static std::once_flag initFlag;
    static MagicHandleRAII* sharedHandle;
};

#endif // MAGICWRAPPER_HPP
