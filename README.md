# LISPInterpreter
 Exercise in interpretation
 Building a LISP interpreter in C because the syntax is simple, the interpreted language is interesting, and C is universal.

# Plans
 The plan is to add more primitives, a tracing evaluator, simple macros.
 But first, a garbage collector.

# Build
Use meson
  ```
  meson setup builddir
  meson compile
  ./lisp
   ```