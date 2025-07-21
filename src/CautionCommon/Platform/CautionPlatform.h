#pragma once

#include <memory>

#ifdef CAUTION_COMMON_EXPORT
#define CAUTIONCOMMON_API __declspec(dllexport)
#else
#define CAUTIONCOMMON_API __declspec(dllimport)
#endif


#define DECLARE_SHARED_POINTERS(name)                   \
  typedef std::shared_ptr<name> name ##       SharedPtr;      \
  typedef std::shared_ptr<const name> name ## ConstSharedPtr; \
  typedef std::weak_ptr<name> name ##         WeakPtr;  \
  typedef std::weak_ptr<const name> name ##   ConstWeakPtr;


#define CAT_DEPRECATED(message) __declspec(deprecated(message))