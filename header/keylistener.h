#ifndef TENGINE_KEYLISTENER_H
#define TENGINE_KEYLISTENER_H

#include <termios.h>
#include "queue.h"
#include "keymap.h"
#include "drawer.h"

/* Defines a KeyListener, which is used to listen for key press events and translate them into values to be placed in
 *  a shared Queue
 * int fd: STDIN file descriptor
 * struct termios oldterm, newterm: structs with values describing terminal settings for terminal before and after
 *  KeyListener initialization
 * int oldflags: terminal flags before KeyListener initialization
 * Queue *eQueue: shared Queue
 * KeyMap *rec_keycodes: KeyMap mapping key presses to integer values
 * pthread **drawer_thread_id: Double pointer initialized by init_drawer function
 */
typedef struct {
    int fd;
    struct termios oldterm, newterm;
    int oldflags;
    Queue *eQueue;
    KeyMap *rec_keycodes;
    Drawer *drawer;
} KeyListener;

// KeyListener operations
KeyListener* init_keylistener(Queue *queue, Drawer *drawer);
void delete_keylistener(KeyListener *key_listener);
void keylistener_add_key(KeyListener *key_listener, const char key[KEYSIZE], int val);
void keylistener_handle_in(KeyListener *key_listener);
void keylistener_exit(KeyListener *key_listener);

/* Defines a struct containing the corresponding key values for each key
 * Using CONST.[KEY], keys can easily be placed in a KeyMap with their desired value
 */
extern const struct KEYCONST {
    const char K_ESC[KEYSIZE];
    const char K_F1[KEYSIZE];
    const char K_F2[KEYSIZE];
    const char K_F3[KEYSIZE];
    const char K_F4[KEYSIZE];
    const char K_F5[KEYSIZE];
    const char K_F6[KEYSIZE];
    const char K_F7[KEYSIZE];
    const char K_F8[KEYSIZE];
    const char K_F9[KEYSIZE];
    const char K_F10[KEYSIZE];
    const char K_F11[KEYSIZE];
    const char K_F12[KEYSIZE];
    const char K_GRAVE_ACCENT[KEYSIZE];
    const char K_1[KEYSIZE];
    const char K_2[KEYSIZE];
    const char K_3[KEYSIZE];
    const char K_4[KEYSIZE];
    const char K_5[KEYSIZE];
    const char K_6[KEYSIZE];
    const char K_7[KEYSIZE];
    const char K_8[KEYSIZE];
    const char K_9[KEYSIZE];
    const char K_0[KEYSIZE];
    const char K_TILDE[KEYSIZE];
    const char K_EXCLAMATION_MARK[KEYSIZE];
    const char K_AT_SIGN[KEYSIZE];
    const char K_HASH[KEYSIZE];
    const char K_DOLLAR_SIGN[KEYSIZE];
    const char K_PERCENTAGE_SIGN[KEYSIZE];
    const char K_CIRCUMFLEX[KEYSIZE];
    const char K_AMPERSAND[KEYSIZE];
    const char K_ASTERISK[KEYSIZE];
    const char K_OPARENTHESIS[KEYSIZE];
    const char K_CPARENTHESIS[KEYSIZE];
    const char K_DASH[KEYSIZE];
    const char K_UNDERSCORE[KEYSIZE];
    const char K_EQ_SIGN[KEYSIZE];
    const char K_PLUS_SIGN[KEYSIZE];
    const char K_BACKSPACE[KEYSIZE];
    const char K_TAB[KEYSIZE];
    const char K_Q[KEYSIZE];
    const char K_q[KEYSIZE];
    const char K_W[KEYSIZE];
    const char K_w[KEYSIZE];
    const char K_E[KEYSIZE];
    const char K_e[KEYSIZE];
    const char K_R[KEYSIZE];
    const char K_r[KEYSIZE];
    const char K_T[KEYSIZE];
    const char K_t[KEYSIZE];
    const char K_Y[KEYSIZE];
    const char K_y[KEYSIZE];
    const char K_U[KEYSIZE];
    const char K_u[KEYSIZE];
    const char K_I[KEYSIZE];
    const char K_i[KEYSIZE];
    const char K_O[KEYSIZE];
    const char K_o[KEYSIZE];
    const char K_P[KEYSIZE];
    const char K_p[KEYSIZE];
    const char K_OBRACKET[KEYSIZE];
    const char K_OBRACE[KEYSIZE];
    const char K_CBRACKET[KEYSIZE];
    const char K_CBRACE[KEYSIZE];
    const char K_BAR[KEYSIZE];
    const char K_BACKSLASH[KEYSIZE];
    const char K_A[KEYSIZE];
    const char K_a[KEYSIZE];
    const char K_S[KEYSIZE];
    const char K_s[KEYSIZE];
    const char K_D[KEYSIZE];
    const char K_d[KEYSIZE];
    const char K_F[KEYSIZE];
    const char K_f[KEYSIZE];
    const char K_G[KEYSIZE];
    const char K_g[KEYSIZE];
    const char K_H[KEYSIZE];
    const char K_h[KEYSIZE];
    const char K_J[KEYSIZE];
    const char K_j[KEYSIZE];
    const char K_K[KEYSIZE];
    const char K_k[KEYSIZE];
    const char K_L[KEYSIZE];
    const char K_l[KEYSIZE];
    const char K_SEMICOLON[KEYSIZE];
    const char K_COLON[KEYSIZE];
    const char K_QUOTE[KEYSIZE];
    const char K_DQUOTE[KEYSIZE];
    const char K_RETURN[KEYSIZE];
    const char K_Z[KEYSIZE];
    const char K_z[KEYSIZE];
    const char K_X[KEYSIZE];
    const char K_x[KEYSIZE];
    const char K_C[KEYSIZE];
    const char K_c[KEYSIZE];
    const char K_V[KEYSIZE];
    const char K_v[KEYSIZE];
    const char K_B[KEYSIZE];
    const char K_b[KEYSIZE];
    const char K_N[KEYSIZE];
    const char K_n[KEYSIZE];
    const char K_M[KEYSIZE];
    const char K_m[KEYSIZE];
    const char K_COMMA[KEYSIZE];
    const char K_LT_SIGN[KEYSIZE];
    const char K_DOT[KEYSIZE];
    const char K_GT_SIGN[KEYSIZE];
    const char K_SLASH[KEYSIZE];
    const char K_QUESTION_MARK[KEYSIZE];
    const char K_SPACEBAR[KEYSIZE];
    const char K_INSERT[KEYSIZE];
    const char K_HOME[KEYSIZE];
    const char K_PGUP[KEYSIZE];
    const char K_DEL[KEYSIZE];
    const char K_END[KEYSIZE];
    const char K_PGDN[KEYSIZE];
    const char K_UARROW[KEYSIZE];
    const char K_LARROW[KEYSIZE];
    const char K_DARROW[KEYSIZE];
    const char K_RARROW[KEYSIZE];
} CONST;

#endif //TENGINE_KEYLISTENER_H
