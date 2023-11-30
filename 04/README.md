# Raycasting simples para testar performance de SIMD intrinsics

O código foi baseado no artigo [Ray Tracing In One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html).

## Compilando o projeto

Em sistemas windows, basta executar na linha de comando ou PowerShell

```ps
.\scripts\build.bat --config Debug
```

para compilar sem otimizações, e

```ps
.\scripts\build.bat --config Release
```

para compilar com otimizações. Note que é necessário ter as ferramentas de
build do Visual Studio com toolchain de C/C++ instalado, out então, o ambiente
ter acesso à ferramenta `cmake` através das variáveis de ambiente.

## Executando

```ps
# Sem otimização
.\build\Debug\CCanvas
# Com otimização
.\build\Release\CCanvas
```

quando a aplicação estiver executando, ao pressionar espaço a implementação Simd será utilizada.
