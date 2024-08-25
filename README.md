# Game Engine CPP
A simple Game Engine for study purposes. It uses:

- Component systems present in the popular game engines like Unreal and Unity.
- Signal/Slot system for communicate between components like Qt.
- Opengl and GLTW3 for graphics rendering
- stb for font rendering

Next steps:
- Add HIP/CUDA for fully GPU game logic. Using the algorithms of ABMGPU.
- Add support for particle system running fully in GPU.
- Add support for Lua as script language for create components, similar with Unity's MonoBehavior.
- Add parallelism in the Event Dispatcher to allow async tasks running simultaneously.
- Develop a way to create Unity's Coroutine in Lua.
