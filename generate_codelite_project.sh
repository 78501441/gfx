#!/bin/sh

rm -fr build/*
cd build && CC=/usr/local/bin/tcc cmake .. -G "CodeLite - Unix Makefiles"
