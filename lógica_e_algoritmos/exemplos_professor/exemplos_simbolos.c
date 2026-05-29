#include <stdio.h>
#include <wchar.h>
#include <locale.h>

#define QTD_SIMBOLOS 36

int main() {
    setlocale(LC_ALL, "");

    int codigos[QTD_SIMBOLOS] = {
        9787,   // ☻ personagem principal
        9785,   // ☹ inimigo simples
        9760,   // ☠ caveira / perigo / monstro

        9829,   // ♥ vida
        9830,   // ♦ tesouro / item especial
        9679,   // ● moeda / ponto / item coletável
        9733,   // ★ bônus / estrela
        9889,   // ⚡ energia / bateria
        9673,   // ◉ escudo / proteção
        10010,  // ✚ cura / hospital / vida extra
        9919,   // ⚿ chave

        9635,   // ▣ saída / portal / base
        9636,   // ▤ porta / passagem
        9637,   // ▥ porta trancada / grade
        9639,   // ▧ porta especial / obstáculo
        8962,   // ⌂ casa / base

        9632,   // ■ bloco / obstáculo
        9633,   // □ bloco vazio / caixa
        9638,   // ▦ obstáculo diferente
        9618,   // ▒ área bloqueada / terreno
        9619,   // ▓ parede densa / área perigosa

        9650,   // ▲ seta para cima / armadilha
        9660,   // ▼ seta para baixo / armadilha
        9658,   // ► seta para direita / direção
        9668,   // ◄ seta para esquerda / direção
        9675,   // ○ item vazio / marcador
        9678,   // ◎ alvo / portal / item especial
        10006,  // ✖ dano / erro / perigo

        9553,   // ║ parede vertical
        9552,   // ═ parede horizontal
        9556,   // ╔ canto superior esquerdo
        9559,   // ╗ canto superior direito
        9562,   // ╚ canto inferior esquerdo
        9565,   // ╝ canto inferior direito

        9617,   // ░ sombra leve / área especial
        9608    // █ bloco cheio / parede cheia
    };

    char descricoes[QTD_SIMBOLOS][60] = {
        "personagem principal",
        "inimigo simples",
        "caveira / perigo / monstro",

        "vida",
        "tesouro / item especial",
        "moeda / ponto / item coletavel",
        "bonus / estrela",
        "energia / bateria",
        "escudo / protecao",
        "cura / hospital / vida extra",
        "chave",

        "saida / portal / base",
        "porta / passagem",
        "porta trancada / grade",
        "porta especial / obstaculo",
        "casa / base",

        "bloco / obstaculo",
        "bloco vazio / caixa",
        "obstaculo diferente",
        "area bloqueada / terreno",
        "parede densa / area perigosa",

        "seta para cima / armadilha",
        "seta para baixo / armadilha",
        "seta para direita / direcao",
        "seta para esquerda / direcao",
        "item vazio / marcador",
        "alvo / portal / item especial",
        "dano / erro / perigo",

        "parede vertical",
        "parede horizontal",
        "canto superior esquerdo",
        "canto superior direito",
        "canto inferior esquerdo",
        "canto inferior direito",

        "sombra leve / area especial",
        "bloco cheio / parede cheia"
    };

    int i;

    wprintf(L"=== TESTE DE SIMBOLOS PARA O JOGO ===\n\n");
    wprintf(L"Simbolo | Codigo | Uso sugerido\n");
    wprintf(L"-------------------------------\n");

    for(i = 0; i < QTD_SIMBOLOS; i++) {
        wprintf(L"   %lc    | %5d  | %s\n", codigos[i], codigos[i], descricoes[i]);
    }

    return 0;
}