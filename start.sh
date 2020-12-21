echo "######## Cleanup"
rm -r ./build/resources
scons --clean
scons
echo "######## Build finished"
echo "Copy resources"
cp -r ./game/resources ./build/resources
./build/alchemy