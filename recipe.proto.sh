#!/usr/bin/zsh

poppy install -fs
rm -rf target/sources
rm -rf target/build
rm -rf target/install
rm -rf pack/*
mkdir -p target/sources/constellation
cp -r * ./target/sources/constellation
rm -rf ./target/sources/constellation/.git
rm -rf ./target/sources/constellation/build
rm -rf ./target/sources/constellation/cmake-build-debug
rm -rf ./target/sources/constellation/cmake-build-release
rm -rf ./target/sources/constellation/target
rm -rf ./target/sources/constellation/dependencies
rm -rf ./target/sources/constellation/pack
mkdir -p target/build
mkdir -p target/install
cmake -GNinja -B ./target/build -DCMAKE_PREFIX_PATH=./dependencies -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=./target/install/static -S .
cmake --build ./target/build
cmake --install ./target/build
cd target/sources || exit
tar --exclude="sources" --exclude="." --exclude="dependencies" --exclude="cmake-build-debug" -czf sources.tar.gz *
cd ../.. || exit
mkdir -p pack
cp target/sources/sources.tar.gz pack

cp $(ls | grep -E '^@|.*poppy.*.toml|pppm.toml') target/install/static
cd target/install/static || exit
tar --exclude="static" --exclude="." --exclude="dependencies" -czf static.tar.gz *
cd ../../.. || exit
cp target/install/static/static.tar.gz pack

poppy push ./pack/sources.tar.gz --arch any --distribution sources
poppy push ./pack/static.tar.gz --arch linux-x64 --distribution static

rm -rf target/sources
rm -rf target/build
rm -rf target/install
rm -rf pack/*