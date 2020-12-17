#!/usr/bin/env bash

# Run in temporary subshell
(
    cd build &&
    make run_embedded_tools_tests
    make run_n64_dualshock_tests
)