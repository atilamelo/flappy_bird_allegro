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
É importante ressaltar que o utilizamos o Visual Studio Code, como ambiente de programação,  juntamente com a extensão WSL, pois a partir do wsl é possível desenvolver em um ambiente baseado em Linux, usar ferramentas e utilitários específicos do Linux e executar e depurar seus aplicativos baseados em Linux a partir do conforto do Windows. Portanto, o tutorial abaixo é baseado nesse ambiente. No entanto, há um tutorial abaixo para configurar o wsl no vscode:

[Instalação do wsl2 no Windows 10 - Thiago Noronha](https://youtu.be/8-WZY4yIS54?si=WqlVA3qtRfcEkAXR)

### Por meio do terminal do Vscode
Para estilizar a parte grafica do jogo utilizamos a biblioteca WidgetZ que deve ser instalada, dentro da pasta pds2_projeto_final, para isso é necessário rodar o comando:
```bash
git clone https://github.com/SiegeLord/WidgetZ
```
Esse comando irá criar a pasta WidgetZ no repositório, porém ainda é necessário entrar na pasta WidgetZ e rodar a sequeencia de comandos abaixo: 
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

* **Gameplay Clássico Flappy Bird:** A mecânica central do jogo foi implementada, incluindo o movimento do pássaro com física de gravidade, pulos controlados pelo jogador via teclado, e a geração randômica de canos como obstáculos. A colisão com os canos ou com o chão ou teto resulta no fim da partida.

* **Sistema de Cenas:** O fluxo do jogo é controlado por um `SceneManager` que gerencia múltiplas cenas, incluindo um Menu Inicial (`StartMenu`), uma tela de Seleção de Personagens (`CharacterSelectionScene`) e a cena principal do jogo (`GameScene`).

* **Arquitetura Orientada a Objetos:** O projeto foi construído sobre uma base sólida de POO, conforme os critérios de avaliação. Foi criada uma hierarquia de classes com uma classe base abstrata (`GameObject`) para os elementos do jogo [cite_start]e interfaces desacopladas como `IDrawable` e `IUpdatable` para definir comportamentos, permitindo alta flexibilidade e extensibilidade.

* **Sistema de Pontuação e Persistência:** Foi implementado um `ScoreSystem` no padrão Singleton para gerenciar o cadastro de jogadores por apelido único e suas estatísticas. O sistema salva os dados em um arquivo .csv, garantindo que as melhores pontuações persistam entre as sessões de jogo, e é capaz de indicar o jogador com a maior pontuação ou um ranking personalizado pegando os top N jogadores.

### Funcionalidades Extras e Destaques

Além disso, diversas funcionalidades extras foram implementadas:

* **✨ Sistema de Temas Dinâmicos (Sprites)**
    O projeto vai além de um único cenário. A tela de seleção de personagens permite que o jogador escolha um "Tema", que altera dinamicamente múltiplos **sprites** e aspectos do jogo. Cada tema possui:
    * Um personagem com animações próprias.
    * Um plano de fundo (`background`) exclusivo.
    * Uma base de chão (`floor`) correspondente.
    * Sprites de canos (`pipe`) customizados.

* **🎵 Música e Efeitos Sonoros**
    Foi implementado um gerenciador de áudio (`GameSound`).
    * **Música de Fundo por Tema:** Cada tema selecionado pode ter sua própria trilha sonora.
    * **Efeitos Sonoros:** Efeitos para ações como pular, pontuar e morrer foram adicionados para dar feedback instantâneo ao jogador.
    * **Botão de Controle:** Um botão de som na tela permite ao jogador ligar ou desligar a música a qualquer momento.

* **🚀 Otimizações de Performance e Memória**
    A arquitetura foi pensada para ser eficiente e segura.
    * **Gerenciador de Recursos (`ResourceManager`):** Implementado como um Singleton, ele centraliza o carregamento de todos os assets (imagens, sons). Isso evita o carregamento duplicado de recursos e gerencia a memória automaticamente com `std::unique_ptr`, prevenindo vazamentos.
    * **Pool de Objetos (`PipePool`):** Em vez de criar e destruir canos constantemente (uma operação custosa), o sistema reutiliza um "pool" de objetos `PipePair`. Isso reduz drasticamente a alocação e desalocação de memória durante o gameplay, resultando em uma performance mais suave e sem "engasgos".

* **🌟 UI Avançada e Animações de Transição**
    Para uma experiência de usuário mais polida, diversas animações de UI foram criadas:
    * **Logo Animado:** No menu, o logo do jogo possui uma animação de flutuação contínua.
    * **Transições de Cena Suaves:** O `SceneManager` realiza um efeito de "fade" para preto ao transitar entre cenas.
    * **Feedback de Impacto:** Ao colidir, uma tela branca (`SplashScreen`) pisca rapidamente para dar uma sensação de impacto.
    * **Sequência de Fim de Jogo Animada:** Após a morte, o bitmap "Game Over" e o painel de placar (`ScoreBoard`) deslizam para a tela em estágios, em vez de aparecerem instantaneamente.

* **🛡️ Programação Defensiva e Validação**
    O sistema utiliza técnicas de programação defensiva, como o tratamento de exceções, além de garantir que os recursos fundamentais sejam garantidamente iniciados antes do jogo iniciar, caso contrário, ocorre um erro de execução.
    * **Validação de Input:** A caixa de texto no menu inicial possui validação para permitir apenas caracteres válidos (letras, números e espaço) e um comprimento específico para o nome do jogador.
    * **Exceções Customizadas:** Classes como `NameException` e `ScoreException` foram criadas para um tratamento de erros mais específico e claro no sistema de pontuação.

## Autor(es)

* [Abraão de Oliveira Ferreira Couto] 
* [Atila Melo de Souza]
* [Davidson Santos de Oliveira]
* [Maria Vitória Leão Silva]
* [Yohana Sofia de Jesus Oliveira]

