# About

This is (going to be) an implementation of Conway's Game Of Life, written in C++ using Qt.

In addition to the simple 2D version of the game, I'd like to experiment with some 3D versions.

All logic for computing and evolving automata is implemented in plugins.

A simple implementation of Conway's Game Of Life is implemented in plugins/simplelife

# Building:

    git clone git://github.com/jl2/LifeAutomata.git
    cd LifeAutomata
    qmake
    make


# Running

    ./bin/qlife
