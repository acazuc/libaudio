NAME = libaudio.a

CC = g++ -std=c++14

ARCH = -m64

AR = gcc-ar

ARFLAGS =

RANLIB = gcc-ranlib

RANLIBFLAGS =

CLFAGS = -g -Wall -Wextra -O3 -pipe -mtune=generic -fuse-linker-plugin -flto

INCLUDES_PATH = -I src
INCLUDES_PATH+= -I ../lib/portaudio/include
INCLUDES_PATH+= -I ../lib/libvorbis/include
INCLUDES_PATH+= -I ../lib/libogg/include

SRCS_PATH = src/

SRCS_NAME = AudioPlayer.cpp \
		BufferedAudioPlayer.cpp \
		CachedAudioPlayer.cpp \
		loadDatas.cpp \

SRCS = $(addprefix $(SRCS_PATH), $(SRCS_NAME))

OBJS_PATH = obj/

OBJS_NAME = $(SRCS_NAME:.cpp=.opp)

OBJS = $(addprefix $(OBJS_PATH), $(OBJS_NAME))

all: odir $(NAME)

$(NAME): $(OBJS)
	@echo " - Making $(NAME)"
	@$(AR) -rc $(ARFLAGS) $(NAME) $(OBJS)
	@$(RANLIB) $(RANLIBFLAGS) $(NAME)

$(OBJS_PATH)%.opp: $(SRCS_PATH)%.cpp
	@echo " - Compiling $<"
	@$(CC) $(ARCH) $(CFLAGS) -o $@ -c $< $(INCLUDES_PATH)

odir:
	@mkdir -p $(OBJS_PATH)

clean:
	@echo " - Cleaning objs"
	@rm -f $(OBJS)

fclean: clean
	@echo " - Cleaning lib"
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
