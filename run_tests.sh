#!/usr/bin/env bash

# Run in temporary subshell
(
    cd build;
    make embedded_tools_tests;
    bin/embedded_tools_tests
)