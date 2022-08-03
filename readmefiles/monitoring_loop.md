<h1>42 Labs 3º Edição</h1>

### Desafio : Aplicação de monitoramento de serviços web.

<h1></h1>

A função `monitoring_loop` realiza os testes de rede nos intervalos determinado até a finalização do programa. 

```c
void	monitoring_loop(t_config *config, FILE *temp_log, int log_file)
{
	t_config			*temp;
	
	//Modifica a variavel global.
	g_exit = 1;

	//Mantém o monitoramento até a variavel global ser modificada para zero.
	while (g_exit)
	{
		temp = config;

		//Realiza a verificação para todos os membros.
		while (temp->next)
		{
			//Realiza os testes caso o tempo decorrido do ultimo teste seja maior que o intervalo especificado.
			if ((ft_time() - temp->last_check) > temp->interval || !temp->last_check)
			{
				printf("\n--------------------------------------\n");
				dprintf(log_file, "\n--------------------------------------\n");
				if (temp->protocol == 1)
					run_HTTP_test(temp, temp_log, log_file);
				else if (temp->protocol == 2)
					run_PING_test(temp, temp_log, log_file);
				else
					run_DNS_test(temp);
				printf("\n--------------------------------------\n");
				dprintf(log_file, "\n--------------------------------------\n");
			}
			temp = temp->next;
		}
		usleep(100);
	}
}
```

<h1></h1>

A função `run_HTTP_test` realiza os testes de protocolo HTTP.

```c
void	run_HTTP_test(t_config *config, FILE *temp_log, int log_file)
{
	time_t	t;
	struct	tm tm;
	char	buffer[100];
	int		response;
	long 	response_code;

	printf("\n%s ", config->name);
	dprintf(log_file, "LOG: %s ", config->name);

	//Atualiza o momento do ultimo teste.
	config->last_check = ft_time();

	//Imprime a data e hora do teste realizado.
	t = time(NULL);
  	tm = *localtime(&t);
	dprintf(log_file, "[%d-%02d-%02d %02d:%02d:%02d]\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
  	printf("[%d-%02d-%02d %02d:%02d:%02d]\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

	//Reabre e trunca arquivo temp_log
	temp_log = freopen(NULL, "w", temp_log);

	//Realiza o requerimento conforme as opções estabelecidas.
	response = curl_easy_perform(config->curl_handle);

	//Reabre o arquivo temp_log em modo leitura
	temp_log = freopen(NULL, "r", temp_log);

	//Gera log caso a conexão seja estabelecida
	if (response == CURLE_OK)
	{
		//Lê arquivo temporario linha a linha e imprime informações relevantes para o log permanente e o terminal.
		while (fgets(buffer, 100, temp_log))
			generate_http_log(buffer, log_file);

		//Obtem o código HTTP retornado.
		curl_easy_getinfo(config->curl_handle, CURLINFO_RESPONSE_CODE, &response_code);

		//Faz a comparação entre o código HTTP recebido e o esperado e imprime o sucesso ou falha.
		if (response_code == config->HTTPcode)
		{
			printf("SUCCESS: expected %ld, got %ld\n", config->HTTPcode, response_code);
			dprintf(log_file, "SUCCESS: expected %ld, got %ld\n\n", config->HTTPcode, response_code);
		}
		else
		{
			printf("FAILED: expected %ld, got %ld\n", config->HTTPcode, response_code);
			dprintf(log_file, "FAILED: expected %ld, got %ld\n\n", config->HTTPcode, response_code);
		}
	}

	//Imprime erro em caso de falha na conexão.
	else
	{
		fgets(buffer, 100, temp_log);
		printf("Error: %s", buffer + 2);
		dprintf(log_file, "Error: %s", buffer + 2);
	}
}
```

<h1></h1>

A função `generate_http_log` imprime as informações relevante obtidas pelo requerimento no arquivo monitoring.log e no terminal.

```c
void	generate_http_log(char *buffer, int log_file)
{
	if (buffer[0] != '*' && !isspace(buffer[0]))
	{
		if (buffer[0] == '<' || buffer[0] == '>')
			buffer += 2;
		dprintf(log_file, "%s", buffer);
		if (strstr(buffer, "HTTP") || strstr(buffer, "Host:") || strstr(buffer, "Accept:"))
			printf("%s", buffer);
	}
}
```

<h1></h1>

A função `run_PING_test` realiza os testes de protocolo PING.

```c
void	run_PING_test(t_config *config, FILE *temp_log, int log_file)
{
	time_t	t;
	struct	tm tm;
	char	buffer[100];
	int		response;

	printf("\n%s ", config->name);
	dprintf(log_file, "\nLOG: %s ", config->name);

	//Atualiza o momento do ultimo teste.
	config->last_check = ft_time();

	//Imprime a data e hora do teste realizado.
	t = time(NULL);
  	tm = *localtime(&t);
	dprintf(log_file, "[%d-%02d-%02d %02d:%02d:%02d]\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
  	printf("[%d-%02d-%02d %02d:%02d:%02d]\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

	//Reabre e trunca arquivo temp_log
	temp_log = freopen(NULL, "w", temp_log);

	//Realiza o requerimento conforme as opções estabelecidas.
	response = curl_easy_perform(config->curl_handle);

	//Reabre o arquivo temp_log em modo leitura
	temp_log = freopen(NULL, "r", temp_log);

	//Imprime Informações sobre sucesso da conexão.
	if (response == CURLE_OK)
	{
		while (fgets(buffer, 100, temp_log))
		{
			if (strstr(buffer, "Connected"))
			{
				printf("SUCCESS: %s", buffer + 2);
				dprintf(log_file, "SUCCESS: %s\n", buffer + 2);
				break ;
			}
		}
	}

	//Imprime Informações sobre o fracasso da conexão.
	else
	{
		fgets(buffer, 100, temp_log);
		printf("FAILED: %s", buffer + 2);
		dprintf(log_file, "FAILED: %s\n", buffer + 2);
	}
}
```