# LISPInterpreter
 Writing an interpreter as an exercise.
 Implementing it in C because it's universal.
 Choosing a LISP because the syntax is simple.

# Plans
 Writing a garbage collector is next. 

# Build
Use meson to compile. Any compiler that supports the standard C library will work.
  ```
  meson setup builddir
  meson compile
  ./lisp
   ```