# 42 Labs 3º Edição

Este é desafio técnico do processo seletivo da 3º Edição do 42 Labs.

## Desafio

O Objetivo desse desafio é criar um programa `monitoring` em C que monitore serviços web utilizando 3 protocolos: *HTTP*, *PING* e *DNS* utilizando as configurações presentes no arquivo `monitoring.db`.

### monitoring.db

O arquivo `monitoring.db` define como a aplicação vai agir. Esse arquivo tem uma estrutura estrita e deve ser rejeitado caso não corresponda a essa estrutura.

A estrutura é definida por linhas e colunas, onde cada linha terá as configurações separadas por um TAB, que definem as colunas. Para cada monitoramento, as configurações até a 3º coluna serão as mesmas, já a partir da 4º a configuração é específica, baseando-se no protocolo escolhido na 2º coluna.

As configurações para cada protocolo são:

| Protocolo   | Configurações                                                           |
|-------------|-------------------------------------------------------------------------|
| HTTP        | nome, protocolo, endereço, método HTTP, código HTTP esperado, intervalo |
| PING        | nome, protocolo, endereço, intervalo                                    |
| DNS         | nome, protocolo, endereço, intervalo, servidor DNS                      |

Abaixo, exemplo do arquivo descrito acima:

```txt
# monitoring.db

intra	HTTP	intra.42.fr	GET	200	120
game ping test	PING	game.42sp.org.br	60
workspaces monitoring	PING	workspaces.42sp.org.br	60
```

### monitoring

O programa `monitoring` lê o arquivo `monitoring.db` e realiza o monitoramento com base nas informações nele contidas, exibindo informações mais detalhadas no arquivo `monitoring.log` e mais simplificados na saida padrão.

Para utilizar programa basta executar o arquivo `monitoring`
```c
./monitoring
``` 

Também é possivel ler as informações no arquivo `monitoring.log` de maneira simplificada utilizando o comando --simplify
```c
./monitoring --simplify
```

## Desenvolvimento

O programa foi feito utilizando funções da libcurl para realizar os testes de rede necessários.

Código explicado:
-	[main.c](https://github.com/caiohmat/42-labs-v3/blob/main/readmefiles/main.md)
-	[get_data.c](https://github.com/caiohmat/42-labs-v3/blob/main/readmefiles/get_data.md)
-	[curl_setup.c](https://github.com/caiohmat/42-labs-v3/blob/main/readmefiles/curl_setup.md)
-	[monitoring_loop.c](https://github.com/caiohmat/42-labs-v3/blob/main/readmefiles/monitoring_loop.md)

[Arquivos do projeto](https://github.com/caiohmat/42-labs-v3)