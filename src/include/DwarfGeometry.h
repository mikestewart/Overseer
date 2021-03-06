#include <DFHack.h>
#include <dfhack/DFTileTypes.h>
#include <osg/Drawable>
#include <osg/Geometry>
#include <osgGA/StateSetManipulator>
#include <osgUtil/TriStripVisitor>
#include <osgUtil/Optimizer>
#include <osgDB/ReadFile>
#include <osgDB/FileUtils>
#include <osg/Material>
#include <osg/Texture2D>
#include <osg/CullFace>
#include <cctype>
#include <vector>
#include <map>

#define MAT_GRASS (1<<31)
#define MAT_GRASS2 (2<<30)
#define MAT_GRASS_DEAD (3<<30)
#define MAT_GRASS_DRY (4<<29)
#define MAT_MAGMA (5<<29)
#define MAT_ICE (6<<29)
#define MAT_OBSIDIAN (7<<29)
#define CONSTRUCTED (1<<28)

enum RampType
{
    NONE,
    NORTH,
    SOUTH,
    EAST,
    WEST,
    NW_UP,
    NW_DOWN,
    NE_UP,
    NE_DOWN,
    SW_UP,
    SW_DOWN,
    SE_UP,
    SE_DOWN,
    HILL,
    UNKNOWN
};

typedef struct
{
    int16_t tiletype;
    DFHack::t_designation designation;
    DFHack::t_occupancy occupancy;
    struct
    {
        uint16_t type;
        uint32_t index;
    } material;
    RampType ramptype;
    bool ceiling;
} Tile;

class DwarfGeometry
{
    public:
        DwarfGeometry();
        DwarfGeometry(DFHack::Maps *m, DFHack::Materials *mt, DFHack::Constructions *cns, DFHack::Vegetation *vgs, osg::Group *g, int sz, double ch, bool ts, bool dc, int is, bool us, bool imgs);
        bool drawGeometryOld();
        bool drawGeometry();
        bool drawVegetation();
        void drawSkybox();
        bool start();
        int getGeometryMax();
        void clean();
    private:
        bool drawNorthWalls(uint32_t z);
        bool drawSouthWalls(uint32_t z);
        bool drawWestWalls(uint32_t z);
        bool drawEastWalls(uint32_t z);
        bool drawFloors(uint32_t z);
        bool drawCeilings(uint32_t z);
        bool drawRamps(uint32_t z);
        void drawFluids();
        void drawNorthRampWestBoundaries(uint32_t x, uint32_t y, uint32_t z, uint32_t wallmat, uint16_t walltype);
        void drawNorthRampEastBoundaries(uint32_t x, uint32_t y, uint32_t z, uint32_t wallmat, uint16_t walltype);
        void drawNorthRampSouthBoundaries(uint32_t x, uint32_t y, uint32_t z, uint32_t wallmat, uint16_t walltype);

        void drawSouthRampWestBoundaries(uint32_t x, uint32_t y, uint32_t z, uint32_t wallmat, uint16_t walltype);
        void drawSouthRampEastBoundaries(uint32_t x, uint32_t y, uint32_t z, uint32_t wallmat, uint16_t walltype);
        void drawSouthRampNorthBoundaries(uint32_t x, uint32_t y, uint32_t z, uint32_t wallmat, uint16_t walltype);

        void drawWestRampNorthBoundaries(uint32_t x, uint32_t y, uint32_t z, uint32_t wallmat, uint16_t walltype);
        void drawWestRampSouthBoundaries(uint32_t x, uint32_t y, uint32_t z, uint32_t wallmat, uint16_t walltype);
        void drawWestRampEastBoundaries(uint32_t x, uint32_t y, uint32_t z, uint32_t wallmat, uint16_t walltype);

        void drawEastRampNorthBoundaries(uint32_t x, uint32_t y, uint32_t z, uint32_t wallmat, uint16_t walltype);
        void drawEastRampSouthBoundaries(uint32_t x, uint32_t y, uint32_t z, uint32_t wallmat, uint16_t walltype);
        void drawEastRampWestBoundaries(uint32_t x, uint32_t y, uint32_t z, uint32_t wallmat, uint16_t walltype);

        void drawCeilingBorders(uint32_t z);

        void drawStairs();
        void drawFortifications();

        void processRamps();
        inline bool isCeiling(int16_t t, int16_t up)
        {
            return !DFHack::isWallTerrain(t) && (DFHack::isRampTerrain(up) || DFHack::isFloorTerrain(up) || DFHack::isWallTerrain(up));
        }
        inline bool isRampTopOrOpenTerrain(int in)
        {
            return DFHack::tileTypeTable[in].c==DFHack::RAMP_TOP || DFHack::tileTypeTable[in].c==DFHack::EMPTY;
        }

        bool loadColors();
        void getMaterial(osg::Geometry *g, uint32_t index);

        bool tristrip;

        DFHack::Maps *Map;
        DFHack::Materials *Mats;
        DFHack::Constructions *Cons;
        DFHack::Vegetation *Vegs;
        osg::ref_ptr<osg::Group> geometryGroup;
        int startz;
        int imageSize;
        osg::ref_ptr<osg::Geode> blockGeode;
        std::map<uint32_t, osg::ref_ptr<osg::Geometry> > *bg;
        std::map<uint32_t, osg::ref_ptr<osg::Vec3Array> > *vertices;
        std::map<uint32_t, osg::ref_ptr<osg::Vec3Array> > *normals;
        std::map<uint32_t, osg::ref_ptr<osg::Vec2Array> > *texcoords;
        std::map<uint32_t, osg::ref_ptr<osg::Image> > textures;
        std::map<uint32_t, osg::ref_ptr<osg::Image> > nmaps;
        std::map<uint32_t, osg::ref_ptr<osg::Program> > programs;
        std::map<uint32_t, osg::ref_ptr<osg::Geode> > stairs;
        std::map<uint32_t, osg::ref_ptr<osg::Geode> > forts;
        std::map<std::string, osg::Vec4*> colors;
        osg::ref_ptr<osg::DrawElementsUInt> face;
        std::vector<std::vector<std::vector<Tile> > > tiles;
        osg::ref_ptr<osg::Texture2D> cracktex, constex;
        uint32_t xmax,ymax,zmax;
        int geomax;
        double ceilingHeight;
        bool doCulling;
        bool doImageScaling;
        bool useShaders;
        osg::ref_ptr<osg::GraphicsContext> gc;
};
