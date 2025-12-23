#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

// ===== CONFIGURAÇÕES =====
const char WIFI_SSID[] = "seu wifi aqui"; //////////////////
const char WIFI_PASSWORD[] = "sua senha do wifi aqui"; /////////////////
const char BOT_TOKEN[] = "seu token do telegram aqui";
const char CHAT_ID[] = "seu id do telegram aqui";
const int TIMEOUT = 30000;

// ===== VARIÁVEIS GLOBAIS =====
WiFiClientSecure client;

void printSeparador(String titulo) {
  Serial.println("\n");
  Serial.println("╔════════════════════════════════════════════════╗");
  Serial.print("║   ");
  Serial.print(titulo);
  for (int i = titulo.length(); i < 44; i++) Serial.print(" ");
  Serial.println("║");
  Serial.println("╚════════════════════════════════════════════════╝");
}

void printTeste(String num, String descricao) {
  Serial.print("\n[TESTE ");
  Serial.print(num);
  Serial.print("] ");
  Serial.println(descricao);
}

void printSucesso(String mensagem) {
  Serial.print("✓ ");
  Serial.println(mensagem);
}

void printErro(String mensagem) {
  Serial.print("✗ ERRO: ");
  Serial.println(mensagem);
}


bool enviarMensagemTelegram(String mensagem) {
  printTeste("TELEGRAM", "Preparando envio de mensagem...");
  
  Serial.print("Mensagem: ");
  Serial.println(mensagem);
  Serial.print("Bot Token: ");
  Serial.println(BOT_TOKEN);
  Serial.print("Chat ID: ");
  Serial.println(CHAT_ID);
  
  // Constroi a URL
  String url = "/bot" + String(BOT_TOKEN) + "/sendMessage";
  String parametros = "?chat_id=" + String(CHAT_ID) + "&text=" + mensagem;
  
  Serial.println("\n[TELEGRAM] Conectando ao api.telegram.org...");
  Serial.println("Host: api.telegram.org");
  Serial.println("Porta: 443 (HTTPS)");
  
  if (!client.connect("api.telegram.org", 443)) {
    printErro("Falha ao conectar ao Telegram");
    Serial.println("Status da conexão: " + String(client.connected()));
    return false;
  }
  
  printSucesso("Conectado ao Telegram!");
  
  // Monta requisição HTTP
  String request = "GET " + url + parametros + " HTTP/1.1\r\n";
  request += "Host: api.telegram.org\r\n";
  request += "Connection: close\r\n";
  request += "\r\n";
  
  Serial.println("\n[TELEGRAM] Enviando requisição HTTP...");
  Serial.println("Requisição completa:");
  Serial.println(request);
  
  client.print(request);
  Serial.println("Requisição enviada!");
  
  // Aguarda resposta
  Serial.println("\n[TELEGRAM] Aguardando resposta do servidor...");
  unsigned long inicio = millis();
  String resposta = "";
  
  while ((unsigned long)(millis() - inicio) < 10000) {
    while (client.available()) {
      char c = client.read();
      resposta += c;
      Serial.write(c);
    }
    if (resposta.length() > 0 && resposta.indexOf("200 OK") != -1) {
      break;
    }
  }
  
  // Lê resto da resposta
  delay(1000);
  while (client.available()) {
    char c = client.read();
    resposta += c;
    Serial.write(c);
  }
  
  Serial.println("\n[TELEGRAM] Resposta completa recebida!");
  Serial.print("Tamanho da resposta: ");
  Serial.print(resposta.length());
  Serial.println(" bytes");
  
  client.stop();
  
  if (resposta.indexOf("\"ok\":true") != -1 || resposta.indexOf("200 OK") != -1) {
    printSucesso("Mensagem enviada com sucesso!");
    Serial.println("\n[TELEGRAM] Resposta JSON:");
    int inicioJson = resposta.indexOf("{");
    if (inicioJson != -1) {
      String json = resposta.substring(inicioJson);
      Serial.println(json);
    }
    return true;
  } else {
    printErro("Resposta indica falha");
    Serial.println("Resposta recebida:");
    Serial.println(resposta);
    return false;
  }
}

void setup() {
  // Inicializa Serial
  Serial.begin(9600);
  delay(1000);
  
  Serial.println("\n\n\n");
  printSeparador("SISTEMA DE TESTE ESP8266 - TELEGRAM");
  Serial.println("\nIniciando sistema de testes...");
  Serial.print("Versão do Core: ");
  Serial.println(ESP.getCoreVersion());
  Serial.print("Frequência da CPU: ");
  Serial.print(ESP.getCpuFreqMHz());
  Serial.println(" MHz");
  Serial.print("Flash Size: ");
  Serial.print(ESP.getFlashChipSize() / 1024);
  Serial.println(" KB");
  Serial.print("Free Heap: ");
  Serial.print(ESP.getFreeHeap());
  Serial.println(" bytes");
  
  // Teste 1: Informações do Sistema
  printSeparador("TESTE 1 - INFORMACOES DO SISTEMA");
  Serial.print("Chip ID: ");
  Serial.println(ESP.getChipId(), HEX);
  Serial.print("Flash Chip ID: ");
  Serial.println(ESP.getFlashChipId(), HEX);
  Serial.print("SDK Version: ");
  Serial.println(ESP.getSdkVersion());
  Serial.print("Boot Version: ");
  Serial.println(ESP.getBootVersion());
  Serial.print("Free Sketch Space: ");
  Serial.print(ESP.getFreeSketchSpace());
  Serial.println(" bytes");
  printSucesso("Informações do sistema obtidas!");
  delay(1000);
  
  // Teste 2: WiFi - Listar Redes Disponíveis
  printSeparador("TESTE 2 - ESCANEAMENTO DE REDES WIFI");
  Serial.println("Escaneando redes WiFi disponíveis...");
  int numRedes = WiFi.scanNetworks();
  Serial.print("Redes encontradas: ");
  Serial.println(numRedes);
  
  // Variáveis para salvar informações da rede alvo
  int canalRede = 0;
  int rssiRede = 0;
  bool redeEncontrada = false;
  
  if (numRedes > 0) {
    Serial.println("\nLista de redes:");
    int maxRedes = (numRedes > 10) ? 10 : (int)numRedes;
    for (int i = 0; i < maxRedes; i++) {
      String ssidAtual = WiFi.SSID(i);
      Serial.print("  ");
      Serial.print(i + 1);
      Serial.print(". ");
      Serial.print(ssidAtual);
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(" dBm) - ");
      if (WiFi.encryptionType(i) == ENC_TYPE_NONE) {
        Serial.println("Aberto");
      } else {
        Serial.println("Protegido");
      }
      
      // Compara SSID (remove espaços extras)
      ssidAtual.trim();
      String ssidAlvo = String(WIFI_SSID);
      ssidAlvo.trim();
      
      if (ssidAtual == ssidAlvo) {
        redeEncontrada = true;
        canalRede = WiFi.channel(i);
        rssiRede = WiFi.RSSI(i);
        Serial.print("    ✓ ESTA É A REDE ALVO! Canal: ");
        Serial.print(canalRede);
        Serial.print(", RSSI: ");
        Serial.print(rssiRede);
        Serial.println(" dBm");
      }
    }
    printSucesso("Escaneamento concluído!");
    if (redeEncontrada) {
      Serial.print("✓ Rede alvo encontrada no escaneamento! Canal: ");
      Serial.print(canalRede);
      Serial.print(", RSSI: ");
      Serial.print(rssiRede);
      Serial.println(" dBm");
    } else {
      Serial.println("⚠ Rede alvo não encontrada no escaneamento!");
    }
  } else {
    printErro("Nenhuma rede encontrada");
  }
  delay(1000);
  
  // Teste 3: Conectar ao WiFi
  printSeparador("TESTE 3 - CONEXAO WIFI");
  Serial.print("SSID: ");
  Serial.println(WIFI_SSID);
  Serial.print("Password: ");
  Serial.println(WIFI_PASSWORD);
  Serial.print("Tamanho da senha: ");
  Serial.print(strlen(WIFI_PASSWORD));
  Serial.println(" caracteres");
  
  Serial.println("\nConfigurando WiFi...");
  Serial.print("SSID alvo: [");
  Serial.print(WIFI_SSID);
  Serial.println("]");
  Serial.print("Tamanho do SSID: ");
  Serial.print(strlen(WIFI_SSID));
  Serial.println(" caracteres");
  
  if (redeEncontrada && canalRede > 0) {
    Serial.print("✓ Usando informações do escaneamento: Canal ");
    Serial.print(canalRede);
    Serial.print(", RSSI ");
    Serial.print(rssiRede);
    Serial.println(" dBm");
  } else {
    Serial.println("⚠ Conectando sem informações específicas do canal");
  }
  
  // Desconecta completamente antes de tentar conectar
  WiFi.disconnect(true);
  delay(2000);
  
  Serial.println("Configurando modo Station...");
  WiFi.mode(WIFI_STA);
  WiFi.setSleepMode(WIFI_NONE_SLEEP); // Desabilita sleep mode
  delay(500);
  
  Serial.println("Iniciando conexão WiFi...");
  
  // Tenta conectar - primeiro sem especificar canal
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  Serial.print("MAC Address do ESP8266: ");
  Serial.println(WiFi.macAddress());
  
  int tentativas = 0;
  int maxTentativas = 60; // Aumentado para 30 segundos
  Serial.println("\nAguardando conexão...");
  
  while (WiFi.status() != WL_CONNECTED && tentativas < maxTentativas) {
    delay(500);
    tentativas++;
    
    // Mostra status detalhado a cada 5 tentativas
    if (tentativas % 5 == 0) {
      Serial.print("\n[Tentativa ");
      Serial.print(tentativas);
      Serial.print("/");
      Serial.print(maxTentativas);
      Serial.print("] Status: ");
      int status = WiFi.status();
      switch(status) {
        case WL_IDLE_STATUS:
          Serial.print("IDLE");
          break;
        case WL_NO_SSID_AVAIL:
          Serial.print("NO SSID AVAIL");
          break;
        case WL_SCAN_COMPLETED:
          Serial.print("SCAN COMPLETED");
          break;
        case WL_CONNECTED:
          Serial.print("CONNECTED");
          break;
        case WL_CONNECT_FAILED:
          Serial.print("CONNECT FAILED");
          break;
        case WL_CONNECTION_LOST:
          Serial.print("CONNECTION LOST");
          break;
        case WL_DISCONNECTED:
          Serial.print("DISCONNECTED");
          break;
        default:
          Serial.print("UNKNOWN (");
          Serial.print(status);
          Serial.print(")");
      }
      Serial.print(" | RSSI: ");
      if (WiFi.status() == WL_CONNECTED) {
        Serial.print(WiFi.RSSI());
        Serial.println(" dBm");
      } else {
        Serial.println("N/A");
      }
    } else {
      Serial.print(".");
    }
  }
  Serial.println();
  
  if (WiFi.status() == WL_CONNECTED) {
    printSucesso("Conectado ao WiFi!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    Serial.print("Gateway: ");
    Serial.println(WiFi.gatewayIP());
    Serial.print("Subnet Mask: ");
    Serial.println(WiFi.subnetMask());
    Serial.print("DNS: ");
    Serial.println(WiFi.dnsIP());
    Serial.print("RSSI: ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
    Serial.print("MAC Address: ");
    Serial.println(WiFi.macAddress());
    Serial.print("Canal: ");
    Serial.println(WiFi.channel());
    Serial.print("Tempo de conexão: ");
    Serial.print(tentativas * 0.5);
    Serial.println(" segundos");
  } else {
    printErro("Falha ao conectar ao WiFi");
    Serial.print("Status final: ");
    int status = WiFi.status();
    Serial.println(status);
    Serial.println("\nPossíveis causas:");
    Serial.println("  1. Senha incorreta");
    Serial.println("  2. Rede muito distante (sinal fraco)");
    Serial.println("  3. Rede não aceita novos dispositivos");
    Serial.println("  4. Problema de compatibilidade");
    Serial.println("\nTentando reconexão automática...");
    
    // Tenta reconectar mais algumas vezes
    for (int i = 0; i < 3; i++) {
      Serial.print("Tentativa de reconexão ");
      Serial.print(i + 1);
      Serial.println("/3");
      WiFi.disconnect();
      delay(1000);
      WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
      delay(5000);
      if (WiFi.status() == WL_CONNECTED) {
        printSucesso("Reconectado após tentativas adicionais!");
        Serial.print("IP: ");
        Serial.println(WiFi.localIP());
        break;
      }
    }
    
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("\n✗✗✗ Não foi possível conectar ao WiFi ✗✗✗");
      Serial.println("Teste interrompido - WiFi necessário!");
      return;
    }
  }
  delay(2000);
  
  // Teste 4: Conectividade Internet
  printSeparador("TESTE 4 - CONECTIVIDADE INTERNET");
  Serial.println("Testando conectividade com internet...");
  
  // Teste de DNS
  printTeste("4.1", "Resolvendo DNS para google.com...");
  IPAddress ipGoogle;
  if (WiFi.hostByName("google.com", ipGoogle)) {
    printSucesso("DNS resolvido!");
    Serial.print("IP de google.com: ");
    Serial.println(ipGoogle);
  } else {
    printErro("Falha ao resolver DNS");
  }
  delay(1000);
  
  // Teste de conexão HTTP simples
  printTeste("4.2", "Testando conexão HTTP com httpbin.org...");
  WiFiClient httpClient;
  if (httpClient.connect("httpbin.org", 80)) {
    printSucesso("Conexão HTTP estabelecida!");
    httpClient.println("GET /get HTTP/1.1");
    httpClient.println("Host: httpbin.org");
    httpClient.println("Connection: close");
    httpClient.println();
    
    delay(1000);
    String resposta = "";
    while (httpClient.available()) {
      char c = httpClient.read();
      resposta += c;
    }
    httpClient.stop();
    
    if (resposta.indexOf("200 OK") != -1) {
      printSucesso("Resposta HTTP recebida!");
      Serial.print("Tamanho da resposta: ");
      Serial.print(resposta.length());
      Serial.println(" bytes");
    }
  } else {
    printErro("Falha ao conectar HTTP");
  }
  delay(2000);
  
  // Teste 5: Certificados SSL
  printSeparador("TESTE 5 - CONFIGURACAO SSL");
  Serial.println("Configurando cliente SSL seguro...");
  
  // Desabilita verificação de certificado (para testes)
  client.setInsecure();
  Serial.println("Modo inseguro ativado (sem verificação de certificado)");
  
  // Teste de conexão SSL
  printTeste("5.1", "Testando conexão SSL com api.telegram.org...");
  Serial.println("Tentando conectar...");
  if (client.connect("api.telegram.org", 443)) {
    printSucesso("Conexão SSL estabelecida!");
    Serial.println("Conexão segura funcionando!");
    client.stop();
  } else {
    printErro("Falha ao conectar SSL");
    Serial.println("Verifique certificados ou use setInsecure()");
  }
  delay(2000);
  
  // Teste 6: ENVIO DE MENSAGEM TELEGRAM
  printSeparador("TESTE 6 - ENVIO TELEGRAM - MENSAGEM 1");
  String mensagem1 = "Pulseira acionada!!!"; ///////////////////
  bool enviado1 = enviarMensagemTelegram(mensagem1);
  delay(3000);
  
  if (enviado1) {
    printSeparador("TESTE 7 - ENVIO TELEGRAM - MENSAGEM 2");
    String mensagem2 = "PERIGO"; ///////////////////
    bool enviado2 = enviarMensagemTelegram(mensagem2);
    delay(3000);
    
    if (enviado2) {
      printSeparador("🚨🚨🚨");
      String mensagem3 = "Teste 3: Terceira mensagem! Sistema funcionando perfeitamente!"; ///////////////////
      enviarMensagemTelegram(mensagem3);
      delay(3000);
    }
  }
  
  // Teste Final: Resumo
  printSeparador("RESUMO DOS TESTES");
  Serial.println("Status Final:");
  Serial.print("  WiFi: ");
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("✓ Conectado");
    Serial.print("    IP: ");
    Serial.println(WiFi.localIP()); 
  } else {
    Serial.println("✗ Desconectado");
  }
  Serial.print("  Free Heap: ");
  Serial.print(ESP.getFreeHeap());
  Serial.println(" bytes");
  Serial.print("  Uptime: ");
  Serial.print(millis() / 1000);
  Serial.println(" segundos");
  
  Serial.println("\n\n╔════════════════════════════════════════════════╗");
  Serial.println("║   TESTES CONCLUIDOS!                        ║");
  Serial.println("╚════════════════════════════════════════════╝\n");
}

void loop() {
  // Loop vazio - todos os testes são feitos no setup
  static unsigned long ultimoPrint = 0;
  if (millis() - ultimoPrint > 10000) {
    Serial.print("\n[Loop] Sistema ativo - Uptime: ");
    Serial.print(millis() / 1000);
    Serial.print("s | WiFi: ");
    if (WiFi.status() == WL_CONNECTED) {
      Serial.print("Conectado (");
      Serial.print(WiFi.RSSI());
      Serial.println(" dBm)");
    } else {
      Serial.println("Desconectado");
    }
    ultimoPrint = millis();
  }
  delay(100);
}
