#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

const char* ssid = "iPhone de Felipe";
const char* password = "12345678";

WebServer server(80);

// String global que vai guardar o HTML da página com os dados dos cachorros
String paginaWeb = "";

// Função que cria o design do site usando HTML
void atualizarPaginaWeb(JsonArray lista) {
  paginaWeb = "<!DOCTYPE html><html lang='pt-BR'><head>";
  paginaWeb += "<meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  paginaWeb += "<title>Alerta - Alimentação Pet</title>";
  paginaWeb += "<style>";
  paginaWeb += "body { font-family: Arial, sans-serif; background-color: #f4f4f9; margin: 0; padding: 20px; text-align: center; }";
  paginaWeb += "h1 { color: #d9534f; }";
  paginaWeb += ".container { display: flex; flex-direction: column; align-items: center; gap: 15px; margin-top: 20px; }";
  paginaWeb += ".card { background: white; padding: 20px; border-radius: 10px; box-shadow: 0 4px 8px rgba(0,0,0,0.1); border-left: 5px solid #d9534f; width: 300px; text-align: left; }";
  paginaWeb += ".card h3 { margin: 0 0 10px 0; color: #333; }";
  paginaWeb += ".card p { margin: 5px 0; color: #666; }";
  paginaWeb += ".alerta { color: #d9534f; font-weight: bold; }";
  paginaWeb += "</style></head><body>";
  
  paginaWeb += "<h1>⚠️ Alerta: Cães Sem Comer há +12h</h1>";
  paginaWeb += "<p>Os seguintes cachorros precisam de atenção imediatamente:</p>";
  paginaWeb += "<div class='container'>";

  // Adiciona cada cachorro como um "card" no site
  for (JsonObject cachorro : lista) {
    const char* nome = cachorro["nome"];
    const char* raca = cachorro["raca"];
    int idade = cachorro["idade"];
    float peso = cachorro["peso"];
    int tempo = cachorro["tempo_sem_comer_horas"];

    paginaWeb += "<div class='card'>";
    paginaWeb += "<h3>" + String(nome) + "</h3>";
    paginaWeb += "<p><strong>Raça:</strong> " + String(raca) + "</p>";
    paginaWeb += "<p><strong>Idade:</strong> " + String(idade) + " anos</p>";
    paginaWeb += "<p><strong>Peso:</strong> " + String(peso) + " kg</p>";
    paginaWeb += "<p class='alerta'><strong>Sem comer há:</strong> " + String(tempo) + " horas</p>";
    paginaWeb += "</div>";
  }

  paginaWeb += "</div></body></html>";
}

// Quando você abre o IP da ESP no navegador, ela entrega essa página
void disponibilizarSite() {
  if (paginaWeb == "") {
    // Página inicial caso nenhum alerta tenha sido enviado ainda
    String paginaVazia = "<!DOCTYPE html><html><head><meta charset='UTF-8'></head><body style='font-family:Arial; text-align:center; padding-top:50px;'>";
    paginaVazia += "<h1>🐾 Sistema de Alerta Pet</h1><p>Nenhum cachorro em estado de alerta no momento. Tudo limpo!</p></body></html>";
    server.send(200, "text/html", paginaVazia);
  } else {
    server.send(200, "text/html", paginaWeb);
  }
}

// Rota onde o Python vai "postar" os dados novos
void tratarAtualizacao() {
  if (server.hasArg("plain") == false) {
    server.send(400, "text/plain", "Corpo vazio");
    return;
  }

  String dadosRecebidos = server.arg("plain");
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, dadosRecebidos);

  if (error) {
    server.send(400, "text/plain", "JSON Invalido");
    return;
  }

  // Atualiza a página do site com os novos dados recebidos
  JsonArray lista = doc.as<JsonArray>();
  atualizarPaginaWeb(lista);

  Serial.println("[ESP] Site atualizado com novos alertas!");
  server.send(200, "text/plain", "ESP atualizada com sucesso!");
}

void setup() {
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nConectado!");
  Serial.print("Acesse o site digitando este IP no navegador: ");
  Serial.println(WiFi.localIP());

  // Rota 1: O Site visível para você
  server.on("/", HTTP_GET, disponibilizarSite);
  
  // Rota 2: Onde o Python envia os dados pelas costas
  server.on("/atualizar", HTTP_POST, tratarAtualizacao);
  
  server.begin();
}

void loop() {
  server.handleClient();
}