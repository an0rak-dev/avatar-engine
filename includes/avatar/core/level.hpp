#ifndef AVATAR_CORE_LEVEL
#define AVATAR_CORE_LEVEL

#include <avatar/core/scene.hpp>

/**
 Defines the basic components of a single game level.

 A game level represents an aggregation of structure to
 describe a scene, the protagonists in it, as well as the
 specific rules, if any.
*/
typedef struct level {
		/* The scene containing all the objects to render on screen */
		av_scene scene;

} av_level;

#endif
