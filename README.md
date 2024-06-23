# Triangle Filling
Triangle Filling is an application that allows rendering an animation of a light source moving over a Bezier surface, with options to change the background and textures. The program is built on Qt 6.6.0, using the C++20 standard.

# Keybindings
This section contains information on how to use the application.

## Display Modes
The application has two display modes depending on the `show` checkbox.

1. Control Points Display and Edit Mode - entering this mode stops the animation. The user can select control points by clicking (the active point will light up red), and then change their height using the slider to the right of the checkbox.
2. Animation Mode - in this mode, you can control the animation (using the `play` and `pause` buttons). Additionally, with the `show grid` checkbox, you can enable the display of the triangle grid.

## Animation Settings
The following settings will stop the animation when modified:

1. Changing the number of triangles in the grid - controlled by the `triangulation` slider.
2. Changing the background image - done using the `image` button.
3. Changing the background texture (normal map) - done using the `map` button.

Other settings can be changed during the animation:
- light color - `light color` button
- background color - `background` button
- diffuse reflection coefficient - `kd` slider
- specular reflection coefficient - `ks` slider
- shininess coefficient - `m` slider