#include "Engine/Application.hpp"

#include "MainScene.hpp"

int main()
{
    {
        Application application;

        MarchingCubes::MainScene* scene = new MarchingCubes::MainScene();
        application.SetStartingScene(scene);
        application.Run();

        delete scene;
    }

    return 0;
}
