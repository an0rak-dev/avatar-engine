#ifndef AVATAR_CORE_INTERFACE
#define AVATAR_CORE_INTERFACE

/* To create a new interface, you can take exemple on the Noop interface.

The important point is to have the same set of defines to remap your new
interface functions to the generic labels.

Then, include your specific interface header between a unique set of defines
bellow.
*/

#if defined(PLATFORM_WINRT) && defined(RENDERER_VULKAN)
#include <interfaces/winvk.hpp>
#else 
#include <interfaces/noop.hpp>
#endif

#endif
