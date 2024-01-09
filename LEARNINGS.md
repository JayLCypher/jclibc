# Things I have learned

## There is no pass-by-reference

Pass-by-reference is a concept in C/C++/(Rust?) where an argument is passed as a reference to a variable where you don't copy the variable.
This doesn't exist.

In reality, you have pass-by-copy. Either pass-by-value-copy or pass-by-value-reference-copy, but it's all copies.
It's all copies...

So if you want a pass-by-ref or pass-by-mut-ref, you need MORE POINTERS TO POINTERS!!!
void f(char) <- pass-by-value
void f(const char) <- pass-by-value const
void f(char *) <- pass-by-ref
void f(char*const) <- pass-by-ref (not const, because it's a temp object copy).
void f(char *const*) <- pass-by-ref-const-ref

So for a function, having a top const on the uppermost reference doesn't matter, because it's a copy.

## Short-strokes

Always be careful about if / while / for exit-conditions. Short-stroking is great, but can be dangerous.
Consider the following.

```c
int func(int i) {
    if (i == 0 || (i += 1) > 0) { return i; }
    return i-1;
}
```

here, the reassignment of i is lost if i == 0 because of short-stroke logical OR statement.

## Careful who you call clever

The standard library and compiler implementors are rightfully considered smart people.
Some of the newer ones may be standing on the shoulders of giants, but damn.
Think of strlen().
Oh yeah, just walk the array until you find '\0'
OR!
You leverage how computers work to walk bytes sized chunks and do some magic number shenanigcans and bit comparisons.
That way you can basically walk multiple bytes per iteration.

Brilliant. If you can, take a look at gcc strlen.
