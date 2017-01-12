# $Id: Makefile,v 1.2 2004/10/12 09:06:17 marquet Exp $
##############################################################################

ROOTDIR=/home/enseign/ASE

CC	= gcc
CFLAGS	= -Wall -W -m32 -pthread -g
LIBDIR  = $(ROOTDIR)/lib 
INCDIR  = -I$(ROOTDIR)/include 
LIBS    = -L$(LIBDIR) -lhardware

###------------------------------
### Main targets 
###------------------------------------------------------------
BINARIES= mkhd dmps frmt dvol mkvol mknfs dfs test_bloc_libre
OBJECTS	= $(addsuffix .o,\
mkhd dmps frmt dvol mkvol mknfs dfs test_bloc_libre drive mbr super inode)

all: $(BINARIES) $(OBJECTS)


###------------------------------
### Main rules 
###------------------------------------------------------------

mkhd.o: mkhd.c
	$(CC) $(CFLAGS) -c mkhd.c $(INCDIR)
mkhd: mkhd.o
	$(CC) $(CFLAGS) -o mkhd mkhd.o $(LIBS)
dmps.o: dmps.c
	$(CC) $(CFLAGS) -c dmps.c $(INCDIR)
dmps: dmps.o drive.o
	$(CC) $(CFLAGS) -o dmps dmps.o drive.o $(LIBS)
frmt.o: frmt.c
	$(CC) $(CFLAGS) -c frmt.c $(INCDIR)
frmt: frmt.o drive.o
	$(CC) $(CFLAGS) -o frmt frmt.o drive.o $(LIBS)
dvol.o: dvol.c
	$(CC) $(CFLAGS) -c dvol.c $(INCDIR)
dvol: dvol.o drive.o mbr.o
	$(CC) $(CFLAGS) -o dvol dvol.o drive.o mbr.o $(LIBS)
mkvol.o: mkvol.c
	$(CC) $(CFLAGS) -c mkvol.c $(INCDIR)
mkvol: mkvol.o drive.o mbr.o
	$(CC) $(CFLAGS) -o mkvol mkvol.o drive.o mbr.o $(LIBS)
mknfs.o: mknfs.c
	$(CC) $(CFLAGS) -c mknfs.c $(INCDIR)
mknfs: mknfs.o drive.o mbr.o super.o
	$(CC) $(CFLAGS) -o mknfs mknfs.o drive.o mbr.o super.o $(LIBS)
dfs.o: dfs.c
	$(CC) $(CFLAGS) -c dfs.c $(INCDIR)
dfs: dfs.o drive.o mbr.o super.o
	$(CC) $(CFLAGS) -o dfs dfs.o drive.o mbr.o super.o $(LIBS)
test_bloc_libre.o: test_bloc_libre.c
	$(CC) $(CFLAGS) -c test_bloc_libre.c $(INCDIR)
test_bloc_libre: test_bloc_libre.o drive.o mbr.o super.o
	$(CC) $(CFLAGS) -o test_bloc_libre test_bloc_libre.o drive.o mbr.o super.o $(LIBS)
drive.o: drive.c
	$(CC) $(CFLAGS) -c drive.c $(INCDIR)
mbr.o: mbr.c
	$(CC) $(CFLAGS) -c mbr.c $(INCDIR)
super.o: super.c
	$(CC) $(CFLAGS) -c super.c $(INCDIR)
inode.o:inode.c
	$(CC) $(CFLAGS) -c inode.c $(INCDIR)

###------------------------------
### Misc.
###------------------------------------------------------------
.PHONY: clean realclean depend
clean:
	$(RM) *.o $(BINARIES)
realclean: clean 
	$(RM) vdiskA.bin vdiskB.bin
