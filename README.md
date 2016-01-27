RVLM FDTD Solver Library
========================

(In early development.)

Runtime dependencies
--------------------

 1. NetCDF
 2. OpenCL

Developer's environment
-----------------------

In order to compile the library and participate in its development, you will
need the following additional software:

 1. C89-compliant compiler (GCC and Clang are fine)
 2. CMake
 3. CppCheck
 4. Valgrind
 5. CUnit
 6. Python (with Jinja2 package)

It's possible to compile the library without points 3–6, but any contributions
aren't going to be accepted if they aren't supplied with proper unit-tests
passing without memory leaks.
