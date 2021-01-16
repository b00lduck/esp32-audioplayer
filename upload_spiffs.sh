#!/bin/bash

set -e

cd mp3duck-gui
npm run build

rm -rf ../data/*
mkdir ../data/css
mkdir ../data/js
cp dist/index.html ../data/
cp dist/css/*.css ../data/css/
cp dist/js/*.js ../data/js

cd ..

pio run --environment nodemcu-32s --target uploadfs