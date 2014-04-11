#ifndef RENDERDATA_H_INCLUDED
#define RENDERDATA_H_INCLUDED

#include <SDL.h>

class cSceneObject;

//------------------------------------------------------------

//Render Resource Definition
//Derived from cResource. Represents a graphics resource.
//Pointers to instances of this class are maintained by the
//Resource Manager.
class cRenderResource : public cResource
{
    private:
    protected:
    public:
        SDL_Surface *m_Surface;
		SDL_Texture* m_Texture;
		SDL_Renderer *m_Renderer;
		cSceneObject *parentScene;

        ~cRenderResource();
        void load();
        void unload();
        cRenderResource();
};

//------------------------------------------------------------

//2D Render Object
//Each instance of this class represents a unqiue renderable item in a 2D scene.
//It a reference to a graphics resource, and position, scale and rotation values.
//This combination of values allows the render manager to determine
//Which graphics resource should be rendered, and where and how.
//There may be many Render Objects referencing only one graphics resource.
//EG. Where a scene contains many identical enemy characters.
class cSDLRenderObject : public cEngineObject
{
    private:
    protected:
    public:

    cRenderResource *m_RenderResource;
	cSceneObject* GetCurrentScene() { return m_RenderResource->parentScene; }
    SDL_Rect m_RenderRect;
    float m_PosX;
    float m_PosY;
    bool m_bVisible;
    SDL_Color m_ColorKey;
    bool m_bColorKeyEnabled;

    cSDLRenderObject();
    void setResourceObject(cRenderResource *RenderResource);
    void setColorKey(unsigned int r, unsigned int g, unsigned int b);
    virtual void update(){};
};

//------------------------------------------------------------

#endif // RENDERDATA_H_INCLUDED
