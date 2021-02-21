# ScopeGuard
A header-only, modern C++ ScopeGuard facility that provide multiple usages which are simple and easy to use.

**NOTE:** It's NOT finished yet but basically usable.

### Simple examples

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












