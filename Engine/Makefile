CXX := g++
CXXFLAGS := -g -std=c++11 -fPIC -Wall
CXXFLAGS += -I include/
CXXFLAGS += -I ../dependencies/glm-0.9.4.1
CXXFLAGS += -I ../dependencies/
CXXFLAGS += -DGLEW_STATIC -DGLEW_NO_GLU

OBJDIR := build

#deps
LDFLAGS += -lGLEW
LDFLAGS := -lGL

CPP_FILES := $(wildcard src/*.cpp)
CPP_FILES :=
OBJ_FILES := $(patsubst %.cpp, %.o, $(CPP_FILES))

lib_Engine.a : $(OBJ_FILES) SFML
	#$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^
	@echo("Entering lib_Engine.a target...")
	ar r $@ $^
	
$(OBJDIR)/%.o : src/%.cpp
	@echo("Entering objects target : "+$(CPP_FILES))
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm src/*.o

SFML:
	$(MAKE) $(MAKEFLAGS) -C ../dependencies/SFML-2.0

.PHONY: clean SFML
