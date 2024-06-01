# Voxel Blaze

Techniques and algorithms for rasterizing cubic Voxels efficiently.

## Instructions

Make sure you have C/C++ development tools installed for your operating system.
You will also need the
[Meson Build System](https://mesonbuild.com/Getting-meson.html) together with
[Ninja Build](https://ninja-build.org/). To install those on Windows, refer to
the
[Installing Meson and Ninja with the MSI Installer](https://mesonbuild.com/Getting-meson.html#installing-meson-and-ninja-with-the-msi-installer)
section of the Meson documentation. Most Linux distributions provide packages
for Meson and Ninja.

1. Clone the repository.

```sh
git clone https://github.com/danielwolbach/voxel-blaze
```

2. Navigate into the project directory.

```sh
cd voxel-blaze
```

3. Setup Meson.

```sh
meson setup build
```

4. Compile the project.

```sh
meson compile -C build
```

5. Run the executable.

```sh
build/voxel-blaze
```

If you are using Wayland on Linux, add the `-Dglfw:display-api=wayland` flag to `meson setup build`.
