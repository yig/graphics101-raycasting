Computer Graphics - Homework Assignment 2 - Raycasting
======================================================

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

Goals:
------

* Understand how to create a virtual picture of a 3D scene.

* Gain experience deriving and implementing mathematical expressions for
geometric calculations.

* Become familiar with a 3D math library (bonus: identical to one
available when programming for a GPU).

* Become more comfortable with C++.

Background:
-------

* Book (FoCG,4e): Chapter 4 *Ray Tracing* and Chapter 13.2 *Instancing*.
* Video: "Lecture 3: Transformations", "Lecture 4: Raycasting", and "Assignment 2: Raycasting"
* Quiz: Transformations, Raycasting

(FoCG,4e is *Fundamentals of Computer Graphics (4th edition)* by Steve Marschner and Peter Shirley.)

Getting Started & Handing In:
-----------------------------

* Download or clone this code repository. Don't fork it on GitHub, or else your code will be visible to everyone.

* Follow the instructions to install a working development environment: <https://github.com/yig/graphics101> . You do not need to install Qt or any other external libraries for this assignment.

* The program is a command line program. The framework and included `glm` vector math library provide all the support code that you need.

* The code will be written in C++. This time, the project is more complex
and you will see and need to use more of the language, including the
`std::vector` container class and object-oriented programming. I have made
use of the modern C++11 standard where appropriate. This makes the
language easier to write and safer, though some reference material,
tutorials, and examples you find will be outdated.

* Build and run the code. The code should compile, but it will complain
when running about not having enough arguments. You should see a message
like:

        Usage: raycasting path/to/scene.json image_out.png long_edge_pixels

* If you are using an IDE like Qt Creator, you will need to set the command line arguments used when running your program. In Qt Creator, click on
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
great debugger interface.

* Check your work with the command line tests by running `ctest --verbose`. This will test whether your Camera and Scene classes are working correctly. If your Camera and Scene classes are not working correctly, nothing will render correctly for the autograder.

* Check your work with the [autograder](https://github.com/yig/graphics101-raycasting-autograder).

* Copy the latest autograder output (`.html` file and associated directory) into a new `output/` subdirectory.

* Create a `.json` scene yourself. Copy it and a `.png` rendering of it into the `output/` subdirectory as well.

* You are encouraged to share blooper images you create while implementing the assignment on Piazza.

* Create a file named `Notes.txt` in the folder. Describe any known issues or extra features. Name people in the class who deserve a star for
helping you (not by giving your their code!).

* When done, run the the `cpack` command from inside your build directory
to generate an appropriate zip file of your `raycasting` project.
The zip file it creates, `raycasting.zip`, will include
the `output` subdirectory and your `Notes.txt` file.
It will ignore unneeded large and numerous directories
(e.g., `build` and `include`).
Upload your `raycasting.zip` before the deadline.
**If you try to upload a zip file with too many files in it, Gradescope will return error code 0.**
(If you can't use `cpack`, try `make zip` or `cmake --build . --target zip`)
If you create a zip file manually, zip your project without the `build` directory.

* **THIS IS AN INDIVIDUAL, NOT A GROUP ASSIGNMENT. That means all code
written for this assignment should be original! Although you are
permitted to consult with each other while working on this assignment,
code that is substantially the same will be considered cheating.**


Rubric
------

* **(10 points)** 3D rays through pixels. The `Camera` methods:

    * **(5 points)** `CameraPerspective::getRay( u,v )`

        * Return a world-space ray through the pixel located at (*u,v*) on the
film plane. A point (*u,v*) on the film plane is located at
**e** + *u* **u** + *v* **v** - *d* **w** in world-space.
Your ray should emanate from the eye **e** itself.
The world-space direction of the ray is the direction from
the eye **e** through the aforementioned film plane point:
*u* **u** + *v* **v** - *d* **w**.

    * **(5 points)** `CameraOrthographic::getRay( u,v )`

        * Return a world-space ray through the pixel located at (*u,v*) on the
film plane. A point (*u,v*) on the film plane is located at
**e** + *u* **u** + *v* **v** in world-space.
 The world-space
rays of an orthographic camera are all parallel, in the direction
-**w**.

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

    * **(15 points)** Plane (the *xy* plane, also known as the
*z* = 0 plane)

        * F(x,y,z) = z

    * **(15 points)** Sphere (centered at the origin with radius 1):

        * F(x,y,z) = x² + y² + z² - 1

    * **(15 points)** Cylinder (bottom at the origin, top at (0,0,1), radius 1)
with a top and bottom cap (circles with radius 1 at z=0 and z=1).
You handle this as a collection of three shapes with conditions:

        * if 0 < z < 1: F(x,y,z) = x² + y² - 1
        * if x² + y² < 1: F(x,y,z) = -z
        * if x² + y² < 1: F(x,y,z) = z-1
        
    * **(15 points)** Cone (bottom at the origin, top at (0,0,1), radius 1 at
the bottom, radius 0 at the top, with a bottom cap).
You handle this as a collection of two shapes with conditions:
    
        * if 0 < z ≤ 1: F(x,y,z) = x² + y² - (1 - z)²
        * if x² + y² < 1: F(x,y,z) = -z

    * **(15 points)** Cube (centered at the origin, with vertices
( ±1, ±1, ±1)). Think of it as six planes:

        * if -1 ≤ y,z ≤ 1: F(x,y,z) = x-1
        * if -1 ≤ y,z ≤ 1: F(x,y,z) = -(x+1)
        * if -1 ≤ x,z ≤ 1: F(x,y,z) = y-1
        * if -1 ≤ x,z ≤ 1: F(x,y,z) = -(y+1)
        * if -1 ≤ x,y ≤ 1: F(x,y,z) = z-1
        * if -1 ≤ x,y ≤ 1: F(x,y,z) = -(z+1)

    * **(bonus 25 points)** Mesh (arbitrary triangle meshes)

        * Intersect with all triangles of the mesh. None of the demo scene files use this, so you’ll have to create your own.
        The `Mesh` class stores a triangle mesh. A triangle has three corners. At each corner there is a position and possibly also a normal and texture coordinate. So there are three corresponding arrays, `face_positions`, `face_normals`, and `face_texcoords`. (If the mesh has normals or texture coordinates, then `face_normals` or `face_texcoords` will be the same length as `face_positions`. If it doesn't, their length will be 0.) Each element in these arrays stores three integer indices, one for each corner of the triangle. The indices tell you where to look in the `positions`, `normals`, and `texcoords` arrays for the 3D position, 3D normal, and 2D texture coordinates at that corner. Putting this all together, you can access the 3D positions for the three corners of the `i`-th triangle like this:

                const vec3 p0 = positions[face_positions[i][0]];
                const vec3 p1 = positions[face_positions[i][1]];
                const vec3 p2 = positions[face_positions[i][2]];


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
(`parser.cpp`). The `main()` function then creates an `Image` to store the
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
you must return information about the intersection in an `Intersection` struct.
Set `.valid = true` if an intersection
occurs and `.valid = false` otherwise. Remember that the incoming ray's point `.p`
and direction `.d` are in world-space. Convert them into object-space by
multiplying them by `Shape`'s method `transformInverse()`.
It should be matrix times vector, not vector times matrix.
Transformation matrices are 4x4, since they make use of
homogeneous coordinates to perform translation.
The homogeneous coordinate of a point should be 1,
since points have fixed positions that should be translated.
The homogeneous coordinate of a vector should be 0,
since vectors do not have fixed positions in space and so
translation is a no-op. (Imagine a vector `<1,0,0>`
pointing along the x-axis. Translating it by `<-2,0,0>` should do nothing;
adding the translation would result in the vector `<-1,0,0>`, which is
the opposite of correct.)

For this raycasting assignment, you only must fill in the `.valid`, `.t`, and
`.material` fields of the returned `Intersection`. To fill out the `.material` field, simply
assign it from the `Shape`'s `material()` method. Note that for the next
assignment, you will have to fill out the rest of the fields. There's no
harm getting a head-start now. The `.position` and `.normal` fields should
be stored in world-space. The incoming ray is in world-space, so use your
t with the world-space ray's position and direction to get a world-space
position. Transform the object-space normal by `transpose(transformInverse())`
to get a world-space normal.

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

`glm` and C/C++ standard library functions you need for this assignment
---------------------------------------------------------------------

**glm.** This assignment makes heavy use of the `glm` library for vector
math. This library implements the [vector and matrix data types from the OpenGL Shading Language (GLSL)](https://www.khronos.org/opengl/wiki/Data_Type_(GLSL)), so substitute
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
part of C's math.h (in C++ included as `<cmath>`)
and C++'s `<algorithm>`. Note that `std::min` and
`std::max` require both parameters to have the exact same type. If not,
you will get a very long compiler error since they are generic functions
written using C++ templates.

Note: This is not necessarily a complete list. I may be forgetting some!

Support code functions you need for this assignment
-----------------------------------------

**Image.** The method `image.pixel(x,y) = c` to set the pixel x,y of an
`Image` image to a `ColorRGBA8` color c. To get the width and height of the
image, use `image.width()` and `image.height()`. The top left pixel is (0,0), not the bottom left pixel.

**ColorRGBA8.** To create an RGB `ColorRGBA8` color, use `ColorRGBA8( red, green, blue )`.
Each of the parameters should be an integer number in the range
[0,255], inclusive.
