#Segurança em IoT com BitDogLab (C/C++)

##Este projeto foca na segurança da comunicação MQTT para dispositivos IoT usando o Raspberry Pi Pico W. Ele demonstra como implementar medidas essenciais de segurança, como autenticação, ofuscação de dados (criptografia leve) e prevenção de ataques de replay. As etapas práticas são projetadas para serem executadas em uma única placa BitDogLab, adaptando-se de um cenário que normalmente envolveria duas placas

##Os principais objetivos incluem:

- Estabelecer conectividade Wi-Fi usando o Pico W SDK e LwIP.
- Configurar comunicação MQTT básica para conectar a um broker e publicar dados em tópicos específicos.
- Implementar autenticação para conexões de cliente MQTT.
- Aplicar uma cifra XOR simples para criptografia leve para ofuscar o conteúdo da mensagem.
- Adicionar timestamps às mensagens e validá-las no lado do assinante para prevenir ataques de replay.

##Materiais Necessários
Para replicar e trabalhar com este projeto, você precisará de:

- Uma placa BitDogLab com Raspberry Pi Pico W.
- Uma rede Wi-Fi protegida (um hotspot de celular pode ser usado).
- Um computador (Notebook/Desktop) com:
- VSCode com extensões CMake e Pico SDK instaladas.
- Mosquitto MQTT broker instalado e configurado no PATH do sistema.
- Wireshark instalado para análise de tráfego de rede.

##Configuração de Software

- Extensões VSCode: Certifique-se de ter as extensões "CMake Tools" e "Pico SDK" instaladas no VSCode. Essas extensões fornecem as funcionalidades necessárias para construir e depurar projetos Pico.
- Pico SDK: O projeto assume que o Pico SDK está corretamente configurado e acessível. O arquivo CMakeLists.txt inclui uma seção para inicializar o Pico SDK a partir de sua localização de instalação.
- Mosquitto Broker:
  - Baixe e instale o Mosquitto em https://mosquitto.org/download/.
  - Certifique-se de que o Mosquitto esteja adicionado à variável de ambiente PATH do seu sistema para fácil acesso via linha de comando.
- Wireshark:
  - Baixe e instale o Wireshark em https://www.wireshark.org/download.html. Esta ferramenta é crucial para observar o tráfego de rede e verificar a criptografia.

#Estrutura do Projeto
O projeto consiste em vários arquivos C (.c) e cabeçalho (.h), juntamente com arquivos de configuração CMake:

├── CMakeLists.txt

├── lwipopts.h

├── main.c

├── mqtt_comm.c

├── mqtt_comm.h

├── wifi_conn.c

└── wifi_conn.h

- CMakeLists.txt: Arquivo de configuração de build para CMake.
- lwipopts.h: Configurações personalizadas para a pilha de rede LwIP.
- main.c: A lógica principal da aplicação, coordenando Wi-Fi, comunicação MQTT e implementando recursos de segurança.
- mqtt_comm.c / mqtt_comm.h: Lida com a inicialização do cliente MQTT, conexão, publicação e callbacks de subscrição.
- wifi_conn.c / wifi_conn.h: Gerencia o estabelecimento da conexão Wi-Fi.


_____________________________________________________________________________________________

#Licença
sob a licença MIT em https://opensource.org/licenses/MIT .

_____________________________________________________________________________________________

#Adriana Rocha










