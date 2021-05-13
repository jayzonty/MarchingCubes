#include "Engine/Application.hpp"

#include "MarchingCubes3DScene.hpp"

int main()
{
    {
        Application application;

        MarchingCubes::MarchingCubes3DScene* scene = new MarchingCubes::MarchingCubes3DScene();
        application.SetStartingScene(scene);
        application.Run();

        delete scene;
    }

    return 0;
}
