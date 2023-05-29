#ifndef AVATAR_CORE_INTERFACE
#define AVATAR_CORE_INTERFACE

/* To create a new interface, you can take exemple on the Noop interface.

The important point is to have the same set of defines to remap your new
interface functions to the generic labels.

Then, include your specific interface header between a unique set of defines
bellow.
*/

#if defined(PLATFORM_WINRT) && defined(RENDERER_DX12)
#include <avatar/interfaces/windx12.hpp>
#else 
#include <avatar/interfaces/noop.hpp>
#endif

#endif
