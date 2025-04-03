
.PHONY: flash rtt menuconfig build clean-build clean

flash:
	west flash

rtt:
	west rtt

menuconfig:
	west build -t menuconfig

build:
	west build -b nrf52840dk/nrf52840 .

clean-build:
	west build -p always -b nrf52840dk/nrf52840 .

clean:
	rm -rf build
