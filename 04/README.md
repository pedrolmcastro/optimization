# Raycasting simples para testar performance de SIMD intrinsics

O código foi baseado no artigo [Ray Tracing In One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html).

## Compilando o projeto

### Windows (Win32)

Em sistemas windows, basta executar na linha de comando ou PowerShell

```ps
.\scripts\build.bat Debug
```

para compilar sem otimizações, e

```ps
.\scripts\build.bat Release
```

para compilar com otimizações. Note que é necessário ter as ferramentas de
build do Visual Studio com toolchain de C/C++ instalado, out então, o ambiente
ter acesso à ferramenta `cmake` através das variáveis de ambiente.

### Linux (X11)

Para compilar, basta entrar em uma shell

```sh
./scripts/build.sh Debug
```

para compilar sem otimizações, e

```ps
./scripts/build.sh Release
```

para compilar com otimizações. Note que é necessário ter as o `cmake` instalado e um
compilador C moderno.

## Executando

Quando a aplicação estiver executando, ao pressionar espaço a implementação Simd será utilizada.

### Windows

```ps
# Sem otimização
.\build\Debug\CCanvas
# Com otimização
.\build\Release\CCanvas
```

### Linux

No linux as builds de debug e release ficam no mesmo local, portanto ao executar a cofiguração
utilizada para compilar que foi usada por último, será utilizada.

```ps
./build/CCanvas
```
