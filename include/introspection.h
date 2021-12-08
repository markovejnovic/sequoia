#ifndef _INTROSPECTION_H_
#define _INTROSPECTION_H_

#include <string>
#include <boost/lexical_cast.hpp>

#ifndef __GNUC__
#error "Anything but the GCC compiler is not supported."
#endif

#define __FUNCTION_NAME__ __FUNCTION__

#endif  // _INTROSPECTION_H_
