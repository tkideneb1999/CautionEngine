#pragma once

#include <memory>

#define DECLARE_SHARED_POINTERS(name)                   \
  typedef std::shared_ptr<name> name ##       SharedPtr;      \
  typedef std::shared_ptr<const name> name ## ConstSharedPtr; \
  typedef std::weak_ptr<name> name ##         WeakPtr;  \
  typedef std::weak_ptr<const name> name ##   ConstWeakPtr;