#pragma once

#ifdef DEBUG

#include <cassert>
#define CAT_ASSERT(expression) assert(expression)

// An assert which can print a text message
#define CAT_ASSERT_MSG(expression, text_message) \
CAT_ASSERT(expression && text_message)

// A static assert
#define CAT_ASSERT_STATIC_IS_TRIVIALLY_COPYABLE(T) \
static_assert(std::is_trivially_copyable_v<T>, "Type is not trivially copyable")

#endif