CC=gcc
# -DNO_ALARM -DNO_CSRF -O0
override CFLAGS += -O3 -g -m32 -fno-omit-frame-pointer -std=gnu89 -D_FORTIFY_SOURCE=2 -Wformat -Wno-implicit-function-declaration -Wno-format-security -Wl,-z,relro -Wl,-z,now -fstack-protector-strong -fsanitize=address -Wno-deprecated-declarations -Wno-int-conversion

ODIR=./src
TDIR=./templates
SRCDIR=./src
IDIR=./src
CGI_DIR=./cgi-bin

LIBS=-lm

all: poolcide

$(TDIR)/%.templ: $(TDIR)/%.templ.html
	./to_templ.py $<

$(ODIR)/%.o: $(SRCDIR)/%.c templates
	$(CC) -c -o $@ $< $(CFLAGS) -I$(TDIR)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ cgi-bin/poolcide $(TDIR)/*.templ

templates: $(TDIR)/towel_details.templ $(TDIR)/towel.templ $(TDIR)/error.templ $(TDIR)/body_index.templ $(TDIR)/head.templ $(TDIR)/towel_dispenser.templ $(TDIR)/user_not_found.templ

poolcide: $(ODIR)/poolcide.o
	$(CC) -o $(CGI_DIR)/$@ $^ $(CFLAGS) $(LIBS)

run: poolcide
	./poolcgid.py

test_rand:
	make clean
	make poolcide 'CFLAGS="-DTEST_RAND"'
	./cgi-bin/poolcide
	make clean
test_query_parser:
	make clean
	make poolcide 'CFLAGS="-DTEST_QUERY_PARSER"'
	echo "test=success" | ./cgi-bin/poolcide
	make clean
test_alpha:
	make clean
	make poolcide 'CFLAGS="-DTEST_ALPHA" -w'
	./cgi-bin/poolcide
	make clean
test_val:
	make clean
	make poolcide 'CFLAGS="-DTEST_VAL" -w'
	./cgi-bin/poolcide
	make clean
test_readline:
	make clean
	make poolcide 'CFLAGS="-DTEST_READLINE" -w'
	echo "line" | ./cgi-bin/poolcide
	make clean
test_hash:
	make clean
	make poolcide 'CFLAGS="-DTEST_HASH" -w'
	./cgi-bin/poolcide
	make clean
test_ini_files:
	make clean
	make poolcide 'CFLAGS="-DTEST_INI_FILES" -w'
	./cgi-bin/poolcide
	make clean
test_escape:
	make clean
	make poolcide 'CFLAGS="-DTEST_ESCAPE" -w'
	./cgi-bin/poolcide
	make clean
test_cookie_parser:
	make clean
	make poolcide 'CFLAGS="-DTEST_COOKIE_PARSER" -w'
	./cgi-bin/poolcide
	make clean
test_towel_enc:
	make clean
	make poolcide 'CFLAGS="-DTEST_TOWEL_ENC" -w'
	./cgi-bin/poolcide
	make clean

tests: test_towel_enc test_cookie_parser test_escape test_ini_files test_rand test_query_parser test_alpha test_val test_readline test_hash
	@echo ""
	@echo "[*] All tests done"
	
format:
	./.custom-format.py -i $(SRCDIR)/*.c
