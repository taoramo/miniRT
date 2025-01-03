# Raytracer written in C

This raytracer has support for spheres, infinite planes, cylinders and cones. Lighting model is a hybrid of global illumination/physics based with diffuse and metal-like reflections and classic Blinn-Phong lighting model. Support for bump maps, checkered colors, textures and emissive objects. This raytracer is SLOW by design. Edit MAX_DEPTH and SAMPLES_PER_PIXEL in inc/miniRT.h to adjust quality and slowness levels.
<img width="1392" alt="Screenshot_2024-04-10_at_13 46 26" src="https://github.com/user-attachments/assets/6e4c8d76-a5c5-4a7f-a065-de75b5acc3b7" />

## Syntax and format for scene file (.rt)

Elements which are defined by a capital letter can only be declared once in the scene.
The first position is the type identifier (composed by one or two characters).

Floats can be in format of int.
For example, coordinates (x,y,z) can be both (1,2,3) and (1.0, 2.0, 3.0).

But integers can not be in format of floats. For example, RGB color (255,0,15) can not be (255.0, 0.0, 15.0)

Numbers like +15, +15.0, 15. are not allowed.

Parameters can be separated by isspace(3) characters.

### Ambient light

```
A <brightness> <RGB color>
```

- brightness (0.0 to 1.0)
- RGB color in "r,g,b" (0 to 255)

Example:
```
A 0 190,204,255
```

### Camera

```
C <view point> <orientation> <FOV>
```

- view point (x,y,z)
- orientation: 3d normalized orientation vector. In range [-1,1] for each x,y,z axis
- FOV : Horizontal field of view in degrees in range [0,180]

Example:
```
C 0,2,2 0,0,-1 90
```

### Light

```
l <position> <brightness> <RGB color>
```

- position: x,y,z coordinates of the light point
- brightness: the light brightness ratio in range [0.0,1.0]: 0.6
- R,G,B colors in range [0-255]: 10, 0, 255

Example:
```
l 5,1,0 1 255,255,255
```

### Sphere

```
sp <position> <diameter> <RGB color> <texture> <specular> <diffusion> <fuzz> <emission> [<bump_map>]
```

- position: x,y,z coordinates of the light point
- brightness: the light brightness ratio in range [0.0, 1.0]
- RGB color in range [0-255]
- texture: solid / checker / *.png
	- If a texture is **checker**, RGB color is added right after it. So the format becomes:
	 `sp <position> <diameter> <RGB color> checker <RGB color> <specular> <diffusion> <emission> [<bump_map>]`
- specular coefficient
- diffusion coefficient
- fuzz coefficient
- emission (x,y,z) in range from 0 to infinity
- bump map (optional): it should be a .png file 

Examples:
```
sp -1,0,-1 1.0 200,10,122 solid 0.5 0.5 0.5 0,0,0 bump_map.png
sp 0,0,-1 1.0 55,13,255 checker 122,122,204 0.3 0.7 0.3 0,0,0
sp 1,0,-1 1.0 55,13,255 earthmap.png 0.9 0.1 0.5 0,0,0 map.png
```

### Plane

```
pl <point> <normal> <RGB color> <texture> <specular> <diffusion> <fuzz> <emission> [<bump_map>]
```

- point: x,y,z coordinates of a point in the plane
- normal: 3d normalized normal vector. In range [-1,1] for each x,y,z axis
- RGB color in range [0-255]
- texture: solid / checker / *.png
	- If a texture is **checker**, RGB color is added right after it. So the format becomes:
	 `pl <point> <normal> <RGB color> <texture> checker <RGB color> <specular> <diffusion> <emission> [<bump_map>]`
- specular coefficient
- diffusion coefficient
- fuzz coefficient
- emission (x,y,z) in range from 0 to infinity
- bump map (optional): it should be a .png file

Example:
```
pl 0,-1,0 0,1,0 122,122,204 solid 0.5 0.5 0.3 0,0,0
```

### Cylinder

```
cy <position> <orientation> <diameter> <height> <RGB color> <texture> <specular> <diffusion> <fuzz> <emission> [<bump_map>]
```

- position: x,y,z coordinates of the center of the cylinder
- orientation: 3d normalized vector of axis of cylinder. In range [-1,1] for each x,y,z axis
- diameter: the cylinder diameter
- height: the cylinder height
- RGB color in range [0-255]
- texture: solid / checker / *.png
	- If a texture is **checker**, RGB color is added right after it. So the format becomes:
	 `cy <position> <orientation> <diameter> <height> <RGB color> <texture> <specular> <diffusion> <emission> [<bump_map>]`
- specular coefficient
- diffusion coefficient
- fuzz coefficient
- emission (x,y,z) in range from 0 to infinity
- bump map: (optional): it should be a .png file 

Examples:
```
cy 50.0,0.0,20.6 0,0,1.0 14.2 21.42 10,0,255 solid 0.1 0.6 0,0,0 bumps.png
```

### Cone

```
co <position> <orientation> <diameter> <height> <RGB color> <texture> <specular> <diffusion> <fuzz> <emission> [<bump_map>]
```

- position: x,y,z coordinates of the center of the cylinder
- orientation: 3d normalized vector of axis of cylinder. In range [-1,1] for each x,y,z axis
- diameter: the cylinder diameter
- height: the cylinder height
- RGB color in range [0-255]
- texture: solid / checker / *.png
	- If a texture is **checker**, RGB color is added right after it. So the format becomes:
	 `cy <position> <orientation> <diameter> <height> <RGB color> <texture> <specular> <diffusion> <emission> [<bump_map>]`
- specular coefficient
- diffusion coefficient
- fuzz coefficient
- emission (x,y,z) in range from 0 to infinity
- bump map: (optional): it should be a .png file 

Examples:
```
cy 50.0,0.0,20.6 0,0,1.0 14.2 21.42 10,0,255 solid 0.1 0.6 0,0,0 bumps.png
```
<img width="1392" alt="Screenshot_2024-04-16_at_21 39 09-200-samples-per-pixel" src="https://github.com/user-attachments/assets/0449096f-06cc-4c40-999e-c1b926b84c80" />
<img width="1392" alt="1-scene-3-spheres-3-lights" src="https://github.com/user-attachments/assets/34908f77-df87-4054-903a-77d39c8bbb8e" />
