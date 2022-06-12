## Description

Raytracer in a pixel shader. The scene contains the flag that is partially covered in fog. The player has to move the camera/remove the fog to get the full flag.

Shader is compiled into DirectX `ps_3_0` and the executable is packed with Crinkler, just like a regular demo scene.

## Building

* VS2019
* Crinkler
    * Move it to solution folder as `link.exe` to use Crinkler instead of msbuild's linker.
* DirectX 9

## Credits

1. Inigo Quilez who's code I heavily referenced to write this
    * https://madethisthing.com/iq/Demo-Framework-4k

2. Inigo Quilez who's distance functions I wholesale copied from
    * https://www.iquilezles.org/www/articles/distfunctions/distfunctions.htm

## Stuff I changed from Quilez's template

1. Ported to `ps_3_0`, needed to create a pass through vertex shader.
2. Release build now supports not-full-screen.
3. Precompile the shaders instead of compiling runtime.
3. Feed shader the time, mouse and texture
3. Of course the shader itself uwu.

## TODO:

Maybe obfuscate the texture fetch.