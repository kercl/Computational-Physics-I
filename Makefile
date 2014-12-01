CC=g++
CFLAGS=-O2 -std=c++11
LDFLAGS= -lgsl -lgslcblas -lm
LIBSRC = lib/csv.cc \
         lib/converter.cc \
         lib/diffgl.cc \
         lib/snapshot.cc

cp1e1:
	-rm exec_cp1e1 2>/dev/null || true
	$(CC) $(CFLAGS) cp1e1.cc $(LIBSRC) $(LDFLAGS) -o exec_cp1e1

cp1e9:
	-rm exec_cp1e9 2>/dev/null || true
	$(CC) $(CFLAGS) cp1e9.cc $(LIBSRC) $(LDFLAGS) -o exec_cp1e9

cp1e10:
	-rm exec_cp1e10 2>/dev/null || true
	$(CC) $(CFLAGS) cp1e10.cc $(LIBSRC) $(LDFLAGS) -o exec_cp1e10

cp2e1:
	-rm exec_cp2e1 2>/dev/null || true
	$(CC) $(CFLAGS) cp2e1.cc $(LIBSRC) $(LDFLAGS) -o exec_cp2e1

cp3e1:
	-rm exec_cp3e1 2>/dev/null || true
	$(CC) $(CFLAGS) cp3e1.cc $(LIBSRC) $(LDFLAGS) -o exec_cp3e1

cp3e2:
	-rm exec_cp3e2 2>/dev/null || true
	$(CC) $(CFLAGS) cp3e2.cc $(LIBSRC) $(LDFLAGS) -o exec_cp3e2

cp3e4:
	-rm exec_cp3e4 2>/dev/null || true
	$(CC) $(CFLAGS) cp3e4.cc $(LIBSRC) $(LDFLAGS) -o exec_cp3e4

