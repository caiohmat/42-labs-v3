<h1>42 Labs 3º Edição</h1>

### Desafio : Aplicação de monitoramento de serviços web.

<h1></h1>

A função `curl_setup` inicializa sessões curl e altera as opções para sua utilização durante o monitoramento.

```c
int	curl_setup(t_config *config, FILE *temp_log)
{
	t_config	*temp;
	
	temp = config;

	//Cria o ambiente necessario ao curl.
	curl_global_init(CURL_GLOBAL_ALL);

	//Repete processo para todas as structs.
	while (temp->next)
	{
		//inicializa sessão curl e guarda ponteiro em curl_handle.
		temp->curl_handle = curl_easy_init();

		if (temp->curl_handle)
		{
			//Especifica endereço alvo.
			curl_easy_setopt(temp->curl_handle, CURLOPT_URL, temp->address);

			//redireciona saida padrão
			curl_easy_setopt(temp->curl_handle, CURLOPT_WRITEFUNCTION, write_data);

			//Manda infor~mações sobre a conexão para o STDERR.
			curl_easy_setopt(temp->curl_handle, CURLOPT_VERBOSE, 1L);

			//Redireciona STDERR para a o arquivo temporario temp_log.
			curl_easy_setopt(temp->curl_handle, CURLOPT_STDERR, temp_log);

			//Caso o protocolo especificado seja HTTP utiliza o método especificado na requisição.
			if (temp->protocol == 1)
				curl_easy_setopt(temp->curl_handle, CURLOPT_CUSTOMREQUEST, temp->DNSserver_HTTPtmethod);
		}
		else
		{
			printf("Error: Could not initiate curl for: %s\n", temp->name);
			return (1);
		}
		temp = temp->next;
	}
	return (0);
}
```