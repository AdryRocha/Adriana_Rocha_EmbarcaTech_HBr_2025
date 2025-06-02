#ifndef WIFI_CONN_H // Definição do cabeçalho para evitar inclusões múltiplas
#define WIFI_CONN_H // Definição do cabeçalho para evitar inclusões múltiplas

/**
 * Função para conectar-se a uma rede Wi-Fi.
 * Objetivo: Conectar-se a uma rede Wi-Fi especificada pelo SSID e senha.
 * Parâmetros:
 *   - ssid: O SSID da rede Wi-Fi.
 *   - password: A senha da rede Wi-Fi.
 */
int connect_to_wifi(const char *ssid, const char *password); // Função para conectar-se a uma rede Wi-Fi

/**
 * Função para desconectar-se da rede Wi-Fi.
 * Objetivo: Desconectar-se da rede Wi-Fi atual.
 */
#endif // wifi_conn.h