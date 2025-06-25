#  Flappy Bird Allegro
## Visão Geral do Projeto

Este projeto é uma recriação clássica do popular jogo Flappy Bird, desenvolvido em C++ utilizando a biblioteca gráfica Allegro 5. O objetivo principal é proporcionar uma experiência de jogo fiel ao original, servindo como um projeto prático para demonstrar o uso de Allegro para desenvolvimento de jogos 2D. Nesse sentido foi desenvovlido:

* **Gameplay Clássico:** Controles simples de um toque/clique para fazer o pássaro voar.
* **Geração de Obstáculos:** Canos que aparecem aleatoriamente e se movem em direção ao pássaro.
* **Detecção de Colisão:** Lógica para identificar colisões do pássaro com os canos ou o chão.
* **Sistema de Pontuação:** Registro da pontuação do jogador baseada nos canos ultrapassados.
* **Tela de Game Over:** Exibição da pontuação final ao perder o jogo.
* **Temas de jogo:** Exibição personalização de sprites, canos, som de fundo, piso e cenário.
* **Reinício de Jogo:** Opção para reiniciar a partida após o game over.

## Tecnologias Utilizadas

* **Linguagem:** C++
* **Biblioteca Gráfica:** Allegro 5
    * `allegro5/allegro.h`
    * `allegro5/allegro_image.h`
    * `allegro5/allegro_primitives.h`
    * `allegro5/allegro_font.h`
    * `allegro5/allegro_ttf.h`
    * `allegro5/allegro_audio.h`
    * `allegro5/allegro_acodec.h`
* **Sistema de Build:** GNU Make

# Como Instalar o WidgetZ
É importante ressaltar que o utilizamos O Visual Studio Code, como ambiente de programação,  juntamente com a extensão WSL, pois a partir do wsl é possível desenvolver em um ambiente baseado em Linux, usar ferramentas e utilitários específicos do Linux e executar e depurar seus aplicativos baseados em Linux a partir do conforto do Windows. Portanto, o tutorial abaixo é baseado nesse ambiente. No entanto, há um tutorial abaixo para configurar o wsl no vscode:

[Instalação do wsl2 no Windows 10 - Thiago Noronha](https://youtu.be/8-WZY4yIS54?si=WqlVA3qtRfcEkAXR)

### Por meio do terminal do Vscode
Para estilizar a parte grafica do jogo utilizamos a biblioteca WidgetZ que deve ser instalada, dentro da pasta pds2_projeto_final, para isso é necessário rodar o comando:
```bash
git clone https://github.com/SiegeLord/WidgetZ
```
Esse comando irá criar a pasta WidgetZ no repositorio, porém ainda é necessário entrar na pasta WidgetZ e rodar a sequeencia de comandos abaixo: 
```bash
cd WidgetZ
make mkdir build && cd build
cmake ..
make
sudo make install
```

## Como Compilar
Para compilar o projeto, no terminal, navegue até a pasta raiz do projeto e execute o seguinte comando:

```bash
make run
```

Esse comando irá compilar todos os arquivos necessário e em seguida irá roda o executável, que será gerado na pasta `bin/`. Caso queira limpar os arquivos gerados pela compilação, utilize:

```bash
make clean
```

## Como Rodar os Testes
Há testes unitarios que validam os métodos implementados. Para rodar esses teste é necessário fazer um de cada vez, para isso é preciso entrar na pasta de teste e executar o comando a seguir:
```bash
cd testes
make run_(nome-do-arquivo)
```
Caso esteja em uma subpasta, é preciso rodar o comando abaixo:
```bash
make run_(nome-da-pasta)_(nome-do-arquivo)
```
## Funcionalidades Principais


## Autor(es)

* [Abrãao de Oliveira Ferreira Couto] 
* [Atila Melo de Souza]
* [Davidson Santos de Oliveira]
* [Maria Vitória Leão Silva]
* [Yohana Sofia de Jesus Oliveira]

