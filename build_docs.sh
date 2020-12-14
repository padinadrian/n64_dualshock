#! /usr/bin/env bash

pandoc\
    --from=gfm \
    -s \
    -o README.html \
    README.md
