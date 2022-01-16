# TODO

## Install
Just download the todo binary

## Build from source
Have g++ installed
Run the following to create the todo binary
```console
g++ main.cpp Item.cpp -o todo -std=c++20
```

## Usage
alias it => in `~/.zshrc` or `~/.bashrc` or whatever, put `alias todo=<FULL-PATH-TO-WHERE-YOU-INSTALLED-IT>`

```console
todo
todo add [...items...]
todo done [...item id's...]
todo remove [...item id's...]
```
