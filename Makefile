# 定义默认的构建类型为 Debug
BUILD_TYPE ?= Debug
# 定义构建目录和编译数据库文件
build = cmake-build
compile_database = compile_commands.json

# 主要目标：生成编译数据库
${compile_database}: ${build}
	cp ${build}/compile_commands.json .

# 伪目标，触发构建
.PHONY: build
build: ${build}
	cmake --build ${build}

# 配置 CMake 构建系统
# 注意：这里使用 := 确保 BUILD_TYPE 的值是立即确定的
${build}:
	cmake -GNinja -B ${build} -DGLFW_BUILD_WAYLAND=ON -DGLFW_BUILD_X11=OFF -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)

# 专门用于 Release 构建的目标
.PHONY: release
release:
	$(MAKE) BUILD_TYPE=Release

# 清理构建产物
.PHONY: clean
clean:
	rm -rf ${build} .cache ${compile_database}
