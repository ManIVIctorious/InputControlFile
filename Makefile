
CC = gcc

  CFLAGS += -g#                     # Enable debug symbols
  CFLAGS += -Og#                    # Set optimisation level, should be g if debug symbols are enabled
  CFLAGS += -march=native#          # Tune for current chipset, don't bother about backwards compatibility
 #CFLAGS += -mtune=native#          # Tune for current chipset, remain backwards compatible
  CFLAGS += -Wall#                  # Enable base set of warnings
  CFLAGS += -Wextra#                # Enable additional warnings
  CFLAGS += -Werror#                # Treat warnings as errors

EXEDIR = $(if ${MyLocalPath}, ${MyLocalPath}, bin)
EXE = $(EXEDIR)/ControlFileInput

OBJ += main.o
OBJ += GetSettingsControlFile.o
OBJ += ParseControlFile.o
OBJ += OutputSettings.o

all: $(EXE)

# Build object files out of C-source files
%.o : %.c Makefile
	$(CC) $(CFLAGS) $(PPF) -c $<

# link all objects to create the executable
$(EXE): $(OBJ)
	$(CC) $(CFLAGS) $(LIB) $(OBJ) -o $@


clean:
	rm -f $(OBJ) $(EXE)
