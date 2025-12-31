# Zynta

Uma simples linguagem de programação feita em C

## Features
Atualmente só analisa com lexer

- Comentarios com "#"
- Identificadores, inteiros, floats, strings, operadores simples e compostos

## Dependencias
- build-essential
- make
- gcc/clang

## Build

simples
```bash
git clone https://github.com/Lemiossa/Zynta.git
cd Zynta
make release # ou debug
```

O binário estará em: build/<release | debug>/bin/potato

## Uso

```bash
./zynta [comandos]
```

Comandos:
- -r/--run-file <ARQUIVO> -> Roda um arquivo de texto em potato
- -v/--version -> Exibe a versão do programa
- -h/--help -> Exibe a mensagem de ajuda

## Licença
MIT
