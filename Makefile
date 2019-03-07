NAME = libaudio.a

CXX = g++

override CXXFLAGS += -std=c++14 -g -Wall -Wextra -O3 -pipe

AR = ar

ARFLAGS = rc

RANLIB = ranlib

RANLIBFLAGS =

INCLUDES_PATH = -I src
INCLUDES_PATH+= -I ../lib/portaudio/include
INCLUDES_PATH+= -I ../lib/libvorbis/include
INCLUDES_PATH+= -I ../lib/libogg/include

SRCS_PATH = src/

SRCS_NAME = loadDatas.cpp \
	    Stream.cpp \
	    Player.cpp \
	    DirectPlayer.cpp \
	    CachedPlayer.cpp \
	    Filters/Filter.cpp \
	    Filters/EchoFilter.cpp \
	    Filters/BilinearTransformFilter.cpp \
	    Filters/BandPassFilter.cpp \
	    Filters/LowPassFilter.cpp \
	    Filters/HighPassFilter.cpp \

SRCS = $(addprefix $(SRCS_PATH), $(SRCS_NAME))

OBJS_PATH = obj/

OBJS_NAME = $(SRCS_NAME:.cpp=.opp)

OBJS = $(addprefix $(OBJS_PATH), $(OBJS_NAME))

all: odir $(NAME)

$(NAME): $(OBJS)
	@echo "AR $(NAME)"
	@$(AR) $(ARFLAGS) $(NAME) $(OBJS)
	@echo "RANLIB $(NAME)"
	@$(RANLIB) $(RANLIBFLAGS) $(NAME)

$(OBJS_PATH)%.opp: $(SRCS_PATH)%.cpp
	@echo "CXX $<"
	@$(CXX) $(CXXFLAGS) -o $@ -c $< $(INCLUDES_PATH)

odir:
	@mkdir -p $(OBJS_PATH)
	@mkdir -p $(OBJS_PATH)Filters

clean:
	@rm -f $(OBJS)
	@rm -f $(NAME)

re: clean all

.PHONY: all clean re odir
