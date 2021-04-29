#include "Engine/Application.hpp"

#include "MarchingCubes2DScene.hpp"

int main()
{
    {
        Application application;

        MarchingCubes::MarchingCubes2DScene* scene = new MarchingCubes::MarchingCubes2DScene();
        application.SetStartingScene(scene);
        application.Run();

        delete scene;
    }

    return 0;
}
