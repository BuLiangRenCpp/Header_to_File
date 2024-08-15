#pragma once

// 此文件包含了项目的一些基本信息

namespace htf
{

#if _WIN32
#define HTF_PROGRAM "htf.exe"
#else
#define HTF_PROGRAM "htf"
#endif

// 版本号
#define HTF_VERSION_MAJOR "1"
#define HTF_VERSION_MINOR "1"
#define HTF_VERSION_PATCH "0"
#define HTF_VERSION HTF_VERSION_MAJOR "." HTF_VERSION_MINOR "." HTF_VERSION_PATCH

}   // namespace htf