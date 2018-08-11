Computer Graphics - Homework Assignment 2 - Raycasting
======================================================

Goals:
------

* Understand how to create a virtual picture of a 3D scene.

* Gain experience deriving and implementing mathematical expressions for
geometric calculations.

* Become familiar with a 3D math library (bonus: identical to one
available when programming for a GPU).

* Become more comfortable with C++.

Getting Started & Handing In:
-----------------------------

* This is a programming assignment. The code framework is provided here.

* The code will be written in C++. This time, the project is more complex
and you will see and need to use more of the language, including the
`std::vector` container class and object-oriented programming. I have made
use of the modern C++11 standard where appropriate. This makes the
language easier to write and safer, though some reference material,
tutorials, and examples you find will be outdated.

* The program is a command line program. It makes use of the open source
Qt framework only to save the resulting raster image to disk.

* You should have already successfully installed the open source version
of the Qt environment from the last assignment:
<https://www.qt.io/download-open-source>
(At the time of writing, version 5.11 is the newest version. Any 5.x
version should work. The installer, by default, includes all versions of
Qt. Save space by installing only the most recent version and a
compiler.) Mac users with [Homebrew](https://brew.sh/)
can alternatively install via: `brew install qt` and `brew cask install qt-creator`.

* Download the assignment. This will create a folder named `raycasting`. Open
the file named `raycasting.pro`. This should launch the Qt Creator
development environment (IDE).

* Build and run the code. The code should compile, but it will complain
when running about not having enough arguments. You should see a message
like:

        Usage: raycasting path/to/scene.json image_out.png long_edge_pixels

* To set the command line arguments that Qt Creator uses, click on
"Projects" and then, under "Build & Run", click on "Run." Set the
"Command line arguments" appropriately. For example:

        spheres_cylinder.json spheres_cylinder-test.png 500

    ![Qt Creator > Projects > Run > Command Line Arguments](docs/readme_images/qt-creator.png)

    For those arguments to work, you would either need to set "Working
directory" to the `examples` subdirectory, or else copy
`spheres_cylinder.json` next to the "Executable" (or wherever you point
"Working directory").

* Add your code to `camera.cpp`, `scene.cpp`, and `shape.cpp`.

* Build and run and test that it is working correctly. Qt Creator has a
great debugger.

* Create 600 pixel images for each of the following `.json` scenes. Copy the
`.json` and your `.png` files into a new `test/` subdirectory.

    * `spheres_cylinder.json`
    * `cone_cube.json`
    * `stress_test.json`
    * `camera_test1.json`
    * `camera_test2.json`
    * `orthographic_test1.json`
    * `orthographic_test2.json`
    * A `.json` scene that you make yourself

* When done, zip your entire `raycasting` directory, including the `test/`
subdirectory containing the scenes and your program's output on them
with the `long_edge_pixels` command line parameter set to 600, and a
*Notes.txt* file. Name the zip file *hw02_lastname_firstname.zip.*
Upload your solution to Blackboard before the deadline. Your
*Notes.txt* should describe any known issues or extra features. Your
*Notes.txt* should also note the names of people in the class who
deserve a star for helping you (not by giving your their code!).

* The framework and glm vector math library provide all the support code
that you need.

* **THIS IS AN INDIVIDUAL, NOT A GROUP ASSIGNMENT. That means all code
written for this assignment should be original! Although you are
permitted to consult with each other while working on this assignment,
code that is substantially the same will be considered cheating.** In your
`Notes.txt`, please note who deserves a star (who helped you with the
assignment).

Overview:
---------

In this assignment, you will be implementing a raycaster, which is the
first step to creating a raytracer. You will be able to create stunning
artwork like this:

![solid color spheres and a cylinder](docs/readme_images/spheres_cylinder.png)

The assignment is broken down into three parts: (1) creating 3D rays
through pixels, (2) intersecting 3D rays with various 3D shapes, and (3)
computing a color for the ray.

(You will add illumination in the follow-up assignment,
[raytracing](https://github.com/yig/graphics101-raytracing).)

Rubric
------

* **(10 points)** 3D rays through pixels. The `Camera` methods:

    * **(5 points)** `CameraPerspective::getRay( u,v )`

        * Return a world-space ray through the pixel located at (*u,v*) on the
film plane. A point (*u,v*) on the film plane is located at
**e** + *u* **u** + *v* **v** - *d* **w** in world-space. (That formula written in TeX markup is: $\mathbf{e} + u\mathbf{u} + v\mathbf{v} - d\mathbf{w}$.)
Your ray can emanate from the eye **e** itself or from the film
plane point. The world-space direction of the rays is the direction from
the eye **e** through the aforementioned film plane point:
*u* **u** + *v* **v** - *d* **w**.
(In TeX: $u\mathbf{u} + v\mathbf{v} - d\mathbf{w}$.)

    * **(5 points)** `CameraOrthographic::getRay( u,v )`

        * Return a world-space ray through the pixel located at (*u,v*) on the
film plane. A point (*u,v*) on the film plane is located at
**e** + *u* **u** + *v* **v** in world-space. (In TeX: $\mathbf{e} + u\mathbf{u} + v\mathbf{v}$.)
 The world-space
rays of an orthographic camera are all parallel, in the direction
-**w**. (In TeX: $- \mathbf{w}$.)

* **(10 points)** The rendering loop. `Scene` methods:

    * **(4 points)** `render()`

        * Converts a pixel's x,y coordinates to u,v coordinates via
`camera->getPixelUV()` and then to a world-space ray via
`camera->getRay()`. Then, get the light along the ray by calling
`rayColor()`.

    * **(1 point)** `rayColor()`

        * Returns the light along the given ray as a color. In this raycasting
assignment, `rayColor()` will call `closestIntersection()` and return the
color of the closest intersected object.

    * **(5 points)** `closestIntersection()`

        * Calls `rayIntersect()` on every shape and returns the closest one
(smallest *t*).

* **(75 points)** Intersections. The `Shape` subclasses' `rayIntersect()`
methods:

    * **(15 points)** Sphere (centered at the origin with radius 1):

        * F(x,y,z) = x² + y² + z² - 1

            $$F(x,y,z) = x^2 + y^2 + z^2 - 1$$

    * **(15 points)** Plane (the *xy* plane, also known as the
*z* = 0 plane)

        * F(x,y,z) = z

            $$F(x,y,z) = z$$

    * **(15 points)** Cylinder (bottom at the origin, top at (0,0,1), radius 1)
with a top and bottom cap (circles with radius 1 at z=0 and z=1).
You handle this as a collection of three shapes with conditions:

        * if 0 < z < 1: F(x,y,z) = x² + y² - 1
        * if x² + y² < 1: F(x,y,z) = -z
        * if x² + y² < 1: F(x,y,z) = z-1
        
        $$\begin{align}
        F\_\text{body}(x,y,z) &= x^2 + y^2 - 1 & \text{if}&\quad 0<z<1 \\\\
        F\_\text{top}(x,y,z) &= z - 1 & \text{if}&\quad x^2+y^2 < 1 \\\\
        F\_\text{bottom}(x,y,z) &= -z & \text{if}&\quad x^2+y^2 < 1
        \end{align}$$

    * **(15 points)** Cone (bottom at the origin, top at (0,0,1), radius 1 at
the bottom, radius 0 at the top, with a bottom cap).
You handle this as a collection of two shapes with conditions:
    
        * if 0 < z ≤ 1: F(x,y,z) = x² + y² - (1 - z)²
        * if x² + y² < 1: F(x,y,z) = -z
        
        $$\begin{align}
        F\_\text{body}(x,y,z) &= x^2 + y^2 - (1-z)^2 & \text{if}&\quad 0<z \leq 1 \\\\
        F\_\text{bottom}(x,y,z) &= -z & \text{if}&\quad x^2+y^2 < 1
        \end{align}$$

    * **(15 points)** Cube (centered at the origin, with vertices
( ±1, ±1, ±1)$). Think of it as six planes:

        * if -1 ≤ y,z ≤ 1: F(x,y,z) = x-1
        * if -1 ≤ y,z ≤ 1: F(x,y,z) = -(x+1)
        * if -1 ≤ x,z ≤ 1: F(x,y,z) = y-1
        * if -1 ≤ x,z ≤ 1: F(x,y,z) = -(y+1)
        * if -1 ≤ x,y ≤ 1: F(x,y,z) = z-1
        * if -1 ≤ x,y ≤ 1: F(x,y,z) = -(z+1)

        $$\begin{align}
        F\_\text{right}(x,y,z)  &= x-1    & \text{if}&\quad -1 \leq y,z \leq 1 \\\\
        F\_\text{left}(x,y,z)   &= -(x+1) & \text{if}&\quad -1 \leq y,z \leq 1 \\\\
        F\_\text{top}(x,y,z)    &= y-1    & \text{if}&\quad -1 \leq x,z \leq 1 \\\\
        F\_\text{bottom}(x,y,z) &= -(y+1) & \text{if}&\quad -1 \leq x,z \leq 1 \\\\
        F\_\text{front}(x,y,z)  &= z-1    & \text{if}&\quad -1 \leq x,y \leq 1 \\\\
        F\_\text{back}(x,y,z)   &= -(z+1) & \text{if}&\quad -1 \leq x,y \leq 1
        \end{align}$$

    * **(bonus 25 points)** Mesh (arbitrary triangle meshes)

        * Intersect with all triangles of the mesh. None of the demo scene files use this, so you’ll have to create your own.

* **(5 points)** An example scene file. Note that `defaults.json` is not a
real example, it just contains sample parameters (matching what the
objects' constructors would set) or sample parameters (for the
transforms). Also note that if a transform dictionary contains a
`translate`, `rotate`, `scale`, and `matrix`, they will be applied to the object
in the order: `matrix * translate * rotate * scale`.

The code
--------

The code for a raycaster/raytracer can actually be quite compact. Here
is a walkthrough. When the program launches (`main.cpp`), the `main()`
function creates a `Scene` object. The scene parses the `.json` input file
(`parser.cpp`). The `main()` function then creates a `QImage` to store the
rendering result, and passes it to `scene.render()`. The code for
`Scene::render()` is in `scene.cpp`. You will fill in `Scene::render()` and
its helper methods:

* `CameraPerspective::getRay()` and `CameraOrthographic::getRay()`. These
methods take a camera-space *u,v* point as a parameter and return a 3D
`ray3`. The code goes in `camera.cpp`.

* `Scene::rayColor()`. This method returns the color along a 3D ray as a
floating point `vec3` with components in the range [0,1]. The code goes
in `scene.cpp`. The code for this is very short. You call simply call
`Scene::closestIntersection()`, and, if there is one, return its
`.material.color_diffuse`. Otherwise, you return black. (You will write a
more sophisticated algorithm in the next assignment.)

To implement `Scene::rayColor()`, you need to implement *its* helper
method `Scene::closestIntersection()`, which returns the closest
intersection with a shape in the scene along the given ray. The code for
that goes in `scene.cpp`.

Finally, to implement `Scene::closestIntersection()`, you will need to
implement *its* helper methods, which are `Shape::rayIntersect()` for each
of the `Shape` subclasses. The code for those goes in `shape.cpp`. The
`rayIntersect()` algorithms are what we have been deriving in class.
You can find our derivations in `docs/Ray Shape Intersection Formula.txt`.
You can find the pseudocode we created in class for the cylinder
in `docs/raycasting_cylinder.py`.
When you implement `rayIntersect()`,
you must fill out the `Intersection& hit_out` output parameter with
information about the intersection. Return `true` if an intersection
occurs and `false` otherwise. Remember that the incoming ray's point `.p`
and direction `.d` are in world-space. Convert them into object-space by
multiplying them by `Shape`'s method `transformInverse()`.

For this raycasting assignment, you only must fill in the `.t` and the
`.material` fields of `hit_out`. To fill out the `.material` field, simply
assign it from the `Shape`'s `material()` method. Note that for the next
assignment, you will have to fill out the rest of the fields. There's no
harm getting a head-start now. The `.position` and `.normal` fields should
be stored in world-space, so use your t with the world-space ray's
position and direction and multiply the normal by
`transpose(transformInverse())`.

C++ you need to know for this assignment
----------------------------------------

One of the most useful container types in C++ is `std::vector<T>`. It is
a list/array class. The `<T>` means that it stores values with type `T`.
(Because of the `<>`, it is called a templated type. Some programming
languages call this generics.) If you have an `std::vector<Foo> v`, you
can check if it is empty with `v.empty()`, you can get the number of
elements it contains with `v.size()`, you can access an element i with
`v[i]` or the bounds-checking version `v.at(i)`. There is also convenient
syntactic sugar in C++ for iterating over all elements of a container.
If you have a `std::vector<ShapePtr> my_shapes`, you can write:

        for( const ShapePtr shape: my_shapes ) {
            ... shape->rayIntersect( ... ) ...
        }

You may notice a `typedef` in one of the headers involving
`std::shared_ptr<T>`. Treat a `std::shared_ptr<T>` as just a regular
old `T*` (pointer). It is a reference counted pointer, so we don't have
to worry about freeing memory or memory leaks.

glm and C/C++ standard library functions you need for this assignment
---------------------------------------------------------------------

**glm.** This assignment makes heavy use of the `glm` library for vector
math. This library matches GLSL (OpenGL Shading Language), so substitute
the keyword GLSL for glm when searching for documentation. You will make
heavy use of `vec4`, `vec3`, `vec2`, `mat4`, possibly `mat3`, and functions like
`dot(v1,v2)`, `inverse(m)`, `transpose(m)`, and `clamp(v, min_value, max_value)`.
You can access the components of a `glm` vector `v` like an array
with `v[i]`. Note that the vector and matrix types have the arithmetic operators
+, -, *, / defined. The behavior depends on what is on the left and
right of the operator. For example, if both sides are vectors, `v1+v2` or
`v1/v2` will perform the addition or division
on each corresponding element of the vectors. If one
side is a scalar and the other is a vector or matrix, `v/5.0` will divide
each element by 5.0. Finally, with a matrix and a vector, `m*v` will
perform matrix multiplication. Note that if you have a `mat4 m` and then
call `mat3(m)`, it will take the top-left 3x3 part of `m`. Similarly with
the vec types; calling `vec3(v)` on a `vec4 v` will keep the first three xyz
components. You can also create higher-dimensional matrices and
vectors from lower-dimensional ones. For example, to create a `vec4` from
a `vec3 v`, use `vec4(v,1.0)`. That sets the 4th component w to 1.0.

**sqrt(x), fabs(x), lround(x), std::min(a,b), std::max(a,b)**. They are
part of C's math.h and C++'s `<algorithm>`. Note that `std::min` and
`std::max` require both parameters to have the exact same type. If not,
you will get a very long compiler error since they are generic functions
written using C++ templates.

Note: This is not necessarily a complete list. I may be forgetting some!

Qt you need for this assignment
-------------------------------

**QImage.** The method `image.setPixel(x,y,c)` to set the pixel x,y of a
`QImage` image to a `QRgb` color c. To get the width and height of the
image, use `image.width()` and `image.height()`.

**QRgb.** To create an RGB `QRgb` color, use `qRgb( red, green, blue )`.
Each of the parameters should be an integer number in the range
[0,255], inclusive.
