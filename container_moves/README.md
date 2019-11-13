
Results:

    ctor

    >>> Inlined Container Test
        ctor
        >>> byValueTest
            copy
            move-assign
            dtor
            ctor
            move-assign
            dtor
            move
            move-assign
            dtor
        <<< byValueTest
        >>> byRefTest
            copy-assign
            ctor
            move-assign
            dtor
            move-assign
        <<< byRefTest
        dtor
    <<< Inlined Container Test

    >>> Extern Container Test
        ctor
        >>> byValueTest
            copy
            move-assign
            dtor
            ctor
            move-assign
            dtor
            move
            move-assign
            dtor
        <<< byValueTest
        >>> byRefTest
            copy-assign
            ctor
            move-assign
            dtor
            move-assign
        <<< byRefTest
        dtor
    <<< Extern Container Test
    dtor


```
InlinedContainerTest:
    ByValue  9 ops
    ByRef    5 ops
ExternContainerTest:
    ByValue  9 ops
    ByRef    5 ops
```
