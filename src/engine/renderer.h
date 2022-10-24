// COMP710 GP Framework 2022
#ifndef __RENDERER_H__
#define __RENDERER_H__

// Forward Declarations:
class TextureManager;
class Shader;
class VertexArray;
class Sprite;
class SpriteSheetAnimation;
struct SDL_Window;

#include <vector2.h>
#include <SDL.h>
#include <SDL_ttf.h>

enum SpriteOrigin
{
	BOTTOMLEFT,
	BOTTOMRIGHT,
	TOPLEFT,
	TOPRIGHT,
	BOTTOMCENTER,
	TOPCENTER,
	CENTER
};

enum FontSize
{
	SMALL,
	MEDIUM,
	LARGE
};

class Renderer
{
    // Member methods:
public:
    Renderer();
    ~Renderer();

    bool Initialise(bool windowed, int width = 0, int height = 0);

    void Clear();
    void Present();

    void SetClearColour(unsigned char r, unsigned char g, unsigned char b);
    void GetClearColour(unsigned char& r, unsigned char& g, unsigned char& b);

    int GetWidth() const;
    int GetHeight() const;

    Sprite* CreateSprite(const char* pcFilename, SpriteOrigin origin);
	Sprite* CreateStaticTextSprite(const char* text, FontSize fSize, SDL_Color color, SpriteOrigin origin);
	SpriteSheetAnimation* CreateAnimatedSprite(const char* pcFilename);
    void DrawSprite(Sprite& sprite, Vector2& position);
	void DrawAnimatedSprite(SpriteSheetAnimation& sprite, Vector2& position, int frame);
	void DrawDynamicText(const char* text, FontSize fSize, Vector2& position, SDL_Color color, SpriteOrigin origin);
#ifdef _DEBUG
	void DebugDraw();
#endif // _DEBUG

protected:
    bool InitialiseOpenGL(int screenWidth, int screenHeight);
    void SetFullscreen(bool fullscreen); 
    void LogSdlError();
    bool SetupSpriteShader();
	TTF_Font* GetFontFromEnum(FontSize fSize);

private:
    Renderer(const Renderer& renderer);
	Renderer& operator=(const Renderer& renderer) {}

    // Member data:
public:

protected: 
    TextureManager* m_pTextureManager;
    SDL_Window* m_pWindow;
    SDL_GLContext m_glContext;
	TTF_Font* m_pFontLarge;
	TTF_Font* m_pFontMedium;
	TTF_Font* m_pFontSmall;

    Shader* m_pSpriteShader;
    VertexArray* m_pSpriteVertexData;

    int m_iWidth;
    int m_iHeight;

    float m_fClearRed;
    float m_fClearGreen;
    float m_fClearBlue;

private:

};

#endif // __RENDERER_H__
