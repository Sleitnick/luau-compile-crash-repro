# Luau Compile Crash Reproduction

## Build

Build with cmake (or run the script below).
```sh
$ ./build.sh
```

## Run

```sh
$ # Windows
$ ./build/Debug/LuauIrExample.exe
```
```sh
$ # Others
$ ./build/Debug/LuauIrExample
```

### Crash

Observe the segfault. If attaching a debugger, this will capture the crash during `luau_load` within `lvmload.cpp` ([line 337](https://github.com/luau-lang/luau/blob/7aba73849f1a6f98e1bcf77aea2fdf86e1551ab8/VM/src/lvmload.cpp#L337) in version tag 0.699). The `name` string is null, coming from `readString` on [line 332](https://github.com/luau-lang/luau/blob/7aba73849f1a6f98e1bcf77aea2fdf86e1551ab8/VM/src/lvmload.cpp#L332).

## Notes

- Seems to only happen if the compiler options include `userdataTypes` with a type that does not show up in the code.
- Changing various other compiler options didn't seem to change anything.
- The userdata remapper callback must be set, but the contents of the callback do not matter (the crash happens when populating the arguments for the call; the function is never called).
