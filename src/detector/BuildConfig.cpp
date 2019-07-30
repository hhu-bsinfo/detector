#include <cstdio>
#include "BuildConfig.h"

#define XSTRINGIFY(a) STRINGIFY(a)
#define STRINGIFY(a) #a

namespace Detector {

const constexpr char BuildConfig::banner[];

#ifdef DETECTOR_VERSION
const char *BuildConfig::VERSION = XSTRINGIFY(DETECTOR_VERSION);
#else
const char *BuildConfig::VERSION = "v0.0.0";
#endif

#ifdef DETECTOR_GIT_REV
const char *BuildConfig::GIT_REV = XSTRINGIFY(DETECTOR_GIT_REV);
#else
const char *BuildConfig::GIT_REV = "unknown";
#endif

#ifdef DETECTOR_GIT_BRANCH
const char *BuildConfig::GIT_BRANCH = XSTRINGIFY(DETECTOR_GIT_BRANCH);
#else
const char *BuildConfig::GIT_BRANCH = "unknown";
#endif

#ifdef DETECTOR_BUILD_DATE
const char *BuildConfig::BUILD_DATE = XSTRINGIFY(DETECTOR_BUILD_DATE);
#else
const char *BuildConfig::BUILD_DATE = "0000-00-00 00:00:00";
#endif

const bool BuildConfig::ADDITIONAL_EXTENDED_COUNTERS_ENABLED = USE_ADDITIONAL_EXTENDED_COUNTERS == 1;

void BuildConfig::printBanner() {
    printf("\n");
    printf(banner, VERSION, BUILD_DATE, GIT_BRANCH, GIT_REV, ADDITIONAL_EXTENDED_COUNTERS_ENABLED ? "Enabled" : "Disabled");
    printf("\n\n");
}

}

#undef STRINGIFY
#undef XSTRINGIFY