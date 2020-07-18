#define INI_LEN_MAX (256)
#define DELIM ('=')
#define KV_SPLIT ('&')
#define KV_START ('?')
#define COOKIE_NAME "poolcode"
#define NL "\r\n"
#define QUERY_COUNT (32)
#define NONCE_LEN (20)
#define ADMIN_CHALLENGE_LEN (16)
#define TOWEL_TOKEN_LEN (8)
#define COOKIE_LEN (10)
#define PRUNE_TIME "15"

#define STORAGE_DIR "../../data/"
#define COOKIE_DIR STORAGE_DIR "cookies/"
#define DATA_DIR STORAGE_DIR "data/"
#define USER_DIR STORAGE_DIR "users/"
#define TOWEL_DIR STORAGE_DIR "towels/"
#define PRIORITY_TOWEL_DIR STORAGE_DIR "priority_towels/"

/* Why include if you can copy&paste? */
#define O_WRONLY 01
#define O_CREAT 0100                                         /* Not fcntl.  */
#define O_EXCL 0200                                          /* Not fcntl.  */
#define S_IWUSR 0200                                    /* Write by owner.  */
#define S_IRUSR 0400                                     /* Read by owner.  */

#define _NULL ((void *)0)
#define FILE void

extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;

#define BODY() readline(0)
#define FILE_NEXT() readline(file)

#define COOKIE_LOC (((state_t *)state)->cookie_loc)
#define USER_LOC (((state_t *)state)->user_loc)
#define NONCE (((state_t *)state)->nonce)

#define PFATAL(x)                                 \
  do {                                            \
                                                  \
    fprintf(stderr, "Line %d FATAL: ", __LINE__); \
    perror(x);                                    \
    fflush(stdout);                               \
    fflush(stderr);                               \
    abort();                                      \
                                                  \
  } while (0);

static runid;
#define LOG(x...)                       \
  do {                                  \
                                        \
    fprintf(stderr, "run %d: ", runid); \
    fprintf(stderr, x);                 \
    fflush(stderr);                     \
                                        \
  } while (0);

#define KV_FOREACH(kv, block)              \
  do {                                     \
                                           \
    int    idx = 0;                        \
    int    key_idx = 0;                    \
    int    val_idx = 1;                    \
    char **cur = (kv);                     \
    char * key, *val;                      \
    while (cur[key_idx] && cur[val_idx]) { \
                                           \
      key = cur[key_idx];                  \
      val = cur[val_idx];                  \
      {block};                             \
      idx++;                               \
      key_idx += 2;                        \
      val_idx += 2;                        \
                                           \
    }                                      \
                                           \
  } while (0);

#define FILE_KV_FOREACH(filename, block)                \
  do {                                                  \
                                                        \
    FILE *file = fopen(filename, "r");                  \
    if (!file) {                                        \
                                                        \
      LOG("Error accessing KV file %s...\n", filename); \
      PFATAL("Couldn't open kv file");                  \
                                                        \
    }                                                   \
    do {                                                \
                                                        \
      char **query = parse_query(FILE_NEXT());          \
      if (!query) { break; }                            \
      KV_FOREACH(query, {block});                       \
                                                        \
    } while (1);                                        \
                                                        \
                                                        \
    fclose(file);                                       \
                                                        \
  } while (0);

#define ROUTE(method_name, route_name)        \
  if (!strcmp(state->method, #method_name) && \
      !strcmp(state->route, #route_name)) {   \
                                              \
    handle_##route_name(state);               \
    handled = 1;                              \
                                              \
  }

#define IS_GET (!strcmp(state->method, "GET"))
#define IS_POST (!strcmp(state->method, "POST"))

/* 0-9A-Za-z */
#define IS_ALPHANUMERIC(c)                                   \
  (((c) >= '0' && (c) <= '9') || ((c) >= 'A' && c <= 'Z') || \
   ((c) >= 'a' && (c) <= 'z'))

/* Templating in C is eas-C */
#define TEMPLATE(x) #x

char empty_list[2][2] = {0};

typedef struct state {

  char *cookie;
  char *nonce;

  char *username;

  char *user_loc;
  char *cookie_loc;

  char *method;
  char *route;

  int logged_in;

  char **queries[QUERY_COUNT];

} state_t;

/* Sane code STARTS with main. Why would anybody read from bottom to top? */
int main() {

  runid = *((int *)rand_str(4));

/* run tests using
   make CFLAGS='-DTEST_RAND'
*/
#if defined(TEST_RAND)

  printf("testing strlen of rand_str(16)\n");
  assert(strlen(rand_str(16)) == 16);
  printf("testing alphanumericity of rand_str(16)\n");
  char *rand = rand_str(1);
  assert(IS_ALPHANUMERIC(rand[0]));
  printf("%s\n", rand_str(16));
  return 0;

#elif defined(TEST_COOKIE_PARSER)

  parse_cookie("cookie");
  printf(parse_cookie(COOKIE_NAME "=testcookie;"));
  assert(!strcmp(parse_cookie(COOKIE_NAME "=testcookie;"), "testcookie"));
  assert(!strcmp(parse_cookie(COOKIE_NAME "=testcookie; "), "testcookie"));
  assert(
      !strcmp(parse_cookie("test=fun; HTTPOnly; " COOKIE_NAME "=testcookie; "),
              "testcookie"));
  return 0;

#elif defined(TEST_QUERY_PARSER)

  int   i;
  char *parseme = "pool=side&fun=true&you're=beautiful!&&fun=";
  printf("parsing %s\n", parseme);
  char **query = parse_query(parseme);
  KV_FOREACH(query, { printf("key: %s, val: %s\n", key, val); })
  assert(parseme[1] == query[0][1]);
  return 0;

#elif defined(TEST_ALPHA)

  char *alpha = "FUN1";
  char *nonalpha1 = "%%!FUN1";
  char *nonalpha2 = "%%!";
  char *nonalpha3 = "%%!0";
  char *alpha1 = dup_alphanumeric(nonalpha1);
  printf("%s: %s", nonalpha1, alpha1);
  assert(!strcmp(alpha, alpha1));
  free(alpha1);
  return 0;

#elif defined(TEST_VAL)

  system("mkdir -p " COOKIE_DIR);
  system("mkdir -p " USER_DIR);
  state_t *state = init_state(_NULL, _NULL, _NULL);
  char *   testval = get_val(state, "test");
  if (!testval) {

    printf("No val read!");
    return 0;

  }

  printf("%s\n", testval);
  printf("%s\n", get_val(state, "test"));
  return 0;

#elif defined(TEST_READLINE)

  char *body = BODY();
  if (body) {

    printf("%s\n" NL, body);

  } else {

    printf("No body read\n");

  }

  return 0;

#elif defined(TEST_HASH)

  printf("%s" NL, hash("test"));
  return 0;

#elif defined(TEST_INI_FILES)

  char *testfile = "testfile.tmp";
  char *key = "testkey";
  char *val = "testval";
  char *val2 = "testval2";
  file_delete(testfile);
  FILE *f = file_create_atomic(testfile);
  fclose(f);
  file_set_val(testfile, key, val);

  char *read_val = file_get_val(testfile, key, "");
  assert(!strcmp(val, read_val));

  file_set_val(testfile, "some", "value");
  file_set_val(testfile, key, val2);
  file_set_val(testfile, "someother", "value");
  system("cat testfile.tmp");
  read_val = file_get_val(testfile, key, "");
  assert(strcmp(val, read_val));
  assert(!strcmp(val2, read_val));

  file_delete(testfile);

  printf("testing empty ini. Query should be empty:\n");
  fflush(stdout);
  FILE *file = file_create_atomic(testfile);
  assert(file);
  debug_print_query(read_ini(testfile));
  assert(!file_get_val(testfile, "test", _NULL));
  remove(testfile);

  return 0;

#elif defined(TEST_ESCAPE)

  printf("AA 4html = %s\n", escape_4_html("AA"));
  assert(!strcmp(escape_4_html("AA"), "&#x41;&#x41;"));
  printf("AA 4py = %s\n", escape_4_py("AA"));
  assert(!strcmp(escape_4_py("AA"), "\\x41\\x41"));
  return 0;

#elif defined(TEST_TOWEL_ENC)

  printf(enc_admin_challenge("test"));
  assert(strlen(enc_admin_challenge("test")));
  return 0;

#else                                                            /* No TEST */

  /* THE ACTUAL MAIN */

  // clang-format off
  #ifndef NO_ALARM
  alarm(15);
  #endif
  
  https://www.openroad.org/cgi-bin/cgienvdemo

  LOG("Welcome to poolcide.\n");

  char *current_cookies = getenv("HTTP_COOKIE");
  char *request_method = getenv("REQUEST_METHOD");
  char *query_string = getenv("QUERY_STRING");
  /* Webserver name to this binary */
  char *script_name = getenv("SCRIPT_NAME");

  char **cookie_kv = parse_query(current_cookies);

  char *cookie = "";
  // clang-format on
  KV_FOREACH(cookie_kv, {

    if (!strcmp(key, COOKIE_NAME)) {

      int i;
      cookie = val;
      for (i = 0; cookie[i]; i++) {

        if (cookie[i] == ';' || cookie[i] == ' ') {

          cookie[i] = '\0';
          break;

        }

      }

      LOG("Got cookie %s\n", cookie);

    }

  });

  state_t *state = init_state(request_method, cookie, query_string);

  write_headers(state);

  /* AJAX State of mind */
  if IS_GET { write_head(state); }

  LOG("REQUEST_METHOD='%s' HTTP_COOKIE='poolcode=%s' QUERY_STRING='%s' .%s\n",
      state->method, state->cookie, query_string, script_name);

  int handled = 0;

  ROUTE(GET, index);
  ROUTE(POST, login);
  ROUTE(POST, register);

  ROUTE(GET, dispense);
  ROUTE(GET, reserve);
  ROUTE(POST, reserve);

  ROUTE(GET, towel);
  ROUTE(POST, towel);
  ROUTE(PUT, towel);

  if (!handled) {

    LOG("Unknown route!\n");
    char *error = "Unsupported route or method!";
    printf(
  #include <error.templ>
    );

  }

  if IS_GET { printf(NL "</html>" NL); }

  fflush(stdin);
  LOG("Finished %s %s - %s %s \n", state->method, state->route, query_string,
      script_name);

  return 0;

#endif

}

int assert(condition) {

  if (!condition) {

    LOG("Assert failed :/\n");
    fflush(stdout);
    trigger_gc(1);
    exit(1);

  }

}

/* Frees all memory we no longer need */
int trigger_gc(code) {

  exit(code);

}

/* The os will free our memory. */
const char *__asan_default_options() {

  /* The os will free our memory. */
  return "detect_leaks=0";

}

char *escape(char *replace, char *str) {

  int i;
  int len = strlen(str);
  int written = 0;
  /* all the right values for all the wrong reasons */
  int   replace_len = ((strlen(replace) + 4));
  char *ret = calloc(1, len * replace_len);
  /* LOG("len %d, replen %d, rep %s, str %s, ptr %p\n", len, replace_len,
   * replace, str, ret); */
  for (i = 0; i < len; i++) {

    /* LOG("%d %c %s", written, str[i], ret); */
    written += sprintf(ret + written, replace, (unsigned char)str[i]);

  }

  /* LOG("\n"); */

  return ret;

}

/* char * */
#define E4(name, replace)          \
  int escape_4_##name(char *str) { \
                                   \
    return escape(replace, str);   \
                                   \
  }

E4(py, "\\x%2x")
E4(html, "&#x%2x;")
E4(hash, "%2x")

/* FILE *(char *) */
int file_create_atomic(filename) {

  int fd = open(filename, O_CREAT | O_WRONLY | O_EXCL, S_IRUSR | S_IWUSR);
  if (fd < 0) {

    perror(filename);
    return _NULL;

  }

  fprintf(stderr, "Created file: %s\n", filename);

  return fdopen(fd, "w");

}

/* A random char in 0-9A-Za-z */
char get_rand_alphanumberic() {

  char ret = 0;
  if (!getrandom(&ret, 1, 0)) { PFATAL("Couldn't get random"); }
  if (IS_ALPHANUMERIC(ret)) { return ret; }
  return get_rand_alphanumberic();

}

/* A new string with only alphanumeric chars.
   The others are stripped. */
/* char * */
int dup_alphanumeric(char *str) {

  int i;
  int retpos = 0;
  if (!str) { return _NULL; }
  char *ret = calloc(1, 1024);
  for (i = 0; str[i]; i++) {

    if (IS_ALPHANUMERIC(str[i])) { ret[retpos++] = str[i]; }

  }

  return ret;

}

/* returns a random string with the given length */
/* char * */
int rand_str(len) {

  int   i;
  char *ret = calloc(2, len);
  if (!ret) { PFATAL("calloc") };
  for (i = 0; i < len; i++) {

    ret[i] = get_rand_alphanumberic();

  }

  return ret;

}

/* char **(char *) */
int parse_query(str) {

  int i;

  if (!str) { return _NULL; }
  int content_len = strlen(str);
  if (!content_len) { return empty_list; }

  char **ret = calloc(1, (content_len)*4 + 8);
  char * contents = strdup(str);
  int    parsing_key = 1;
  int    current_len = 0;
  ret[0] = contents;
  int val_count = 0;

  /* Strip tailing newline */
  if (contents[content_len - 1] == '\n') {

    content_len = content_len - 1;
    contents[content_len] = '\0';

  }

  /* parse */
  for (i = 0; i < content_len; i++) {

    if (!contents[i]) {

      ret[++val_count] = "";
      return ret;

    } else if ((contents[i] == (parsing_key ? DELIM : KV_SPLIT)) &&

               current_len) {

      contents[i] = 0;
      ret[++val_count] = &contents[i + 1];
      parsing_key = !parsing_key;
      current_len = 0;

    } else {

      current_len++;

    }

  }

  return ret;

}

/* char ** */
int read_ini(char *filename) {

  int    i;
  int    ini_pos = 0;
  int    key_exists;
  char **ini = calloc(1, 256);
  char * keys[128] = {0};
  int    linec = 0;

  FILE_KV_FOREACH(filename, {

    key_exists = 0;
    for (i = 0; keys[i]; i++) {

      if (!strcmp(key, keys[i])) { key_exists = 1; }

    }

    if (!key_exists) {

      ini[ini_pos++] = key;
      ini[ini_pos++] = val;

    }

  });

  return ini;

}

void write_ini(filename, ini) {

  int   i;
  int   key_exists;
  char *keys[128] = {0};
  int   linec = 0;
  FILE *file = fopen(filename, "w");
  if (!file) { PFATAL("Couldn't open ini file"); }
  KV_FOREACH(ini, {

    key_exists = 0;
    for (i = 0; keys[i]; i++) {

      if (!strcmp(key, keys[i])) { key_exists = 1; }

    }

    if (!key_exists) {

      LOG("Outputting %s=%s\n", key, val);

      if (fprintf(file, "%s=%s\n", key, val) < 0) {

        perror("Writing ini");
        trigger_gc(1);
        exit(1);

      }

      keys[i] = key;

    }

  });

  fclose(file);

}

void debug_print_query(query) {

  fprintf(stderr, "---> Query:\n");
  KV_FOREACH(query, { fprintf(stderr, "%s=%s\n", key, val); });
  fprintf(stderr, "<--- EOQ\n");
  fflush(stderr);

}

#define LOC(name, DIR)                  \
  char *loc_##name(char *locname) {     \
                                        \
    char *loc = calloc(1, 1032);        \
    sprintf(loc, "%s%s", DIR, locname); \
    return loc;                         \
                                        \
  }

LOC(cookie, COOKIE_DIR)
LOC(user, USER_DIR)

int file_set_val(filename, key_to_write, val_to_write) {

  LOG("Setting %s to %s\n", key_to_write, val_to_write);
  char *keycpy = strdup(key_to_write);
  char *valcpy = strdup(val_to_write);

  char **ini = read_ini(filename);

  int wrote_val = 0;
  int last_idx = -1;

  KV_FOREACH(ini, {

    if (!strcmp(key, keycpy)) {

      ini[val_idx] = valcpy;
      wrote_val = 1;

    }

    last_idx = val_idx;

  });

  if (!wrote_val) {

    ini[last_idx + 1] = keycpy;
    ini[last_idx + 2] = valcpy;

  } else {

    free(keycpy);

  }

  write_ini(filename, ini);
  return 0;

}

/* char * */
int get_val(state_t *state, char *key_to_find) {

  int i;

  for (i = 0;; i++) {

    /*LOG("p: %d %p\n"NL, i, state->queries[i]);*/
    if (!state->queries[i]) { state->queries[i] = parse_query(BODY()); }

    if (!state->queries[i]) { return _NULL; }
    /*LOG("query: %s\n"NL, state->queries[i]);*/
    KV_FOREACH(state->queries[i], {

      /*LOG("tofind: %s - %s %s\n", key_to_find, key, val);*/
      if (!strcmp(key, key_to_find)) {

        int len = strlen(val);
        if (val[len - 1] <= ' ') { val[len - 1] = '\0'; }
        return val;

      }

    });

  }

  dprintf(2, "Getval without return should never be reached\n");
  assert(0);

}

/* char * */
int file_get_val(filename, key_to_find, default_val) {

  char **ini = read_ini(filename);
  KV_FOREACH(ini, {

    if (!strcmp(key, key_to_find)) { return val; }

  });

  return default_val;

}

int write_headers(state_t *state) {

  write_palmtree();
  printf(
      "Content-Security-Policy: script-src 'nonce-%s'; style-src 'nonce-%s'"
      " https://fonts.googleapis.com/css2?family=Lobster&display=swap;" NL
      "X-Frame-Options: SAMEORIGIN" NL "X-Xss-Protection: 1; mode=block" NL
      "X-Content-Type-Options: nosniff" NL
      "Referrer-Policy: no-referrer-when-downgrade" NL
      "Feature-Policy "
      "geolocation 'self'; midi 'self'; sync-xhr 'self'; microphone 'self'; "
      "camera 'self'; magnetometer 'self'; gyroscope 'self'; speaker 'self'; "
      "fullscreen *; payment 'self';" NL "Content-Type: text/html" NL

      "Cache-Control: no-store" NL "Set-Cookie: " COOKIE_NAME
      "=%s; HttpOnly" NL NL,
      NONCE, NONCE, ((state_t *)state)->cookie);
  return 0;

}

int write_palmtree() {

  printf("Palm-0: |       __ _.--..--._ _" NL
         "Palm-1: |    .-' _/   _/\\_   \\_'-." NL
         "Palm-2: |   |__ / 🦜 _/\\__/\\_   \\__|" NL
         "Palm-3: |      |___/\\_\\__/  \\___|" NL
         "Palm-4: |             \\__/     " NL
         "Palm-5: |              \\__/ " NL
         "Palm-6: |               \\__/        |>" NL
         "Palm-7: |       __🥥____.~\\__/~.____|" NL
         "Palm-8: |     /  ENO                 \\" NL
         "Palm-9: |~~~~~~  ~~~~~ ~~!~~  ~~~ 🌊🌊~ ~~~" NL);

}

int write_head(state_t *state) {

  char *palm = head_palm();
  printf(
#include <head.templ>
  );

  return 0;

}

int head_palm() {

  return "<meta name=palm content=\"\n "
         "......................................__\n "
         ".............................,-~*`¯lllllll`*~,\n "
         ".......................,-~*`lllllllllllllllllllllllllll¯`*-,\n "
         "..................,-~*llllllllllllllllllllllllllllllllllllllllllll*-,"
         "\n "
         "...............,-*llllllllllllllllllllllllllllllllllllllllllllllllll."
         "\\\n "
         ".............;*`lllllllllllllllllllllllllll,-~*~-,"
         "llllllllllllllllllll\\\n "
         "..............\\llllllllllllllllllllllll/"
         ".........\\ENOlllllllllll,-`~-,\n "
         "...............\\lllllllllllllllllllll,-*...........`~-~-,...(.(¯`*,`"
         ",\n "
         "................\\llllllllllll,-~*.....................)_-\\..*`*;..)"
         "\n .................\\,-*`¯,*`)............,-~*`~................/\n "
         "..................|/.../.../~,......-~*,-~*`;................/.\\\n "
         "................./.../.../.../"
         "..,-,..*~,.`*~*................*...\\\n "
         "................|.../.../.../"
         ".*`...\\...........................)....)¯`~,\n "
         "................|./.../..../.......)......,.)`*~-,............/"
         "....|..)...`~-,\n "
         "..............././.../...,*`-,.....`-,...*`....,---......\\..../"
         "...../..|.........¯```*~-,,,,\n "
         "...............(..........)`*~-,....`*`.,-~*.,-*......|.../..../.../"
         "............\\........\n "
         "................*-,.......`*-,...`~,..``.,,,-*..........|.,*...,*...|"
         "..............\\........\n "
         "...................*,.........`-,...)-,..............,-*`...,-*....(`"
         "-,............\\.......\n "
         "......................f`-,.........`-,/"
         "...*-,___,,-~*....,-*......|...`-,..........\\........\n \">";

}

/* state_t * */
int init_state(request_method, current_cookie, query_string) {

  int      i;
  state_t *state = calloc(sizeof(state_t), 1);

  if (!request_method || !((char *)request_method)[0]) {

    LOG("No request method provided. Assuming GET\n");
    state->method = "GET";

  } else {

    state->method = request_method;

  }

  char *new_cookie = dup_alphanumeric(current_cookie);
  if (new_cookie && !new_cookie[0]) {

    free(new_cookie);
    new_cookie = _NULL;

  }

  if (!new_cookie) {

    /* A new browser, welcome! :) */
    new_cookie = rand_str(COOKIE_LEN);

  }

  state->cookie = new_cookie;
  state->cookie_loc = loc_cookie(state->cookie);

  FILE *file = file_create_atomic(state->cookie_loc);
  if (file) {

    fprintf(stderr, "Existing cookie %s\n", state->cookie);
    fclose(file);

  }

  state->logged_in = !strcmp(cookie_get_val(state, "logged_in", "0"), "1");
  LOG("User is%s logged in.\n", state->logged_in ? "" : " not");

  if (state->logged_in) {

    state->username = cookie_get_val(state, "username", _NULL);

  }

  if (state->username) {

    LOG("User %s is back!\n", state->username);
    state->user_loc = loc_user(state->username);

  } else {

    state->username = "New User";

  }

  state->nonce = rand_str(NONCE_LEN);

  maybe_prune(state, COOKIE_DIR);
  maybe_prune(state, USER_DIR);

  state->route = "index";

  if (query_string) {

    state->queries[0] = parse_query(query_string);
    KV_FOREACH(state->queries[0], {

      if (!strcmp(key, "route")) { state->route = val; }

    })

    if (state->route[0]) { LOG("Route: %s\n", state->route); }

  } else {

    query_string = "";

  }

  return state;

}

/* char * */
int parse_cookie(cookies) {

  int i;

  if (!cookies) { return _NULL; }
  int content_len = strlen(cookies);
  if (!content_len) { return _NULL; }

  char *contents = strdup(cookies);
  int   parsing_key = 1;
  int   current_len = 0;
  char *current_key = contents;
  char *current_val = _NULL;
  int   val_count = 0;

  /* Strip tailing newline */
  if (contents[content_len - 1] == '\n') {

    content_len = content_len - 1;
    contents[content_len] = '\0';

  }

  for (i = 0; i < content_len; i++) {

    if (contents[i] == ';') {

      contents[i] = '\0';
      if (parsing_key) {

        current_key = contents + i + 1;
        current_val = 0;

      } else {

        if (!strcmp(current_key, COOKIE_NAME)) {

          return dup_alphanumeric(current_val);

        }

        parsing_key = 1;
        current_key = contents + i + 1;
        current_val = _NULL;

      }

    } else if (parsing_key && contents[i] == ' ') {

      current_key = contents + i + 1;

    } else if (parsing_key && (contents[i] == DELIM || contents[i] == ' ')) {

      contents[i] = '\0';
      parsing_key = 0;
      current_val = contents + i + 1;

    }

  }

  if (!strcmp(current_key, COOKIE_NAME)) {

    return dup_alphanumeric(current_val);

  }

  return _NULL;

}

int csrf_new(state) {

  char *csrf = rand_str(8);
  cookie_set_val(state, "csrf", csrf);
  return csrf;

}

int csrf_validate(state) {

#ifdef NO_CSRF
  return 1;
#endif

  char *csrf_sent = get_val(state, "csrf");
  char *csrf_stored = cookie_get_val(state, "csrf", _NULL);
  if (!csrf_stored) {

    LOG("INTERNAL_ERROR: No valid csrf token could be found for cookie %s!\n",
        ((state_t *)state)->cookie);
    abort();
    return 0;

  }

  if (!csrf_stored[0]) {

    LOG("Tried to write data without requestion CSRF token first\n");
    return 0;

  }

  cookie_set_val(state, "csrf", "");
  if (strcmp(csrf_sent, csrf_stored)) {

    LOG("CSRF Validation failed, expected %s (len %d) but got %s (len %d)\n",
        csrf_stored, strlen(csrf_stored), csrf_sent, strlen(csrf_sent));
    return 0;

  }

  return 1;

}

int handle_index(state_t *state) {

  /*int cf = cookie_file(cookie);*/
  /*read_ini(USER_DIR + username);*/

  char *username = state->username;
  int   logged_in = state->logged_in;

  char *csrf = csrf_new(state);

  printf(
#include "body_index.templ"
  );

  return 0;

}

char **split(char *str, char splitter) {

  int i;

  if (!str) { return empty_list; }
  int len = strlen(str);
  if (!len) { return empty_list; }
  char **ret = calloc(sizeof(char *), len / 2);
  int    pos = 0;
  ret[pos] = str;
#define CURRENT_ITEM ret[pos]
  for (i = 0; str[i]; i++) {

    if (str[i] == splitter) {

      str[i] = '\0';
      if (strlen(ret[pos])) {

        /* LOG("Found element:: %s\n", CURRENT_ITEM); */
        pos++;

      }

      CURRENT_ITEM = str + i + 1;

    }

  }

#undef CURRENT_ITEM
  if (!strlen(ret[pos])) {

    ret[pos] = _NULL;
    pos--;

  }

  LOG("Split item count for %s: %d\n", str, pos);
  return ret;

}

/* reads a line */
/* char *(FILE *) */
int readline(f) {

  char buf[100001];
  if (!(!f ? gets(buf) : fgets(buf, sizeof(buf), f))) {

    /* Looks like EOF to me */
    return _NULL;

  }

  char *ret = malloc(strlen(buf) + 2);
  strcpy(ret, buf);
  return ret;

}

char **own_towel_list(state) {

  int i;

  char *own_towels = get_user_val(state, "towels", "");
  LOG("User towels: %s\n", own_towels);
  return split(own_towels, '/');

}

int ls(state, dir) {

  int i;

  /* prune all (26 + 26 + 10) requests */
  maybe_prune(state, dir);
  /* using forward slash as divider = never a valid unix filename */
  char *list_str = run("ls -t '%s' | tr '\\n' '/' | head -c 99999", dir);
  return split(list_str, '/');

}

int maybe_prune(state, dir) {

  static prune_offset = 0;

  if (((state_t *)state)->nonce[0] == 'A' + (prune_offset++ % 20)) {

    LOG("Pruning %s this time (every (26+26+10)th time).\n", dir);
    prune(dir);

  }

}

int prune(dir) {

  LOG("Pruning all files in %s older than " PRUNE_TIME " minutes\n", dir);
  /* mmin -> motification time, amin -> access time */
  LOG(run("find '%s' -mmin +" PRUNE_TIME " -type f -delete", dir));

}

int render_own_towels(state) {

  char **towel_list = own_towel_list(state);
  if (!towel_list || !towel_list[0]) { return ""; }
  return render_towel_template(state, towel_list, 0);

}

int render_all_towels(state) {

  char **towel_list = ls(state, TOWEL_DIR);
  if (!towel_list || !towel_list[0]) { return ""; }
  return render_towel_template(state, towel_list, 1);

}

int render_towel_template(state, towel_list, highlight_priority_towels) {

  int    i;
  char **priority_towels = _NULL;
  if (highlight_priority_towels) {

    priority_towels = ls(state, PRIORITY_TOWEL_DIR);

  }

  char *ret = calloc(1, 1024000);
  int   retpos = 0;

#define CURRENT_TOWEL (((char **)towel_list)[i])
  for (i = 0; CURRENT_TOWEL; i++) {

    int   k;
    int   priority_towel = 0;
    char *priority_towel_admin = "";
    char *towel_name = CURRENT_TOWEL;
    int   towel_len = strlen(CURRENT_TOWEL);
    for (k = 0; priority_towels && priority_towels[k]; k++) {

      /*  LOG("Priority towel %s\n", priority_towels[k]); */

      if (!strncmp(towel_name, priority_towels[k], towel_len)) {

        priority_towel = 1;
        /* +1 because we used an underscore as divider ([token]_[name]) */
        priority_towel_admin = priority_towels[k] + towel_len + 1;
        if (!priority_towel_admin) { priority_towel_admin = ""; }
        /* LOG("Towel %s is a priority towel, owner: %s\n", towel_name,
            priority_towel_admin); */
        break;

      }

    }

    /* LOG("Current towelname: %s, ret: %s @%p-%p\n", towel_name, ret, ret, ret
     * + retpos); */

    retpos += sprintf(ret + retpos,
#include <towel.templ>
    );

    if (retpos > 1000000) break;

  }

#undef CURRENT_TOWEL

  return ret;

}

int cookie_get_val(state, key, default_val) {

  return file_get_val(COOKIE_LOC, key, default_val);

}

int cookie_set_val(state, key, val) {

  file_set_val(COOKIE_LOC, key, val);

}

void file_delete(filename) {

  remove(filename);

}

/* returns 0 on error / if :user exists */
int user_create(name, pass) {

  // clang-format off
  char *user_loc = loc_user(name);
  FILE *file = file_create_atomic(user_loc);

  https://stackoverflow.com/questions/230062/whats-the-best-way-to-check-if-a-file-exists-in-c

  if (!file) {

    /* failure */
    /*if (errno == EEXIST) {*/
    /* the file probably already existed */
    perror("Could not create user entry.");
    return 0;
    /*}*/

  } else {

    char *pass_hash = hash(pass);
    fprintf(file, "name=%s\npass_hash=%s\n", name, pass_hash);

  }

  fclose(file);
  return 1;
  // clang-format on

}

int handle_register(state_t *state) {

  if (!csrf_validate(state)) {

    trigger_gc(1);
    goto invalid_username;

  }

  char *username = dup_alphanumeric(get_val(state, "username"));
  if (!strlen(username)) { goto invalid_username; }
  cookie_set_val(state, "username", username);
  if (!user_create(username, get_val(state, "password"))) {

    goto invalid_username;

  }

  state->username = username;
  state->user_loc = loc_user(state->username);
  cookie_set_val(state, "logged_in", "1");
  cookie_set_val(state, "csrf", "");
  printf("success");
  return 0;
invalid_username:
  printf("<h1>Sorry, username taken!</h1>");
  trigger_gc(1);
  exit(1);
  return -1;

}

/* char * */
int get_user_val(state, key, default_val) {

  return file_get_val(USER_LOC, key, default_val);

}

/* char * */
int set_user_val(state, key, val) {

  return file_set_val(USER_LOC, key, val);

}

int cookie_remove(state) {

  LOG("Removing cookie");
  file_delete(((state_t *)state)->cookie_loc);

}

int handle_login(state_t *state) {

  if (!csrf_validate(state)) {

    trigger_gc(1);
    goto user_not_found;

  }

  LOG("Logging in...\n");
  cookie_set_val(state, "logged_in", "0");
  char *username = dup_alphanumeric(get_val(state, "username"));
  LOG("Login started for user %s\n", username);
  if (strlen(username) < 1) { goto user_not_found; }
  state->user_loc = loc_user(username);
  cookie_set_val(state, "username", username);
  char *login_pw_hash = hash(get_val(state, "password"));
  LOG("Password hash: %s\n", login_pw_hash);
  char *stored_pw_hash = get_user_val(state, "pass_hash", _NULL);
  if (!stored_pw_hash) { goto user_not_found; }
  if (!strcmp(login_pw_hash, stored_pw_hash)) {

    LOG("Login successful for user %s", username);
    cookie_set_val(state, "logged_in", "1");
    printf("success");
    return 0;

  }

user_not_found:
  LOG("Login failed for user %s. Expected pw hash %s (len %d) but got (len %d)",
      username, stored_pw_hash, strlen(stored_pw_hash), strlen(login_pw_hash));
  printf(
#include "user_not_found.templ"
  );

error:
  cookie_remove(state);
  trigger_gc(1);
  exit(1);

}

/* char * */
int run(cmd, param) {

  int  i;
  char command[1024];

  sprintf(command, cmd, param);

  LOG("Running %s\n", command);

  FILE *fp = popen(command, "r");
  if (fp == _NULL) {

    perror("run command");
    trigger_gc(1);
    exit(1);

  }

  char *ret = readline(fp);
  pclose(fp);

  if (!ret) {

    LOG("No Return\n");
    ret = "";

  } else {

    // clang-format off
    int pos = strlen(ret);

    if (pos) {

      /* strip newline endings, pos downto 0 */

      while (pos --> 0) {

        if (ret[pos] != '\n') { break; }
        ret[pos] = '\0';

      }

    }

    /* LOG("Return was %s\n", ret); */
    // clang-format on

  }

  return ret;

}

/* char *(char *) */
int hash(to_hash) {

  return run("echo '%s' | sha256sum", escape_4_hash(to_hash));

}

int enc_admin_challenge(admin_challenge) {

  return run(
      "echo '%s'"
      "| ./age -r "
      "age1mngxnym3sz9t8jtyfsl43szh4pg070g857khq6zpw3h9l37v3gdqs2nrlx -a"
      "| tr -d '\\n'",
      admin_challenge);

}

int handle_reserve(state_t *state) {

  if (!csrf_validate(state)) {

    char *error = "I C you cannot SuRF. - CSRF validation failed.";
    printf(
#include <error.templ>
    );
    return 0;

  }

  char *csrf = "";

  char *admin_challenge = rand_str(ADMIN_CHALLENGE_LEN);
  char *towel_token = rand_str(TOWEL_TOKEN_LEN);
  LOG("admin_challenge was: %s\n", admin_challenge);
  LOG("towel_token was: %s\n", towel_token);
  char *color = get_val(state, "color");

  char towel_space[1036];
  sprintf(towel_space, TOWEL_DIR "%s", dup_alphanumeric(towel_token));

  FILE *file = file_create_atomic(towel_space);
  if (!file) {

    perror(towel_space);
    char *error = "Sorry, towel dispensing failed. :(";
    printf(
#include <error.templ>
    );
    return 0;

  }

  fprintf(file, "%s", color);
  fclose(file);

  char *user_towels_old = get_user_val(state, "towels", "");
  char *user_towels_new =
      calloc(1, strlen(user_towels_old) + ADMIN_CHALLENGE_LEN + 2);
  /* The towels list gets separated with slashes for serialization. */
  sprintf(user_towels_new, "%s/%s", user_towels_old, towel_token);
  set_user_val(state, "towels", user_towels_new);

  char *admin_challenge_enc = "";
  if (IS_POST || state->nonce[0] < '9') {

    admin_challenge_enc = enc_admin_challenge(admin_challenge);

  }

  char *own_towels = render_own_towels(state);
  char *towels = render_all_towels(state);

  printf(
#include <towel_dispenser.templ>
  );
  fflush(stdout);

  char *towel_admin_id = "";
  if IS_POST {

    char *towel_admin_id = get_val(state, "towel_admin_id");

    int id_len = strlen(towel_admin_id);
    if (!id_len) {

      LOG("Empty towel admin response received. In case you expected an admin "
          "to "
          "access this towel, there may be a proxy messing up adminness.\n");
      return -1;

    }

    if (!strcmp(admin_challenge, towel_admin_id)) {

      LOG("An admin entered the scene!\n");
      add_priority_towel_for(state->username, towel_token);
      printf("Admin at the pool!\n");

    }

  }

}

int handle_dispense(state) {

  char *admin_challenge = rand_str(ADMIN_CHALLENGE_LEN);
  char *towel_token = "";
  LOG("admin_challenge was: %s\n", admin_challenge);
  LOG("towel_token was: %s\n", towel_token);
  char *color = "";

  char *admin_challenge_enc = "";
  char *own_towels = render_own_towels(state);
  char *towels = render_all_towels(state);
  if (!towels) { towels = ""; }

  char *csrf = csrf_new(state);

  printf(
#include <towel_dispenser.templ>
  );

}

int add_priority_towel_for(username, towel_token) {

  char priority_towel_space[1036];
  sprintf(priority_towel_space, PRIORITY_TOWEL_DIR "%s_%s",
          dup_alphanumeric(towel_token), username);

  FILE *file = file_create_atomic(priority_towel_space);
  if (!file) {

    perror(priority_towel_space);

  } else {

    fclose(file);

  }

}

int handle_towel(state_t *state) {

  int    i;
  char **towels = own_towel_list(state);
  char * towel = get_val(state, "token");
  for (i = 0; towels[i]; i++) {

    if (!strcmp(towel, towels[i])) {

      char *username = state->username;
      char *color = escape_4_html(get_towel_color(towel));
      printf(
#include <towel_details.templ>
      );
      return 0;

    }

  }

  LOG("User %s does not posess towel %s\n", state->username, towel);
  char *error =
      "Don't steal somebody elses towel, please. We're on holidays!.\n";
  printf(
#include <error.templ>
  );

}

int get_towel_color(towel) {

  char  towelpath[1036];
  char *color = calloc(1, 4096);
  sprintf(towelpath, TOWEL_DIR "%s", towel);
  LOG("Reading color from towel at %s\n", towel);
  FILE *file = fopen(towelpath, "r");
  fread(color, 1, 4096, file);
  fclose(file);
  return color;

}

