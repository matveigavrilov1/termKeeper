#!/bin/bash
set -euo pipefail

SCRIPT_DIR=$(dirname $0)

source /etc/os-release
source ${SCRIPT_DIR}/../poetry/install.sh ".venv-${ID}-${VERSION_ID}"

poetry run doit conan-install
