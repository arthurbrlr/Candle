# Candle
## Started in March 2020 - Ongoing

This is the "sequel" to Dried. With the lockdown in France and the Covid19 outbreak, I had quite a lot of spare time, so I decided to come back to a project I abandonned in September 2019.
After messing around with the existing code base of Dried, I realised it would be near impossible to add new major systems, and I restarted from scratch, creating Candle.

There's no real goal behind that project other than learning how a modern game engine should work, ultimately, I'd like to create some small games using Candle, but there are severall features that are missing to even think about it.

## Using Candle
***Warning: Build only on Windows!***

#### Testing:
You can download the `/Demo` folder. It contains the executable for CandleTester along with some placeholder shaders and "art" I made. For now scenes are hardcoded so there's not that much you can do with it, but I plan on adding scene serialisation in files soon.

#### Building:

In Git Bash, navigate to the folder of your choice and enter the following command:<br>
`git clone --recurse-submodules git://github.com/arthurbrlr/Candle`

Then, once the project is fully downloaded, open Visual Studio (I'm using 2019 so I recommend you to do so) and look for the solution file *Candle.sln*. Open it, select *CandleTester* and set it to be the startup project (right click > *Set as startup project*). You can now build and (hopefully) run Candle with some predefined scenes.

## Project Structure:
- CandleEngine: Engine source code,
- CandleTester: Client application for testing purposes,
- CandleTemplate: Template client application, can be used to start creating a game from scratch.

### Using:
- C++ as the main programming language for the engine and the client,
- OpenGL as the main rendering API, I'm planning on adding Vulkan once I will have the time to do so,
- OpenAL as the sound API, not implemented in the Component system I have yet,
- Project building for Windows only, using Visual Studio 2019,
- In theory, the project can be build and run on Linux and MacOS (I'm not using any Windows specific API there) but I'm not yet providing a solution for that.

### Features of the engine:
- A 2D and 3D renderer (but 3D meshes can't be rendered through the ECS yet),
- Basic postprocessing (brightness filters, gaussian blur and bloom effect using shaders),
- A very basic ECS and scene system,
- An working editor,
- Gameplay scripting using C++,
- Asset loading in the project folder (textures and shaders).

## What comes next [(trello board)](https://trello.com/b/zaCBNTEl/candleengine):
- Refactoring the Entity class,
- Scene serialisation,
- 2D Physics,
- Text Rendering,
- Audio listeners and streaming from disk,
- So much more, help me, I haven't seen the sun since last January.

## Acknowledgements:
- [GLFW](https://www.glfw.org)
- [GLEW](http://glew.sourceforge.net)
- [OpenAL](https://www.openal.org)
- [ImGui](https://github.com/ocornut/imgui)
- [SpdLog](https://github.com/gabime/spdlog)
- [GLM](https://github.com/g-truc/glm)
