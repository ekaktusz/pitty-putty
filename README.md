![banner](./assets/logo.png)

Pitty-Putty is a small demonstration game for [bägla](https://github.com/ekaktusz/bagla-engine). It aims to be a very simple and barebones Jazz Jack Rabbit clone. Consider it as a learning project.

Lets go!

To start working on the repo:

## Linux
git submodule update --init --recursive
sudo apt install autoconf libtool libudev-dev libx11-dev libxrandr-dev libxcursor-dev libgl1-mesa-dev
cp hooks/pre-commit .git/hooks/ && chmod +x .git/hooks/pre-commit

## Windows (from PowerShell)
Copy-Item hooks/pre-commit .git/hooks/