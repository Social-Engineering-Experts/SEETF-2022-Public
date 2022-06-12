fonttexture = open("rsrc/font-min.jpg", "rb").read()
c_str = "unsigned char bufferFontTexture[] = {%s};"%",".join(map(str, [*fonttexture]))
open("src/_fonttexture_gen.h", "w").write(c_str)