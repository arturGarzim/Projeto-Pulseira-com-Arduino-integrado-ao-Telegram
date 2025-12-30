# 📡 **ESP8266 – SISTEMA DE TESTES WIFI E INTEGRAÇÃO COM TELEGRAM**
---

## 📖 Descrição

Este projeto implementa um **sistema completo de testes e diagnóstico para o ESP8266**, focado em **conectividade WiFi**, **acesso à internet**, **comunicação segura via HTTPS (SSL)** e **envio de mensagens 
pelo Telegram** utilizando a **Bot API**.Todo o processo é executado automaticamente durante o `setup()`, exibindo **logs detalhados no Serial Monitor**, facilitando a depuração e validação do ambiente de rede.

---

## 🚀 Funcionalidades

- 🔍 **Diagnóstico do sistema**
  - Chip ID, memória livre, flash, SDK, versão do core e uptime
- 📶 **Escaneamento de redes WiFi**
  - Lista redes disponíveis
  - Exibe RSSI (intensidade do sinal)
  - Identifica automaticamente a rede configurada
- 🔗 **Conexão WiFi avançada**
  - Tentativas controladas
  - Logs detalhados de status
  - Reconexão automática em caso de falha
- 🌐 **Testes de conectividade com a internet**
  - Resolução de DNS
  - Teste HTTP com servidor externo
- 🔐 **Comunicação segura (HTTPS / SSL)**
  - Uso de `WiFiClientSecure`
  - Conexão segura com servidores HTTPS
- 🤖 **Integração com Telegram**
  - Envio de mensagens via bot
  - Requisições HTTPS manuais
  - Validação da resposta do servidor
- 📊 **Resumo final**
  - Status do WiFi
  - Memória disponível
  - Tempo de execução do sistema

---

## 🧪 Etapas de Teste Executadas

1. Informações do sistema
2. Escaneamento de redes WiFi
3. Conexão à rede WiFi
4. Testes de conectividade com a internet (DNS e HTTP)
5. Configuração e teste SSL
6. Envio de mensagens via Telegram
7. Resumo final do sistema

---

## ⚙️ Configuração

Antes de compilar e enviar o código para o ESP8266, configure as variáveis abaixo:

```cpp
const char WIFI_SSID[] = "NOME_DA_REDE";
const char WIFI_PASSWORD[] = "SENHA_DA_REDE";
const char BOT_TOKEN[] = "TOKEN_DO_BOT_TELEGRAM";
const char CHAT_ID[] = "CHAT_ID_DO_TELEGRAM";

•• NO VÍDEO DO HARDWARE DO PROJETO ••
- O fio verde está ligado no pino D5 do ARDUINO
- O fio preto está ligado no pino GND do ARDUINO e abastece negativamente na PROTOBOARD




