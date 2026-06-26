import time
import json
import requests
from datetime import datetime
from threading import Thread
from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler

ESP_IP = "http://172.20.10.7/atualizar"  # Lembre-se de verificar se o seu IP mudou na ESP!
ARQUIVO_JSON = "cachorros.json"
LIMITE_HORAS = 12

def verificar_tempo_e_notificar():
    try:
        with open(ARQUIVO_JSON, 'r', encoding='utf-8') as f:
            cachorros = json.load(f)
        
        cachorros_em_alerta = []
        agora = datetime.now()

        print(f"\n[{agora.strftime('%H:%M:%S')}] Checando o estômago dos cachorros...")

        for c in cachorros:
            # Converte o texto do JSON em uma data/hora real que o Python entende
            hora_refeicao = datetime.strptime(c["ultima_refeicao"], "%Y-%m-%d %H:%M:%S")
            
            # Calcula a diferença de tempo
            diferenca = agora - hora_refeicao
            horas_sem_comer = int(diferenca.total_seconds() / 3600)
            
            print(f" - {c['nome']} está há {horas_sem_comer} horas sem comer.")

            if horas_sem_comer >= LIMITE_HORAS:
                # Cria uma cópia dos dados para enviar, incluindo as horas calculadas
                dados_alerta = c.copy()
                dados_alerta["tempo_sem_comer_horas"] = horas_sem_comer
                cachorros_em_alerta.append(dados_alerta)

        # Se houver cães famintos, envia para a ESP
        if cachorros_em_alerta:
            print(f"⚠️ Alerta! {len(cachorros_em_alerta)} cachorro(s) precisam comer! Enviando para a ESP...")
            resposta = requests.post(ESP_IP, json=cachorros_em_alerta, timeout=5)
            print("Resposta da ESP:", resposta.text)
        else:
            print("✅ Tudo sob controle. Nenhum cachorro em alerta.")

    except Exception as e:
        print("Erro ao processar:", e)

# Loop que roda em segundo plano contando o tempo de fato
def loop_relogio_automatico():
    while True:
        verificar_tempo_e_notificar()
        time.sleep(1800) # Checa a cada 30 minutos 

# Monitor de modificação manual do arquivo (caso você edite o JSON pra dizer que o cão comeu)
class MonitorarArquivo(FileSystemEventHandler):
    def on_modified(self, event):
        if event.src_path.endswith(ARQUIVO_JSON):
            print(f"\n[+] O arquivo {ARQUIVO_JSON} foi atualizado manualmente! Recalculando...")
            verificar_tempo_e_notificar()

if __name__ == "__main__":
    t = Thread(target=loop_relogio_automatico, daemon=True)
    t.start()

  
    event_handler = MonitorarArquivo()
    observer = Observer()
    observer.schedule(event_handler, path='.', recursive=False)
    observer.start()

    print("Contador automático e Monitor iniciados com sucesso!")
    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        observer.stop()
    observer.join()