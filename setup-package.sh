#!/bin/bash

script_dir=$(dirname "${BASH_SOURCE[0]}")
chmod 755 $script_dir/build-package.sh
$script_dir/build-package.sh
