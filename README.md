# Trabalho Final de PDS2 - Flappy Bird 
## Resumo do Projeto

Este projeto é uma versão do jogo Flappy Bird, onde o jogador controla um pássaro que deve desviar de obstáculos (canos) para alcançar a maior pontuação possível. O objetivo é passar por entre os canos sem colidir, mantendo o pássaro no ar ao pressionar a tecla espaço. 

## Requisitos do Sistema


# Como Instalar o WidgetZ
É importante ressaltar que o utilizamos O Visual Studio Code, como ambiente de programação,  juntamente com a extensão WSL, pois a partir do wsl é possível desenvolver em um ambiente baseado em Linux, usar ferramentas e utilitários específicos do Linux e executar e depurar seus aplicativos baseados em Linux a partir do conforto do Windows. Portanto, o tutorial abaixo é baseado nesse ambiente. No entanto, há um tutorial abaixo para configurar o wsl no vscode:

[Instalação do wsl2 no Windows 10 - Thiago Noronha](https://youtu.be/8-WZY4yIS54?si=WqlVA3qtRfcEkAXR)

## Por meio do terminal do Vscode
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

# Como Compilar
Para compilar o projeto, no terminal, navegue até a pasta raiz do projeto e execute o seguinte comando:

```bash
make run
```

Esse comando irá compilar todos os arquivos necessário e em seguida irá roda o executável, que será gerado na pasta `bin/`. Caso queira limpar os arquivos gerados pela compilação, utilize:

```bash
make clean
```

##Estrutura do Projeto:
## Como Rodar os Testes
## Funcionalidades Principais


## Autor(es)

* [Abrãao de Oliveira Ferreira Couto] 
* [Atila Melo de Souza]
* [Davidson Santos de Oliveira]
* [Maria Vitória Leão Silva]
* [Yohana Sofia de Jesus Oliveira]

#
