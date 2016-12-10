CXX=g++
CXXFLAGS=-O2 -std=c++11 -Wall -Wextra
LIBS=
INCLUDES=

#==========================
SRCDIR  = .
SRCS    = $(wildcard $(SRCDIR)/*.cpp)
OBJS    = $(addprefix ./, $(notdir $(SRCS:.cpp=.exe)))

all: $(OBJS)

#==========================

$(SRCDIR)/%.exe: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $+

#==========================

clean :
		@rm -f ./*.exe ./*.stackdump

