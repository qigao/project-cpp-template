# cpp_core_project

LGTM Has to be set up manually after template is created:

## About cpp_vcpkg_project
A production-ready C++ project  based on [project_options](https://github.com/aminya/project_options.git)

## Usage

```sh
cmake --list-presets all                    # List all CMake presets
cmake --preset windows                      # Configure
cmake --build --preset windows              # Build
ctest --preset windows                      # Test
cmake --build --preset windows -t install   # Install
```

## More Details

 * [Dependency Setup](./readme/dependencies.md)
 * [Building Details](./readme/building.md)
 * [Docker](./readme/docker.md)
 