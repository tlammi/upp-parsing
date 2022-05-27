
.PHONY: test

test: build
	cd build && meson test

build:
	meson build

