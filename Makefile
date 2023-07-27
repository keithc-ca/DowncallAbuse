VERSION   := 21
JAVA_HOME := $(HOME)/opt/jdk-$(VERSION)

CLASSES   := bin/sample/DowncallAbuse.class

CXXFLAGS  := -fPIC -g -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/linux
LIBRARY   := libabuse.so
OBJECTS   := abuse.o

.PHONY : all clean run

all : $(CLASSES) $(LIBRARY)

clean :
	rm -f $(CLASSES) $(LIBRARY) $(OBJECTS)

run : all
	$(JAVA_HOME)/bin/java -cp bin --enable-preview -Djava.library.path=. --enable-native-access=ALL-UNNAMED sample.DowncallAbuse

bin/sample/DowncallAbuse.class : src/sample/DowncallAbuse.java
	$(JAVA_HOME)/bin/javac -d bin --enable-preview --source $(VERSION) src/sample/DowncallAbuse.java

$(LIBRARY) : $(OBJECTS)
	gcc -o $@ -g -shared $(OBJECTS)
