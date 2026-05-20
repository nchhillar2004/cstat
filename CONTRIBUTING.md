# Contributing

To contribute:

1. Fork the repo
2. Create a new branch. Prefer names like:
    - `feat/feature-name`
    - `fix/fix-name`
3. Make your changes, following our [code style](#code-style-mandatory)
4. Open a Pull Request

> [!NOTE]
> Issues will not be assigned.
>
> If you can solve an issue, just open a PR.
>
> If you want guidance or discussion first, comment under the issue before opening a PR.

## Pull request rules (mandatory)

- Link the issue at the top  
  (if no issue exists, create one first)

- Provide a `Summary` of your changes  
  (at least 2 points)

```md
Fixes: #1

## Summary
- 
-
```

## Code style (mandatory)

- Public functions and macros (usable in other files) use `camelCase`
- Internal helper functions and macros use a `_` (underscore) prefix and follow `snake_case`
- Do not forget to add comments 

For every `.h` file we follow this structure:

```c
/*
  file_name.h
  brief description
*/

#ifndef cstat_file_name_h
#define cstat_file_name_h

/* ===== MACROS ===== */

// global constants, UPPER_SNAKE_CASE
#define CONSTANT_MACRO ""

// public macros, camelCase
#define publicMacroFunction()

// internal/private macros, UPPER_SNAKE_CASE with `_` prefix
#define _HELPER_MACRO()


/* ===== FUNCTIONS ===== */

// public functions, camelCase
void sampleFunction(void);

// internal/private functions, snake_case with `_` prefix
void _sample_helper_function(void);

#endif
```
