BASELIBDIR=..
include $(BASELIBDIR)/makefile.template

INCLUDEDIR=-I. -I..
VPATH=./:../tinyxml:../sqlite3/3.23.1

OBJ_TINYXML=tinyxml.o tinyxmlparser.o tinyxmlerror.o tinystr.o
OBJ_SQLITE3=sqlite3.o
OBJ_LIBBASE= ace.o AnalyIPRange.o Base64.o CircleAction.o CodeConver.o DbgPrint.o FileVersion.o function.o inifile.o iprange.o ipconfig.o MacroDefine.o MD5.o ProcessMan.o os.o RC4-LIB.o socket.o stdfile.o StringDetermine.o StringMatch.o task.o TimeFun.o tolists.o XmlReader.o RunSqlCommand.o RunSqlConnect.o RunSqlRead.o RunSqlRead.o RunSqlTransaction.o GetHardInfo.o user.o

all: libbaselibcore.a

libbaselibcore.a: $(OBJ_TINYXML) $(OBJ_SQLITE3) $(OBJ_LIBBASE)
	$(AR_CMD)

clean:
	rm -f *.o
