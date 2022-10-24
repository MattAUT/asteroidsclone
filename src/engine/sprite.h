// COMP710 GP Framework 2022
#ifndef __SPRITE_H__
#define __SPRITE_H__

#include "renderer.h" // We need this for enum definitions 

class Texture;
class Vector2;
class VertexArray;

#include <vector>

class Sprite
{
    // Member methods:
public:
    Sprite();
    ~Sprite();

    bool Initialise(Texture& texture, VertexArray& vertexData);
    void Process(float deltaTime);
    void Draw(Renderer& renderer, Vector2& position);

    int GetWidth() const;
    int GetHeight() const;

    void SetAngle(float angle);
    float GetAngle() const;

    void SetScale(float scale);
    float GetScale() const;

    void SetAlpha(float alpha);
    float GetAlpha() const;

    void SetRedTint(float value);
    float GetRedTint() const;
    void SetGreenTint(float value);
    float GetGreenTint() const;
    void SetBlueTint(float value);
    float GetBlueTint() const;

	static bool GenerateVertices
	(
		std::vector<float>* vertices,
		Vector2 topLeftUV, 
		Vector2 topRightUV, 
		Vector2 bottomRightUV, 
		Vector2 bottomLeftUV, 
		SpriteOrigin origin
	);

protected:
private:
    Sprite(const Sprite& sprite);
    Sprite& operator=(const Sprite& sprite) {}

    // Member data:
public:

protected:
    Texture* m_pTexture;
	VertexArray* m_pVertexData;

    float m_angle;

    int m_width;
    int m_height;

    float m_scale;
    float m_alpha;

    float m_tintRed;
    float m_tintGreen;
    float m_tintBlue;

private:

};

#endif // __SPRITE_H__

