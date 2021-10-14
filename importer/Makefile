build-lib:
	git clone https://github.com/assimp/assimp
	cmake assimp/CMakeLists.txt
	make assimp/ -j4

build-lib-ex:
	cmake assimp/CMakeLists.txt
	make assimp/ -j4

build-importer:
	g++ importer.cpp -Lassimp/bin -Iassimp/include -o importer