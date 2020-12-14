#! /usr/bin/env bash

pandoc\
    --from=gfm\
    --metadata title="README"\
    -s\
    -o README.html\
    README.md
