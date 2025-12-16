build=cmake-build
compile_database=compile_commands.json

${compile_database}: build
	cp cmake-build/compile_commands.json .

.PHONY: build
build: ${build}
	cmake --build cmake-build

${build}:
	cmake -GNinja -S . -B cmake-build

.PHONY: download
download:
	cmake -GNinja -S . -B cmake-build -DFETCHCONTENT_FULLY_DISCONNECTED=OFF
	cmake -GNinja -S . -B cmake-build -DFETCHCONTENT_FULLY_DISCONNECTED=ON

.PHONY: clean
clean:
	rm -rf cmake-build .cache compile_commands.json
