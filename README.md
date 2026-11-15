# 🌡️ Sensor de Ambiente ODS3

**Projeto de Internet das Coisas (IoT) – ESP32**

Projeto acadêmico desenvolvido com o objetivo de aplicar conceitos de Internet das Coisas (IoT), utilizando sensores, atuadores e comunicação MQTT para monitoramento de temperatura e sinalização de condições de atenção e alerta.

O projeto está relacionado ao **Objetivo de Desenvolvimento Sustentável 3 (ODS 3) – Saúde e Bem-Estar**, da Organização das Nações Unidas (ONU).

---

## 🎯 1. Objetivo do Projeto

Desenvolver um protótipo inteligente utilizando a plataforma ESP32 para monitorar a temperatura de um ambiente, identificar diferentes condições de funcionamento e emitir alertas visuais e sonoros conforme os valores identificados pelo sensor.

O sistema busca demonstrar como a tecnologia IoT pode contribuir para o monitoramento de condições ambientais, oferecendo informações em tempo real e possibilitando o acompanhamento remoto por meio da comunicação MQTT.

## 🌍 2. Relação com a ODS 3 – Saúde e Bem-Estar

A ODS 3 tem como finalidade assegurar uma vida saudável e promover o bem-estar para todos.

O projeto está relacionado a esse objetivo por demonstrar a aplicação da tecnologia no monitoramento de condições ambientais que podem influenciar o conforto e o bem-estar das pessoas.

Por meio da leitura de temperatura e da identificação de situações que exigem atenção, o protótipo demonstra como sistemas inteligentes podem auxiliar no acompanhamento de ambientes.

**Importante:** o protótipo possui finalidade educacional e experimental, não sendo um equipamento médico ou dispositivo de diagnóstico.

## 🔧 3. Componentes Utilizados

| Componente | Função |
|---|---|
| ESP32 | Microcontrolador responsável pelo processamento das informações e comunicação Wi-Fi. |
| Sensor NTC | Responsável pela leitura da temperatura. |
| LED RGB | Indica visualmente o estado identificado pelo sistema. |
| Buzzer | Emite alertas sonoros em situações de alerta. |
| Resistor | Utilizado no circuito para limitar a corrente elétrica. |
| Wi-Fi | Permite a conexão do ESP32 à internet. |
| MQTT | Protocolo utilizado para transmitir os dados de temperatura. |

## ⚙️ 4. Funcionamento do Sistema

O funcionamento começa com a leitura da temperatura realizada pelo sensor NTC, conectado à entrada analógica do ESP32.

O microcontrolador recebe os valores do sensor, realiza o processamento e identifica em qual faixa de temperatura o ambiente se encontra.

Conforme o resultado, o sistema aciona os componentes responsáveis pela sinalização visual e sonora.

### 4.1 Classificação da Temperatura

O protótipo utiliza quatro condições de monitoramento:

| Temperatura | Estado | Sinalização |
|---|---|---|
| Abaixo de 17 °C | Alerta | LED vermelho e buzzer |
| De 17 °C até abaixo de 28 °C | Normal | LED verde |
| De 28 °C até abaixo de 31 °C | Atenção | LED amarelo |
| 31 °C ou superior | Alerta | LED vermelho e buzzer |

### 4.2 Atuação dos Componentes

**Sensor NTC:** realiza a leitura da temperatura e fornece os valores ao ESP32 por meio de uma entrada analógica.

**ESP32:** processa os dados recebidos, classifica a temperatura e controla os atuadores. Também é responsável pela conexão Wi-Fi e comunicação MQTT.

**LED RGB:** apresenta três condições visuais:
- Verde: funcionamento normal.
- Amarelo: condição de atenção.
- Vermelho: condição de alerta.

**Buzzer:** é acionado quando a temperatura está abaixo de 17 °C ou igual ou superior a 31 °C, emitindo um sinal sonoro para indicar uma condição de alerta.

## 📡 5. Comunicação MQTT

O projeto utiliza o protocolo MQTT (Message Queuing Telemetry Transport), um protocolo de comunicação leve bastante utilizado em aplicações de Internet das Coisas.

O ESP32 conecta-se à rede Wi-Fi e estabelece comunicação com um broker MQTT, responsável por receber e disponibilizar as mensagens transmitidas pelos dispositivos.

Neste protótipo, é utilizado o broker público de testes:

- **Broker:** test.mosquitto.org
- **Porta:** 1883
- **Protocolo:** MQTT
- **Conectividade:** Wi-Fi

Após realizar a leitura e o processamento da temperatura, o ESP32 publica os dados no tópico MQTT configurado no código.

Dessa maneira, as informações podem ser recebidas por outros clientes MQTT inscritos no mesmo tópico, permitindo o acompanhamento remoto das medições.

## 🔄 6. Fluxo de Funcionamento

1. Inicialização do ESP32.
2. Configuração dos pinos do sensor, LED RGB e buzzer.
3. Conexão à rede Wi-Fi.
4. Conexão ao broker MQTT.
5. Leitura da temperatura pelo sensor NTC.
6. Processamento dos dados pelo ESP32.
7. Classificação da temperatura.
8. Acionamento do LED RGB e do buzzer conforme a condição identificada.
9. Publicação dos dados no broker MQTT.
10. Repetição do processo continuamente.

## 💻 7. Tecnologias Utilizadas

- ESP32
- Arduino / C++
- Arduino IDE ou Visual Studio Code
- Simulador Wokwi
- Wi-Fi
- Protocolo MQTT
- Broker Mosquitto

## 🧪 8. Ambiente de Desenvolvimento

O protótipo foi desenvolvido utilizando o simulador **Wokwi**, que permite a montagem e simulação de circuitos eletrônicos e microcontroladores.

A utilização do simulador possibilita testar o comportamento dos sensores e atuadores, além de verificar a comunicação do ESP32 com a rede Wi-Fi e o broker MQTT.

## 📚 9. Considerações Finais

O desenvolvimento deste projeto permitiu aplicar conhecimentos relacionados à Internet das Coisas, programação de microcontroladores, leitura de sensores, acionamento de atuadores e comunicação entre dispositivos.

A integração entre ESP32, sensor NTC, LED RGB, buzzer e protocolo MQTT demonstra como diferentes tecnologias podem trabalhar em conjunto para desenvolver soluções inteligentes de monitoramento.

Além do aprendizado técnico, o projeto demonstra a possibilidade de utilizar a tecnologia como ferramenta de apoio à promoção da saúde e do bem-estar, em alinhamento com a ODS 3.

---

**Projeto acadêmico – Sensor de Ambiente ODS3**

Desenvolvido por: Kayque Santos

**Tema:** Internet das Coisas (IoT) e Saúde e Bem-Estar.

**Objetivo de Desenvolvimento Sustentável:** ODS 3 – Saúde e Bem-Estar.