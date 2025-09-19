## How to build?

### Step 0: Download CMake

### Step 1: Download and set up vcpkg

Download vcpkg anywhere on your computer (not in our repo):
```git clone https://github.com/microsoft/vcpkg.git```

Then run the bootstrap script
```cd vcpkg && ./bootstrap-vcpkg.sh```

Then setup the following variables
```
export VCPKG_ROOT=/path/to/vcpkg
export PATH=$VCPKG_ROOT:$PATH
```
I would suggest you try to make the above variables persistent, 
but you might possibly run into issues with VS Code CMake extension not recognizing these.

### Step 2: Clone the repo

### Step 3: Set up a CMakeUserPresets.json file

In the root directory of the repo, create a file names `CMakeUserPresets.json`.
This file should have the following content:
```
{
    "version": 2,
    "configurePresets": [{
        "name": "default",
        "inherits": "vcpkg",
        "environment": {
            "VCPKG_ROOT": "<ENTER PATH TO VCPKG HERE>"
        }
    }]
}
```
where you must replace the path to vcpkg obviously.
The existing `CMakePresets.json` file uses the environment setup by `CMakeUserPresets.json`.

### Step 4: Configure, Build, and Run!

I highly recommend using the VS Code CMake extension to do this.
Otherwise, I suggest looking online on how to compile with CMake.
Note that the current setup uses the `default` preset 
(i.e. you'll need to configure with `--preset default` or something).

The compiled executable will be in the `build` directory.

### References
- https://learn.microsoft.com/en-us/vcpkg/get_started/get-started?pivots=shell-bash
