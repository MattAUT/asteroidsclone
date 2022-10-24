// COMP710 GP Framework 2022
#ifndef __TEXTUREMANAGER_H__
#define __TEXTUREMANAGER_H__

// Library Includes:
#include <string>
#include <map>

// Forward Declarations:
class Texture;

class TextureManager
{
    // Member methods:
public:
    TextureManager();
    ~TextureManager();

    bool Initialise();

	void AddTexture(const char* key, Texture* pTexture);
	void RemoveTexture(const char* key);
    Texture* GetTexture(const char* pcFilename);

#ifdef _DEBUG
	void DebugDraw();
#endif // _DEBUG
protected:

private:
    TextureManager(const TextureManager& textureManager);
	TextureManager& operator=(const TextureManager& textureManager) {}

    // Member data:
public:

protected:
    std::map<std::string, Texture*> m_pLoadedTextures;

private:

};

#endif // __TEXTUREMANAGER_H__
