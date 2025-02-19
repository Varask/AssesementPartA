# Cube Rotation Visualization (Part A)

This project is a Qt-based OpenGL application that displays a 3D cube with an animated texture (magma) and provides several interactive features via a menu. The application fulfills the following requirements:

- **Line Rotation**: Opens a dialog to input a line (point **b** and direction vector **d**) and a rotation angle. The cube rotates about the specified line.
- **View Position**: Opens a dialog to set the camera's eye position and the point it is looking at. The cube is re-rendered from the new viewpoint.
- **Default Position**: Resets the cube and camera to their default settings (camera at (0,0,3) looking toward the origin).
- **Animation**: Toggles an automatic spin animation of the cube.
- **Toggle Gloss**: Allows you to turn on/off a gloss effect on the bright areas of the texture.
- **Zoom**: Use the mouse wheel to zoom in and out.
- **Manual Rotation**: Drag the mouse to manually rotate the cube (which disables the automatic animation).

## Build and Run

1. **Using Qt Creator:**
   - Open the project file in Qt Creator.
   - Depending the vesion of the project you might need GML replace the `INCLUDEPATH` in the .pro by your path.
   - Make sure that your resource file (`.qrc`) includes the textures (e.g., `textures/texture.png` and `textures/mine.png`).
   - Build and run the project.

2. **Command Line:**
   - Configure and build using `qmake` and `make` (or the equivalent for your platform).

## Resources

- Icon and texture files are managed via the Qt resource system.
- The project uses QOpenGLWidget for rendering.
- All menus and dialogs are built using Qt Widgets.

