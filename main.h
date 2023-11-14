#ifndef MAIN
#define MAIN

static inline int timems(void);
static inline int alivehere(int x, int y);
int countneighbors(int x, int y);
void tick(WINDOW *buf);
void randomise(void);
void resizecopy(void);
void clearstate(void);

#endif
