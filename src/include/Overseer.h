#include <DFHack.h>
#include <osgGA/TrackballManipulator>
#include <osgDB/ReadFile>
#include <iostream>

#include "DwarfGeometry.h"

class Overseer
{
    public:
        Overseer();
        ~Overseer();

        bool go();
    private:
        bool connectToDF();
        void loadSettings();

        osg::Group *root;

        DFHack::ContextManager *DFMgr;
        DFHack::Context *DF;
        DFHack::Maps *Maps;
        DFHack::Materials *Mats;
        DFHack::Constructions *Cons;
        DFHack::Vegetation *Vegs;

        DwarfGeometry *dg;

        bool enableRamps;
        bool fullscreen;
        bool tristrip;
        bool doCulling;
        bool doVeggies;
        bool useShaders;
        bool useHeadlight;
        bool doImageScaling;
        int startz;
        int camz;
        int imageSize;
        double mouseSensitivity;
        double moveSpeed;
        double ceilingHeight;
        double ambient;
        double diffuse;
        double specular;
        std::string keys[7];
};
