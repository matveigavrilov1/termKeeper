#!/bin/bash

VENV_DIR="${1:-.venv}"

export POETRY_VIRTUALENVS_IN_PROJECT=false
export POETRY_VIRTUALENVS_PATH="${VENV_DIR}"

poetry install