#!/usr/bin/env bash
if [ -z "$1" ]; then
    echo "Usage: deploy.sh <target>"
    echo "  Example: deploy.sh phoenix@1.2.3.4"
    exit 1
fi

(find build/lib -type f -iname "*.so" ; find build/bin ; find configs ; find . -type f -iname "*.sh" ) \
| rsync -v -u --files-from=- . "$1:~/deploy_auto_drive-$USER"
