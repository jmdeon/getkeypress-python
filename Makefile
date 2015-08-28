CC := gcc
CFLAGS := --inplace
BIN_NAME := getkeypress.so
SRC := getkeypress.c key_util.c getdevicefile.c config.c
SETUP := setup.py

all: $(BIN_NAME)

$(BIN_NAME): $(SRC)
	python $(SETUP) build_ext $(CFLAGS)

clean:
	rm -f *.so $(BIN_NAME)
