#include "BuildConfig.h"

#define XSTRINGIFY(a) STRINGIFY(a)
#define STRINGIFY(a) #a

namespace IbPerfLib {

#ifdef IBPERFLIB_VERSION
const char *BuildConfig::VERSION = XSTRINGIFY(IBPERFLIB_VERSION);
#else
const char *BuildConfig::VERSION = "v0.0.0";
#endif

#ifdef IBPERFLIB_GIT_REV
const char *BuildConfig::GIT_REV = XSTRINGIFY(IBPERFLIB_GIT_REV);
#else
const char *BuildConfig::GIT_REV = "unknown";
#endif

#ifdef IBPERFLIB_BUILD_DATE
const char *BuildConfig::BUILD_DATE = XSTRINGIFY(IBPERFLIB_BUILD_DATE);
#else
const char *BuildConfig::BUILD_DATE = "0000-00-00 00:00:00";
#endif

}

#undef STRINGIFY
#undef XSTRINGIFY