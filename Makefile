objs = start.o init.o nand.o exception.o leds.o uart.o timer.o key.o main.o my_printf.o nor.o string_utils.o lib1funcs.o
objs += lcd/s3c2440_lcd_controller.o
objs += lcd/lcd_controller.o
objs += lcd/lcd_4_3.o
objs += lcd/lcd_device.o
objs += lcd/lcd.o
objs += lcd/framebuffer.o
objs += lcd/font_8x16.o
objs += lcd/font.o
objs += lcd/lcd_test.o

objs += ts/ts_test.o
objs += ts/touchscreen.o
objs += ts/adc.o
objs += ts/tslib.o

objs += i2c/at24cxx.o
objs += i2c/i2c_controller.o
objs += i2c/i2c_test.o
objs += i2c/s3c2440_i2c_controller.o

all: $(objs)
	arm-linux-ld -T linker.lds $^ libgcc.a -o i2c.elf
	arm-linux-objcopy -O binary -S i2c.elf i2c.bin
	arm-linux-objdump -D i2c.elf > i2c.dis

clean:
	rm *.bin *.o *.elf *.dis lcd/*.o ts/*.o i2c/*.o

.PHONY:	clean

%.o: %.c
	arm-linux-gcc -march=armv4 -c -o $@ $<

%.o: %.S
	arm-linux-gcc -march=armv4 -c -o $@ $<

