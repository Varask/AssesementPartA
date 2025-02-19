# Cube Rotation Visualization 🚀

This project is a **Qt-based OpenGL application** that displays a 3D cube with an animated texture (🔥 magma block from Minecraft) and provides several interactive features via a menu.


## Features 🎨

- **Line Rotation** ✏️  
  Rotate the cube about a user-specified line (defined by a point **b** and a direction vector **d**) with a given rotation angle.
  
- **View Position** 👁️  
  Set the camera’s position and target through a dialog box.

- **Default Position** 🔄  
  Reset the cube and camera to their default state (camera at (0,0,3) looking at the origin).

- **Animation** ⏩  
  Toggle an automatic cube rotation about the Y-axis.

- **Toggle Gloss** ✨  
  Enable or disable a gloss (specular highlight) effect on the bright areas of the texture.

- **Zoom & Manual Rotation** 🔍🖱️  
  Use the mouse wheel to zoom in/out and drag the mouse to rotate the cube manually (this disables automatic rotation).

- **Custom Background & Icon** 🎨  
  The window has a custom background color (#456990) and a custom icon (mine.png).

## Download 📥

You can download the project from the git repository:

```bash copy  
git clone https://github.com/Varask/AssesementPartA.git
```

## Build and Run 🛠️

1. **Open the Project**:  

    Open the project in Qt Creator.
   
2. **Resources**:  

    Ensure your resource file (e.g., `resources.qrc`) includes the texture (`textures/texture.png`) and icon (`textures/mine.png`).
   
3. **Build and Run**:  
   
    Build and run the project from Qt Creator.
