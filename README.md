# GLSLCompiler

<<<<<<< HEAD
GLSLCompiler package for sublime text, using Opengl 3.3.
=======
GLSLCompiler package for sublime text.
>>>>>>> master

This package adds a .glsl file build-system. 

It builds glsl code with shipped binary, and print errors if there's any.

We use keywords "vs", "vertex", "fs", "fragment", "ps", "pixel" to determine if the input file is vertex shader or not.

If no keyword were found. It will pass #define VERTEX_SHADER / FRAGMENT_SHADER and compile the same shader twice.

## OSX

Install library sfml from home-brew to use shipped binary directlly.

Or you can build the executable by yourself.

<<<<<<< HEAD
## Windows

Nothing todo so far.
=======
If you encounter permission issues, just call 'chmod 755 path\to\glslcompiler'.

## Windows

Nothing todo so far.
>>>>>>> master
