# Potato

Uma simples linguagem de programação feita em C

## Features
Atualmente só analisa com lexer

- Comentarios com "#"
- Identificadores, inteiros e strings

## Build

simples
```bash
git clone https://github.com/Lemiossa/Potato.git
cd Potato
make release # ou debug
```

O binário estará em: build/<release | debug>/bin/potato

## Uso

```bash
./potato [comandos]
```

Comandos:
- -r/--run-file <ARQUIVO> -> Roda um arquivo de texto em potato
- -v/--version -> Exibe a versão do programa
- -h/--help -> Exibe a mensagem de ajuda

## Licença
MIT
