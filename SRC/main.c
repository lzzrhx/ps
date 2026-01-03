#include "globals.h"

#include "inline_n.h"
#include "display.h"
#include "joypad.h"
#include "camera.h"
#include "object.h"
#include "utils.h"
#include "track.h"
#include "ship.h"


Camera camera;
Object* ships;
Ship ship;
Track track;
//Object* sceneobjs;


///////////////////////////////////////////////////////////////////////////////
// Initialization
///////////////////////////////////////////////////////////////////////////////
void Setup(void) {
    u_short shipsstarttexture, scenestarttexture, trackstarttexture;
    VECTOR startpos;
    ScreenInit();
    CdInit();
    JoyPadInit();
    ResetNextPrim(GetCurrBuff());
    shipsstarttexture = GetTextureCount();
    LoadTextureCMP("\\ALLSH.CMP;1", NULL);
    //scenestarttexture = GetTextureCount();
    //LoadTextureCMP("\\TRACK02\\SCENE.CMP;1", NULL);
    ships = LoadObjectPRM("\\ALLSH.PRM;1", shipsstarttexture);
    //sceneobjs = LoadObjectPRM("\\TRACK02\\SCENE.PRM;1", scenestarttexture);
    trackstarttexture = GetTextureCount();
    LoadTextureCMP("\\TRACK02\\LIBRARY.CMP;1", "\\TRACK02\\LIBRARY.TTF;1");
    LoadTrackVertices(&track, "\\TRACK02\\TRACK.TRV;1");
    LoadTrackFaces(&track, "\\TRACK02\\TRACK.TRF;1", trackstarttexture);
    LoadTrackSections(&track, "\\TRACK02\\TRACK.TRS;1");
    ship.object = GetObjectByIndex(ships, 0);
    setVector(&startpos, 32599, -347, -45310);
    ShipInit(&ship, &track, &startpos);
    setVector(&camera.position, ship.object->position.vx, ship.object->position.vy - 350, ship.object->position.vz - 1200);
    camera.rotmat = (MATRIX){0};
    camera.lookat = (MATRIX){0};
}


///////////////////////////////////////////////////////////////////////////////
// Update function
///////////////////////////////////////////////////////////////////////////////
void Update(void) {
    u_short i;
    VECTOR up = (VECTOR){0, -ONE, 0};
    EmptyOT(GetCurrBuff());
    JoyPadUpdate();
    if (JoyPadCheck(PAD1_LEFT)) {
        if (ship.velyaw <= 0) {
            ship.velyaw -= 128;
        } else {
            ship.velyaw -= 256;
        }
    }
    else if (JoyPadCheck(PAD1_RIGHT)) {
        if (ship.velyaw >= 0) {
            ship.velyaw += 128;
        } else {
            ship.velyaw += 256;
        }
    } else {
        if (ship.velyaw > 0) {
            ship.velyaw -= 128;
        } else if (ship.velyaw < 0) {
            ship.velyaw += 128;
        }
    }
    if (ship.velyaw > 2048) ship.velyaw = 2048;
    if (ship.velyaw < -2048) ship.velyaw = -2048;
    if (JoyPadCheck(PAD1_UP)) {
        ship.pitch += 6;
    } else if (JoyPadCheck(PAD1_DOWN)) {
        ship.pitch -= 6;
    }
    if (JoyPadCheck(PAD1_CROSS)) {
        ship.thrustmag += 3000;
    } else if (ship.thrustmag > 0) {
        ship.thrustmag -= 15000;
    }
    if (ship.thrustmag < 0) {
        ship.thrustmag = 0;
    } else if (ship.thrustmag > ship.thrustmax) {
        ship.thrustmag = ship.thrustmax;
    }
    ShipUpdate(&ship);
    camera.position.vx = ship.object->position.vx - (ship.forward.vx >> 2) + (up.vx >> 3);
    camera.position.vy = ship.object->position.vy - (ship.forward.vy >> 2) + (up.vy >> 3);
    camera.position.vz = ship.object->position.vz - (ship.forward.vz >> 2) + (up.vz >> 3);
    LookAt(&camera, &camera.position, &ship.object->position, &(VECTOR){0, -ONE, 0});
}


///////////////////////////////////////////////////////////////////////////////
// Render function that invokes the draw of the current frame
///////////////////////////////////////////////////////////////////////////////
void Render(void) {
    //RenderSceneObjects(sceneobjs, &camera);
    RenderTrack(&track, &camera);
    RenderObject(ship.object, &camera);
    DrawXYZAxis(&ship, &camera);
    DisplayFrame();
}


///////////////////////////////////////////////////////////////////////////////
// Main function
///////////////////////////////////////////////////////////////////////////////
int main(void) {
    Setup();
    while (1) {
        Update();
        Render();
    }
    return 0;
}
