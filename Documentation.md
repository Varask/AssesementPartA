# Project Documentation: Cube Rotation Visualization (Part A)

## Overview

This project is a Qt-based OpenGL application that renders a 3D cube with an animated texture. It includes multiple interactive features through a menu-driven interface. The application implements the following functionalities as specified:

1. **Line Rotation**:  
   - **Functionality**: Allows the user to input a line defined by a point **b** and a direction vector **d**, along with a rotation angle α.  
   - **Implementation**:  
     - A dialog (LineRotationDialog) collects 3 float values for **b** and **d** and an angle.
     - The rotation is computed as:  
       `M = T(+b) * R(angle, normalized(d)) * T(-b) * M_current`  
       where T() represents a translation matrix.
     - This transformation is applied to the cube model.

2. **View Position**:  
   - **Functionality**: Allows the user to specify the camera (eye) position and the point to look at.  
   - **Implementation**:  
     - A dialog (ViewPositionDialog) collects the eye coordinates and the target point.
     - The view is updated using a `lookAt`-like function (implemented via QMatrix4x4) similar to `glm::lookAt`.

3. **Default Position**:  
   - **Functionality**: Resets the view to the default setting (camera at (0,0,3) looking toward the origin) – the initial view when the program is run.
   - **Implementation**:  
     - The `resetDefault()` method sets the view matrix accordingly and resets the cube’s transformation.

4. **Animation**:  
   - **Functionality**: Toggles an automatic spin animation of the cube about the Y-axis.
   - **Implementation**:  
     - A QTimer triggers incremental rotations around the Y-axis when enabled.

5. **Texture Animation**:  
   - **Functionality**: The cube's texture (a magma block texture) contains 3 animation phases. The texture automatically changes every 700ms.
   - **Implementation**:  
     - The texture is split into three 16×16 sub-images from a 16×48 PNG.
     - A timer (textureTimer) cycles through the textures.

6. **Gloss Effect with Toggle**:  
   - **Functionality**: Applies a gloss (specular highlight) effect to the bright areas of the texture. The effect can be toggled on or off.
   - **Implementation**:  
     - In the fragment shader, a specular component is added if `uGlossOn` is true.
     - The gloss intensity is computed using a `smoothstep` function between two brightness thresholds (corresponding to colors #CA4E06 and #F89E44).
     - The gloss multiplier has been amplified for increased visibility.
     - A menu item (“Toggle Gloss”) triggers the slot to invert the `glossEnabled` state.

7. **Zoom and Manual Rotation**:  
   - **Functionality**:  
     - **Zoom**: The user can zoom in/out using the mouse wheel, which adjusts the camera distance.
     - **Manual Rotation**: The user can click and drag the mouse to manually rotate the cube. This disables the automatic animation.
   - **Implementation**:  
     - The `wheelEvent()` is reimplemented to update the view matrix based on the camera distance.
     - Mouse press and move events compute rotation deltas to modify the model matrix.

8. **Window Icon and Background**:  
   - **Functionality**: The application window has an icon (mine.png) and a background color (#456990).
   - **Implementation**:  
     - The MainWindow sets its window icon using `setWindowIcon(QIcon(":/textures/textures/mine.png"));`
     - In `initializeGL()`, the clear color is set with `glClearColor(0.27f, 0.41f, 0.56f, 1.0f);`.

## Architecture and Implementation Details

- **Project Structure**:  
  - The project uses Qt Widgets and OpenGL via QOpenGLWidget.
  - All textures and icons are managed with the Qt resource system (using a .qrc file).

- **Rendering Pipeline**:  
  - The cube is rendered with vertex data that includes positions, normals, and texture coordinates.
  - Custom shaders implement Phong lighting with ambient, diffuse, and specular components.
  - The gloss effect is integrated into the specular calculation and is controlled by a uniform variable.

- **Menus and User Interaction**:  
  - The MainWindow contains a menu with items for “Line Rotation”, “View Position”, “Default Position”, “Animation”, and “Toggle Gloss”.
  - Dialogs are used to gather input data from the user.

## Diagrams

*You can add your diagrams below using Mermaid or PlantUML.*

**Example Mermaid Diagram:**

```mermaid
graph TD;
    A[MainWindow] --> B[CubeWidget];
    B --> C[OpenGL Rendering];
    A --> D[LineRotationDialog];
    A --> E[ViewPositionDialog];
    B --> F[Animation Timer];
    B --> G[Texture Timer];
