#pragma once

typedef enum { UNTEXTURED = -1, TEX0 = 0, TEX1 = 1 } TEXTURES;

class TextureManager {
   public:
    static void InitTextures();
    static void Update();

   private:
    static void InitTexture(GLuint* tex_location, const char* file);

    static GLuint tex0, tex1;
};
