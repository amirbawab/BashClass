#!/bin/bash

RESOURCE_DIR="${PWD}/resources/src"
cmake . \
    -DSYNTAX_ERRORS="${RESOURCE_DIR}/syntax_errors.json"\
    -DSYNTAX_GRAMMAR="${RESOURCE_DIR}/grammar.json"\
    -DLEXICAL_ERRORS="${RESOURCE_DIR}/lexical_errors.json"\
    -DLEXICAL_CONFIG="${RESOURCE_DIR}/lexical_config.json"\
    -DLEXICAL_STATE_MACHINE="${RESOURCE_DIR}/lexical_graph.json"
