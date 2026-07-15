initBuild:
	mkdir -p build && cd build && cmake .. && cmake --build .

build:
	cd build && cmake --build .

run:
	cd build && ./CPA

.PHONY: initBuild build run


