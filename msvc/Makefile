CFLAGS = /nologo /MD /I ../libvix /c
LFLAGS = /nologo ../libvix/VixAllProducts.lib \
		 kernel32.lib user32.lib advapi32.lib ole32.lib oleaut32.lib ws2_32.lib shell32.lib
MFLAGS = /nologo

all:vmprun.exe

vmprun.exe:../src/vmprun.c
	cl $(CFLAGS) ../src/vmprun.c 
	link $(LFLAGS) vmprun.obj /out:vmprun.exe
	mt $(MFLAGS) -manifest vmprun.exe.manifest /outputresource:vmprun.exe;2
	rm vmprun.exe.manifest

clean:
	rm -rf *.obj

cleanall:clean
	rm -rf *.exe
