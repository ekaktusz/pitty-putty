![banner](./assets/logo.png)

Pitty-Putty is a small demonstration game for [bägla](https://github.com/ekaktusz/bagla-engine).

Lets go!

To start working on the repo:

## Linux
Install cmake and ninja.

Initalize submodules after clone:
```sh
git submodule update --init --recursive
```

Install dependencies for building:
```sh
sudo apt install autoconf libtool libudev-dev libx11-dev libxrandr-dev libxcursor-dev libgl1-mesa-dev
```

Setup hooks (currently not used)
```sh
cp hooks/pre-commit .git/hooks/ && chmod +x .git/hooks/pre-commit
```

To build, from the repo root do:
```sh
./build.sh
```
## Windows (from PowerShell)
Setup hooks
```
Copy-Item hooks/pre-commit .git/hooks/
```