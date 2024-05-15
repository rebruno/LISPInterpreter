# LISPInterpreter
 Writing an interpreter exercise.
 Building a LISP interpreter in C because the syntax is simple, the interpreted language is interesting, and C is universal.

# Plans
 Writing a garbage collector is next. 

# Build
Use meson to compile. Any compiler that supports the standard C library will work.
  ```
  meson setup builddir
  meson compile
  ./lisp
   ```