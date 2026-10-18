#include "treap.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

#define FILHO_ESQ 3
#define FILHO_DIR 4 

typedef struct no {
    struct no *esq;
    struct no *dir;

    int valor;
    int prio;
} No;

struct treap {
    No *raiz;
    int elementos;
};


Treap cria_treap () {
    Treap arv = malloc(sizeof(struct treap));
    if (arv == NULL) {
        printf("Alocação de árvore falhou\n");
        return NULL;
    }

    arv->raiz = NULL;
    arv->elementos = 0;

    return arv;
}


static No* cria_no (int valor) {
    No *no = malloc(sizeof(No));
    if (no == NULL) {
        printf("Alocação de nó falhou\n");
        return NULL;
    }

    no->esq = NULL;
    no->dir = NULL;
    no->valor = valor;
    no->prio = rand();
    return no;
}


static No* busca_no (No *no, int valor) {
    if (no == NULL) return NULL;
    if (no->valor == valor) return no;
    else if (no->valor > valor) return busca_no(no->esq, valor);
    else return busca_no(no->dir, valor);
}

bool busca_treap (Treap arv, int valor) {
    if (arv == NULL) return false;
    return (busca_no(arv->raiz, valor) != NULL);
}

static void rotacao_direita(No **raiz) {
    No *nova_raiz = (*raiz)->esq;
    (*raiz)->esq = nova_raiz->dir;
    nova_raiz->dir = *raiz;
    *raiz = nova_raiz;
}

static void rotacao_esquerda(No **raiz) {
    No *nova_raiz = (*raiz)->dir;
    (*raiz)->dir = nova_raiz->esq;
    nova_raiz->esq = *raiz;
    *raiz = nova_raiz;
}


static bool insere_no (No **no, int valor) {
    if (*no == NULL) { // Chegou na folha
        No *novo = cria_no(valor);
        if (novo == NULL) return false;

        *no = novo;
        return true;
    }
    
    if ((*no)->valor == valor) {
        printf("Não é permitido adicionar valores duplicados na árvore (%d).\n", valor);
        return false;
    }


    else if ((*no)->valor > valor) {
        bool rec = insere_no(&(*no)->esq, valor);
        if (rec) {
            if ((*no)->esq->prio > (*no)->prio) rotacao_direita(no);
        }
        return rec;
    }
    else {
        bool rec = insere_no(&(*no)->dir, valor);
        if (rec) {
            if ((*no)->dir->prio > (*no)->prio) rotacao_esquerda(no);
        }
        return rec;
    }
}


bool insere_treap (Treap arv, int valor) {
    if (arv == NULL) {
        printf("Tentativa de inserção em árvore inexistente!\n");
        return false;
    }

    if (!insere_no(&arv->raiz, valor)) return false;

    arv->elementos++;
    return true;
}


static int conta_filhos (No *no) {
    if (no == NULL) {
        printf("Tentativa de contar os filhos de nó NULL.\n");
        return -1;
    }

    if (no->esq != NULL && no->dir != NULL) return 2;
    if (no->dir != NULL) return FILHO_DIR;
    if (no->esq != NULL) return FILHO_ESQ;
    return 0;
}

static bool remove_no (No **no, int valor) {
    if (*no == NULL) return false;

    if ((*no)->valor == valor) {
        int filhos = conta_filhos(*no);
        if (filhos == 0) {
            free(*no);
            *no = NULL;
        }
        else if (filhos == FILHO_ESQ) {
            No *removido = *no;
            *no = (*no)->esq;
            free(removido);
        }
        else if (filhos == FILHO_DIR) {
            No *removido = *no;
            *no = (*no)->dir;
            free(removido);
        }
        else if (filhos == 2) {
            if ((*no)->esq->prio > (*no)->dir->prio) rotacao_direita(no);
            else rotacao_esquerda(no);
            return remove_no(no, valor);
        }
        else return false; // conta_filhos retornou -1, na prática não deve acontecer
        return true;
    }

    else if ((*no)->valor > valor) return remove_no(&(*no)->esq, valor);
    else return remove_no(&(*no)->dir, valor);
}

bool remove_treap (Treap arv, int valor) {
    if (arv == NULL) {
        printf("Tentativa de remoção em árvore inexistente!\n");
        return false;
    }

    if (!remove_no(&arv->raiz, valor)) {
        printf("Tentativa de remover nó que não está na árvore (%d).\n", valor);
        return false;
    }

    arv->elementos--;
    return true;
}


static void imprime_no (No* no, int altura, char *lado) {
    if (no == NULL) return;
    for (int i = 0; i < altura; i++) printf("\t");
    printf("%d (prio: %d) (%s)", no->valor, no->prio, lado);
    printf("\n");
    imprime_no(no->esq, altura + 1, "esq");
    imprime_no(no->dir, altura + 1, "dir");
}

void imprime (Treap arv) {
    if (arv == NULL) {
        printf("Tentativa de impressão de árvore inexistente!\n");
        return;
    }

    imprime_no(arv->raiz, 0, "raiz");
}

static void destroi_no (No *no) {
    if (no == NULL) return;
    destroi_no(no->esq);
    destroi_no(no->dir);
    free(no);
}

void destroi_treap (Treap arv) {
    if (arv == NULL) {
        printf("Tentativa de destruição de árvore inexistente!\n");
        return;
    }

    destroi_no(arv->raiz);
    free(arv);
}



/* ------- Impressão gráfica feita por IA (extra, útil para apresentação) ------- */
 
typedef struct {
    float espac_x;   // distância horizontal entre nós consecutivos (em ordem)
    float espac_y;   // distância vertical entre níveis
    float raio;
    float margem;
    ALLEGRO_FONT *fonte;
} Layout;
 
static int altura_no (No *no) {
    if (no == NULL) return 0;
    int e = altura_no(no->esq);
    int d = altura_no(no->dir);
    return 1 + (e > d ? e : d);
}
 
// Desenha texto centralizado em (cx, cy) com escala "s" (a fonte embutida é pequena)
static void desenha_texto (ALLEGRO_FONT *fonte, const char *txt, float cx, float cy,
                           float s, ALLEGRO_COLOR cor) {
    ALLEGRO_TRANSFORM t;
    al_identity_transform(&t);
    al_scale_transform(&t, s, s);
    al_translate_transform(&t, cx, cy);
    al_use_transform(&t);
    al_draw_text(fonte, cor, 0, -al_get_font_line_height(fonte) / 2.0f, ALLEGRO_ALIGN_CENTRE, txt);
    al_identity_transform(&t);
    al_use_transform(&t);
}
 
/* Percorre em ordem: a posição x de cada nó é seu índice na ordem crescente, o y é a
profundidade. Com arestas == true desenha só as linhas, senão desenha só os círculos
(assim nenhuma linha fica por cima de um círculo). Devolve em *x_out a posição do nó. */
static void desenha_no (No *no, int prof, int *cont, const Layout *l, bool arestas,
                        float *x_out, float *y_out) {
    float xe = 0, ye = 0, xd = 0, yd = 0;
 
    if (no->esq) desenha_no(no->esq, prof + 1, cont, l, arestas, &xe, &ye);
    float x = l->margem + (*cont)++ * l->espac_x;
    float y = l->margem + prof * l->espac_y;
    if (no->dir) desenha_no(no->dir, prof + 1, cont, l, arestas, &xd, &yd);
 
    if (arestas) {
        ALLEGRO_COLOR cor = al_map_rgb(90, 90, 90);
        if (no->esq) al_draw_line(x, y, xe, ye, cor, 2);
        if (no->dir) al_draw_line(x, y, xd, yd, cor, 2);
    }
    else {
        char buf[32];
        al_draw_filled_circle(x, y, l->raio, al_map_rgb(225, 238, 255));
        al_draw_circle(x, y, l->raio, al_map_rgb(40, 70, 140), 2);
 
        snprintf(buf, sizeof buf, "%d", no->valor);
        float s = l->raio / 10.0f;
        float larg = al_get_text_width(l->fonte, buf) * s;
        if (larg > 1.7f * l->raio) s *= 1.7f * l->raio / larg;
        desenha_texto(l->fonte, buf, x, y, s, al_map_rgb(20, 20, 20));
 
        // Prioridade embaixo do círculo, com fundo branco para não se misturar às arestas
        snprintf(buf, sizeof buf, "p:%d", no->prio);
        float pw = al_get_text_width(l->fonte, buf) / 2.0f + 2;
        float py = y + l->raio + 9;
        al_draw_filled_rectangle(x - pw, py - 6, x + pw, py + 6, al_map_rgb(255, 255, 255));
        desenha_texto(l->fonte, buf, x, py, 1.0f, al_map_rgb(150, 60, 60));
    }
 
    *x_out = x;
    *y_out = y;
}
 
static void desenha_arvore (No *raiz, const Layout *l) {
    float x, y;
    int cont = 0;
    al_clear_to_color(al_map_rgb(255, 255, 255));
    desenha_no(raiz, 0, &cont, l, true, &x, &y);
    cont = 0;
    desenha_no(raiz, 0, &cont, l, false, &x, &y);
    al_flip_display();
}
 
void imprime_ia (Treap arv) {
    if (arv == NULL) {
        printf("Tentativa de impressão de árvore inexistente!\n");
        return;
    }
    if (arv->raiz == NULL) {
        printf("Árvore vazia, nada para desenhar.\n");
        return;
    }
 
    if (!al_init() || !al_init_primitives_addon() || !al_init_font_addon() || !al_install_keyboard()) {
        printf("Falha ao inicializar o Allegro\n");
        return;
    }
 
    // Dimensiona a janela de acordo com o tamanho da árvore
    Layout l;
    l.margem = 50;
    l.espac_x = 50;
    l.espac_y = 80;
    if (arv->elementos * l.espac_x + 2 * l.margem > 1600) {
        l.espac_x = (1600 - 2 * l.margem) / arv->elementos;
        if (l.espac_x < 24) l.espac_x = 24;
    }
    int niveis = altura_no(arv->raiz);
    if (niveis > 1 && (niveis - 1) * l.espac_y + 2 * l.margem + 20 > 900) {
        l.espac_y = (900 - 2 * l.margem - 20) / (niveis - 1);
        if (l.espac_y < 45) l.espac_y = 45;
    }
    l.raio = l.espac_x * 0.45f;
    if (l.raio > 22) l.raio = 22;
 
    int larg = (int)((arv->elementos - 1) * l.espac_x + 2 * l.margem);
    int alt = (int)((altura_no(arv->raiz) - 1) * l.espac_y + 2 * l.margem + 20);
    if (larg < 400) larg = 400;
    if (alt < 300) alt = 300;
 
    ALLEGRO_DISPLAY *disp = al_create_display(larg, alt);
    if (disp == NULL) {
        printf("Falha ao criar a janela\n");
        return;
    }
    al_set_window_title(disp, "Treap");
 
    l.fonte = al_create_builtin_font();
    ALLEGRO_EVENT_QUEUE *fila = al_create_event_queue();
    if (l.fonte == NULL || fila == NULL) {
        printf("Falha ao criar fonte/fila de eventos\n");
        if (l.fonte) al_destroy_font(l.fonte);
        if (fila) al_destroy_event_queue(fila);
        al_destroy_display(disp);
        return;
    }
    al_register_event_source(fila, al_get_display_event_source(disp));
    al_register_event_source(fila, al_get_keyboard_event_source());
 
    desenha_arvore(arv->raiz, &l);
 
    // Mantém a janela aberta até fechar ou apertar ESC
    bool fim = false;
    while (!fim) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(fila, &ev);
        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) fim = true;
        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) fim = true;
        else if (ev.type == ALLEGRO_EVENT_DISPLAY_EXPOSE) desenha_arvore(arv->raiz, &l);
    }
 
    al_destroy_event_queue(fila);
    al_destroy_font(l.fonte);
    al_destroy_display(disp);
}