# GLSLCompiler

GLSLCompiler package for sublime text, using Opengl 3.3.

This package adds a .glsl file build-system. 

It will build glsl code with shipped binary, and print errors if there's any.

It will search for keywords "vs", "vertex", "fs", "fragment", "ps", "pixel" to determine if input file is vertex shader or not.

If no key words were found. It will pass #define VERTEX_SHADER / FRAGMENT_SHADER and compile the same shader twice.

## OSX

Install library sfml from home-brew to use shipped binary directlly.

Or you can build the executable yourself.

## Windows

Nothing todo so far.
