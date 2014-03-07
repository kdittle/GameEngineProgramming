#ifndef RENDERMANAGER2D_H_INCLUDED
#define RENDERMANAGER2D_H_INCLUDED

#include "EngineObject.h"

//ABSTRACT BASE CLASS FOR 2D RENDER MANAGER
//2D RENDER MANAGERS SHOULD BE DERVIED FROM THIS CLASS

class c2DRenderManager : public cEngineObject
{
    private:
    protected:
    public:
        virtual ~c2DRenderManager(){}
        virtual bool init(unsigned int Width=800, unsigned int Height=600, bool fullScreen=false, char* WindowTitle=0) = 0;
        virtual void free() = 0;
        virtual bool update() = 0;
};


#endif // 2DRENDERMANAGER_H_INCLUDED
