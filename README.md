# ScopeGuard
A header-only, modern C++ ScopeGuard facility that provided three easy ways to use.

## Simple examples

```C++
#include "ScopeGuard.h"
...
{
    ...
    ON_SCOPE_EXIT{
        // callback statments
    };
    ...
} // callback statments are executed at this point
```

```C++
...
{
    void my_callback();
    ...
    SCOPEGUARD(my_callback);
    ...
} // my_callback is invoked at this point
```

```C++
...
{
    void my_callback();
    ...
    auto sg = sg::MakeScopeGuard(my_callback);
    ...
} // my_callback is invoked at this point
```

You could also see the comments in file ScopeGuard.h.












