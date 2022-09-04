# 1. O Trabalho

Este trabalho tem por objetivo a implementação de um sistema (que simula) o controle de um Air Fryer com controle digital.

O trablho envolve o desenvolvimento do software que efetua o controle completo da AirFyer, incluindo:
* Ligar/desligar o equipamento;
* Controlar a temperatura;
* Temporização e diferentes modos de alimentos.

Especificamente a temperatura do forno é controlada à partir de dois elementos atuadores:

* Resistor de potência de 15 Watts utilizado para aumentar temperatura;
* Ventoinha que puxa o ar externo (temperatura ambiente) para reduzir a temperatura do sistema.

Os comandos do usuário do sistema para definir a temperatura desejada serão controlados de três maneiras:

* Através de botões no painel de controle (Via UART);
* Através de entrada de teclado no terminal;
* Seguindo os tempo e temperaturas pré-definidas para cada tipo de alimento.

Botões de Entrada

* Liga/Desliga  
* Inicia/Cancela  
* Temperatura +/- (A cada 5 ˚C)  
* Tempo +/- (Minutos)



## 2. Execução do código

1) Acessar a placa raspberry pi desejada:

```sh 
ssh "nome_de_usuario"@"ip_da_placa"
```

2) Após a liberação de acesso à placa, clonar o repositório do projeto:

```sh 
git@github.com:ilus1/Tabralho-2.git
```

3) É possivel executar o programa diretamente através do comando:

```sh 
make run
```


# 3. Testes

## Pré-Aquecimento
![Pré-Aquecimento](/imgs/Pre-Aquecimento.png)

## Controle de temperatura com temporizador
![Funcionamento](/imgs/Funcionamento.png)

## Resfriamento
![Resfriamento](/imgs/Resfriamento.png)
