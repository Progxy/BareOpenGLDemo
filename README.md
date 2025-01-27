## BareOpenGLDemo
The main purpose was to demo a 3d model rendering application using minimal dependencies.
The matrix based calculations are perfomed using only the `glibc math.h library`.
The parsing of the glTF (only format currently supported) is possible thanks to [glTF](https://github.com/Progxy/glTF).
For the internal parsing of images in glTF is used (imageReader)[https://github.com/Progxy/imageReader].

### Use
To use it just run `build.sh` and then `run.sh`.
(After installing OpenGL).
Note that the main purpose is to be a reference for personal implementations.
