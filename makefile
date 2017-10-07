SRC=src/solar_tracker.c
HEX=`pwd`/build/solar_tracker.hex

CC=sdcc
FAMILY=pic14
PROC=16f690
OUT=build/
RUN=pk2cmd
VDD=4
PROGCMD=pk2cmd -PPIC$(PROC)  


all: $(SRC:.c=.hex)

$(SRC:.c=.hex): $(SRC)
	mkdir -p $(OUT)
	$(CC) --use-non-free -m$(FAMILY) -p$(PROC) -o $(OUT) $^

clean:
	rm -f $(SRC:.c=.asm) $(SRC:.c=.cod) $(SRC:.c=.hex) $(SRC:.c=.lst) $(SRC:.c=.o)

.PHONY: all clean


run: 
	$(PROGCMD) -A$(VDD) -T -R

program: all
	$(PROGCMD) -M -F$(HEX) -Y

